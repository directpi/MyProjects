#include "web_server.h"

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include "libs/httplib.h"
#include "libs/json.hpp"

#include <unistd.h>  // mkstemp, close

using json = nlohmann::json;

WebServer::WebServer() : game(Game::create()) {}

namespace {

const char* get_env(const char* key) {
  if (!key) return nullptr;
  return std::getenv(key);
}

std::string get_env_or(const char* key, const char* default_value) {
  const char* v = get_env(key);
  if (v && *v) return std::string(v);
  return std::string(default_value ? default_value : "");
}

int get_env_int_or(const char* key, int default_value) {
  const char* v = get_env(key);
  if (!v || !*v) return default_value;
  try {
    return std::stoi(v);
  } catch (...) {
    return default_value;
  }
}

struct TempFileGuard {
  std::string path;
  explicit TempFileGuard(std::string p) : path(std::move(p)) {}
  ~TempFileGuard() {
    if (!path.empty()) std::remove(path.c_str());
  }
  TempFileGuard(const TempFileGuard&) = delete;
  TempFileGuard& operator=(const TempFileGuard&) = delete;
  TempFileGuard(TempFileGuard&& other) noexcept : path(std::move(other.path)) {
    other.path.clear();
  }
  TempFileGuard& operator=(TempFileGuard&& other) noexcept {
    if (this != &other) {
      if (!path.empty()) std::remove(path.c_str());
      path = std::move(other.path);
      other.path.clear();
    }
    return *this;
  }
};

std::optional<std::pair<TempFileGuard, std::string>> create_temp_file() {
  // NOTE: mkstemp creates the file atomically with 0600 permissions.
  char tmpl[] = "/tmp/maze_upload_XXXXXX";
  int fd = ::mkstemp(tmpl);
  if (fd == -1) return std::nullopt;
  ::close(fd);
  std::string filename = tmpl;
  return std::make_pair(TempFileGuard(filename), filename);
}

bool is_in_bounds(const MazeInfo& maze, const std::pair<int, int>& p) {
  const int rows = static_cast<int>(maze.downWall.size());
  const int cols = rows > 0 ? static_cast<int>(maze.downWall[0].size()) : 0;
  return p.first >= 0 && p.second >= 0 && p.first < rows && p.second < cols;
}

}  // namespace

void to_json(json& j, const MazeInfo& m) {
  j = json{{"rows", m.downWall.size()},
           {"cols", m.downWall[0].size()},
           {"RightWalls", m.rightWall},
           {"DownWalls", m.downWall}};
}
void from_json(const json& j, std::pair<int, int>& p) {
  j.at("row").get_to(p.first);
  j.at("col").get_to(p.second);
}

