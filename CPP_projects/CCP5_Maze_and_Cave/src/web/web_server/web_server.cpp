#include "web_server.h"

#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include "../../include/dungeon/core/Maze.h"
#include "../../include/dungeon/io/MazeLoader.h"
#include "../../include/dungeon/pathfinding/MazeBfsPathfinder.h"
#include "libs/httplib.h"
#include "libs/json.hpp"

using json = nlohmann::json;

WebServer::WebServer() : game(Game::create()) {}

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

  auto cors_handler = [&](const httplib::Request& /*req*/,
                          httplib::Response& res) {
    res.set_header("Access-Control-Allow-Origin", "http://localhost:3000");
    res.set_header("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
    res.set_header("Access-Control-Allow-Headers", "Content-Type");
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

    const std::string temp_filename = "temp_maze_upload.txt";
    std::ofstream temp_file(temp_filename);
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

    std::remove(temp_filename.c_str());

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
               json body = json::parse(req.body);

               std::pair<int, int> start;
               from_json(body.at("start"), start);
               std::pair<int, int> end;
               from_json(body.at("end"), end);

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
             }
           });

  const std::string host = "0.0.0.0";
  const int port = 8080;
  std::cout << "Starting C++ web server on http://" << host << ":" << port
            << std::endl;
  if (!svr.listen(host.c_str(), port)) {
    std::cerr << "Failed to start server on port " << port
              << ". Возможно, порт уже занят." << std::endl;
  }
}