void WebServer::Run() {
  httplib::Server svr;

  // DoS protection: limit request body size (covers JSON and multipart payloads).
  constexpr size_t kMaxPayloadBytes = 2 * 1024 * 1024;  // 2 MiB
  svr.set_payload_max_length(kMaxPayloadBytes);

  const std::string cors_origin =
      get_env_or("MAZE_CORS_ORIGIN", "http://localhost:3000");

  auto cors_handler = [&](const httplib::Request& /*req*/,
                          httplib::Response& res) {
    res.set_header("Access-Control-Allow-Origin", cors_origin);
    res.set_header("Vary", "Origin");
    res.set_header("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
    res.set_header("Access-Control-Allow-Headers",
                   "Content-Type, Authorization");
    res.set_header("Access-Control-Max-Age", "600");
    return httplib::Server::HandlerResponse::Unhandled;
  };

  svr.Options("/(.*)", [](const httplib::Request&, httplib::Response& res) {
    res.status = 204;
  });

  svr.new_task_queue = [] { return new httplib::ThreadPool(8); };
  svr.set_pre_routing_handler(cors_handler);

  svr.Get("/", [](const httplib::Request&, httplib::Response& res) {
    res.set_content("Maze C++ Web Server is running!", "text/plain");
  });

  svr.Post("/api/maze/load", [&](const httplib::Request& req,
                                 httplib::Response& res) {
    if (!req.form.has_file("maze_file")) {
      json error_response = {{"error", "Файл не был предоставлен в запросе."}};
      res.status = 400;
      res.set_content(error_response.dump(), "application/json");
      return;
    }
    const auto& file = req.form.get_file("maze_file");

    if (file.content.size() > kMaxPayloadBytes) {
      json error_response = {{"error", "Файл слишком большой."}};
      res.status = 413;
      res.set_content(error_response.dump(), "application/json");
      return;
    }

    auto tmp = create_temp_file();
    if (!tmp.has_value()) {
      json error_response = {
          {"error",
           std::string("Внутренняя ошибка сервера: temp file: ") +
               std::strerror(errno)}};
      res.status = 500;
      res.set_content(error_response.dump(), "application/json");
      return;
    }
    auto& [guard, temp_filename] = *tmp;

    std::ofstream temp_file(temp_filename, std::ios::binary);
    if (!temp_file) {
      json error_response = {
          {"error",
           "Внутренняя ошибка сервера: не удалось создать временный файл."}};
      res.status = 500;
      res.set_content(error_response.dump(), "application/json");
      return;
    }
    temp_file << file.content;
    temp_file.close();

    try {
      game.get()->createDungeons(DungeonType::MAZE, temp_filename);
    } catch (const std::runtime_error& e) {
      json error_response = {
          {"error", "Неверный формат файла лабиринта. Проверьте данные."}};
      res.status = 400;
      res.set_content(error_response.dump(), "application/json");
      return;
    }

    auto maze = std::get<MazeInfo>(game.get()->getGameInfo());

    json success_response;
    to_json(success_response, maze);
    res.status = 200;
    res.set_content(success_response.dump(), "application/json");
  });

  svr.Get("/api/maze/generate", [&](const httplib::Request& req,
                                    httplib::Response& res) {
    if (!req.has_param("rows") || !req.has_param("cols")) {
      json error_response = {
          {"error", "Параметры 'rows' и 'cols' обязательны."}};
      res.status = 400;
      res.set_content(error_response.dump(), "application/json");
      return;
    }
    try {
      std::string rows_str = req.get_param_value("rows");
      std::string cols_str = req.get_param_value("cols");

      int rows = std::stoi(rows_str);
      int cols = std::stoi(cols_str);

      if (rows <= 0 || cols <= 0 || rows > 50 || cols > 50) {
        json error_response = {
            {"error", "Размеры должны быть в диапазоне от 1 до 50."}};
        res.status = 400;
        res.set_content(error_response.dump(), "application/json");
        return;
      }

      game.get()->createDungeons(DungeonType::MAZE, rows, cols);

      auto maze = std::get<MazeInfo>(game.get()->getGameInfo());
      json success_response;
      to_json(success_response, maze);

      res.status = 200;
      res.set_content(success_response.dump(), "application/json");

    } catch (const std::invalid_argument& e) {
      json error_response = {
          {"error", "Параметры 'rows' и 'cols' должны быть целыми числами."}};
      res.status = 400;
      res.set_content(error_response.dump(), "application/json");
    } catch (const std::out_of_range& e) {
      json error_response = {
          {"error", "Значения 'rows' или 'cols' слишком большие."}};
      res.status = 400;
      res.set_content(error_response.dump(), "application/json");
    }
  });

  svr.Post("/api/maze/solve",
           [&](const httplib::Request& req, httplib::Response& res) {
             try {
               if (req.body.size() > kMaxPayloadBytes) {
                 json error_response = {{"error", "Запрос слишком большой."}};
                 res.status = 413;
                 res.set_content(error_response.dump(), "application/json");
                 return;
               }
               json body = json::parse(req.body);

               std::pair<int, int> start;
               from_json(body.at("start"), start);
               std::pair<int, int> end;
               from_json(body.at("end"), end);

               // Validate bounds against current maze (avoid invalid indices).
               auto maze = std::get<MazeInfo>(game.get()->getGameInfo());
               if (!is_in_bounds(maze, start) || !is_in_bounds(maze, end)) {
                 json error_response = {{"error", "Точки вне границ лабиринта."}};
                 res.status = 400;
                 res.set_content(error_response.dump(), "application/json");
                 return;
               }

               auto path = game.get()->getPath(start, end);

               if (path.size() > 0) {
                 json success_response = path;
                 res.status = 200;
                 res.set_content(success_response.dump(), "application/json");
               } else {
                 json error_response = {{"error", "Путь не найден."}};
                 res.status = 404;  // Not Found
                 res.set_content(error_response.dump(), "application/json");
               }

             } catch (const json::exception& e) {
               json error_response = {{"error", "Неверный формат запроса: " +
                                                    std::string(e.what())}};
               res.status = 400;
               res.set_content(error_response.dump(), "application/json");
             } catch (const std::exception& e) {
               json error_response = {
                   {"error", "Внутренняя ошибка обработки запроса."}};
               res.status = 500;
               res.set_content(error_response.dump(), "application/json");
             }
           });

  // Safer default: bind only on localhost. Override via env if needed.
  const std::string host = get_env_or("MAZE_SERVER_HOST", "127.0.0.1");
  const int port = get_env_int_or("MAZE_SERVER_PORT", 8080);
  std::cout << "Starting C++ web server on http://" << host << ":" << port
            << std::endl;
  if (!svr.listen(host.c_str(), port)) {
    std::cerr << "Failed to start server on port " << port
              << ". Возможно, порт уже занят." << std::endl;
  }
}