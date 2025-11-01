/**
 * @file: s21_racing_rest_client.cpp
 * @description: Упрощенный C++ REST клиент для Racing игры (заглушка)
 * @dependencies: s21_racing_rest_client.hpp
 * @created: 2025-10-07
 */

#include "s21_racing_rest_client.hpp"

#include <chrono>
#include <cstring>
#include <iostream>
#include <regex>
#include <thread>

namespace s21 {

RacingRestClient::RacingRestClient(const std::string& url)
    : baseUrl(url), gameId(1), gameStarted(false) {
  const char* envUrl = std::getenv("BRICKGAME_API_URL");
  if (envUrl && *envUrl) {
    baseUrl = std::string(envUrl);
  } else if (baseUrl.empty()) {
    baseUrl = "http://127.0.0.1:8080/api";
  }
  curl_global_init(CURL_GLOBAL_DEFAULT);
}

RacingRestClient::~RacingRestClient() {
  // Останавливаем поток при уничтожении объекта
  shouldStop = true;
  if (startThread.joinable()) {
    startThread.join();
  }
  curl_global_cleanup();
}

bool RacingRestClient::startGame() {
  // Проверяем, не нужно ли остановиться
  if (shouldStop) {
    return false;
  }

  // Используем заранее установленный gameId (discover/по умолчанию 1)
  if (gameId <= 0) gameId = 1;

  // Запускаем игру на сервере синхронно
  std::string endpoint = "/games/" + std::to_string(gameId);
  std::string resp = makeRequest(endpoint, "POST");

  if (resp.empty() || resp.find("error") != std::string::npos) {
    return false;
  }

  gameStarted = true;
  return true;
}

bool RacingRestClient::ensureStarted(int retries, int sleepMs) {
  (void)retries;
  (void)sleepMs;
  if (gameStarted) return true;

  // Если был предыдущий поток запуска — корректно дожидаемся
  if (startThread.joinable()) {
    shouldStop = true;
    startThread.join();
  }

  // Не блокируем UI: считаем игру запущенной и стартуем в фоне
  shouldStop = false;
  gameStarted = true;
  startThread = std::thread([this] { this->startGame(); });
  return true;
}

void RacingRestClient::resetGame() {
  if (!gameStarted) {
    return;
  }

  gameStarted = false;
}

void RacingRestClient::stopGame() {
  // Уведомим сервер и просто сбросим флаг
  userInput(Terminate, false);
  shouldStop = true;
  gameStarted = false;
  if (startThread.joinable()) {
    startThread.join();
  }
}

void RacingRestClient::userInput(UserAction_t action, bool hold) {
  // Разрешаем Terminate даже если игра не запущена локально
  if (!gameStarted && action != Terminate) return;
  int actionId = 0;
  switch (action) {
    case Pause:
      actionId = 1;
      break;
    case Terminate:
      actionId = 2;  // terminate/exit maps to library id 2
      break;
    case Down:
      actionId = 6;
      break;
    case Left:
      actionId = 3;
      break;
    case Right:
      actionId = 4;
      break;
    case Up:
      actionId = 5;
      break;  // accelerate
    case Action:
      actionId = 7;
      break;  // start/restart
    default:
      actionId = 0;
      break;
  }
  std::ostringstream body;
  body << "{\"action_id\":" << actionId
       << ",\"hold\":" << (hold ? "true" : "false") << "}";
  makeRequest("/actions", "POST", body.str());
}

GameInfo_t RacingRestClient::getGameInfo() {
  GameInfo_t gameInfo = {};
  if (!gameStarted) return gameInfo;
  std::string resp = makeRequest("/state", "GET");
  if (resp.empty()) return gameInfo;
  convertJsonToGameInfo(resp, &gameInfo);
  return gameInfo;
}

bool RacingRestClient::isGameRunning() { return gameStarted; }

void RacingRestClient::setPaused(bool pause) {
  if (!gameStarted) return;
  std::ostringstream body;
  body << "{\"action_id\":1,\"hold\":" << (pause ? "true" : "false") << "}";
  makeRequest("/actions", "POST", body.str());
}

std::string RacingRestClient::makeRequest(const std::string& endpoint,
                                          const std::string& method,
                                          const std::string& data) {
  std::string url = baseUrl + endpoint;
  std::string response;

  // Отладочная информация убрана

  CURL* curl = curl_easy_init();
  if (!curl) {
    return "";
  }
  const char* envUser = std::getenv("BRICKGAME_API_USER");
  const char* envPass = std::getenv("BRICKGAME_API_PASS");
  std::string userpwd;
  if (envUser && envPass) {
    userpwd = std::string(envUser) + ":" + std::string(envPass);
  }
  // Увеличиваем таймауты для стабильной работы
  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  if (!userpwd.empty()) {
    curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
    curl_easy_setopt(curl, CURLOPT_USERPWD, userpwd.c_str());
  }
  curl_easy_setopt(
      curl, CURLOPT_WRITEFUNCTION,
      +[](void* contents, size_t size, size_t nmemb, void* userp) -> size_t {
        size_t realsize = size * nmemb;
        std::string* out = static_cast<std::string*>(userp);
        out->append(static_cast<char*>(contents), realsize);
        return realsize;
      });
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
  curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 300L);         // was 5000
  curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, 120L);  // was 2000
  curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);     // avoid signal stalls
  curl_easy_setopt(curl, CURLOPT_TCP_NODELAY, 1L);  // send immediately
  struct curl_slist* headers = nullptr;
  if (method == "POST") {
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "Expect:");  // disable 100-continue
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    if (!data.empty()) {
      curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
      curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.size());
    }
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
  } else {
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
  }
  CURLcode res = curl_easy_perform(curl);
  long httpCode = 0;
  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
  if (headers) curl_slist_free_all(headers);
  curl_easy_cleanup(curl);

  // Отладочная информация
  // Отладочная информация убрана

  if (res != CURLE_OK) {
    return "";
  }

  // Если тело пустое, но пришёл успешный код, считаем запрос успешным
  if (response.empty() && httpCode >= 200 && httpCode < 300) {
    return "OK";
  }
  return response;
}

int RacingRestClient::discoverRacingGameId() {
  // GET /games и найдём объект с name == "Racing"
  std::string resp = makeRequest("/games", "GET");
  if (resp.empty()) return -1;
  // очень простой поиск по имени
  std::regex re(R"(\{[^}]*"id"\s*:\s*(\d+)[^}]*"name"\s*:\s*"Racing"[^}]*\})");
  std::smatch m;
  if (std::regex_search(resp, m, re)) {
    try {
      return std::stoi(m[1]);
    } catch (...) {
      return -1;
    }
  }
  return -1;
}

std::string RacingRestClient::parseJsonField(const std::string& json,
                                             const std::string& field) {
  (void)json;   // Подавляем предупреждение
  (void)field;  // Подавляем предупреждение
  // Заглушка - возвращаем пустую строку
  return "";
}

void RacingRestClient::convertJsonToGameInfo(const std::string& json,
                                             GameInfo_t* gameInfo) {
  // очень простой парсинг числовых полей по regex (без внешних зависимостей)
  auto findInt = [&](const char* key) -> int {
    std::regex re(std::string("\"") + key + "\"\\s*:\\s*(-?\\d+)");
    std::smatch m;
    if (std::regex_search(json, m, re)) return std::stoi(m[1]);
    return 0;
  };
  auto findBool = [&](const char* key) -> bool {
    std::regex re(std::string("\"") + key + "\"\\s*:\\s*(true|false)");
    std::smatch m;
    if (std::regex_search(json, m, re)) return m[1] == "true";
    return false;
  };

  gameInfo->score = findInt("score");
  int highScore = findInt("highScore");
  if (highScore == 0) highScore = findInt("high_score");
  gameInfo->high_score = highScore;
  gameInfo->level = findInt("level");
  gameInfo->speed = findInt("speed");

  // Правильная обработка состояния паузы
  bool paused = findBool("pause");
  bool running = findBool("game_running");

  if (!running) {
    gameInfo->pause = GAMEOVER_STATE;
  } else if (paused) {
    gameInfo->pause = PAUSE_STATE;
  } else {
    gameInfo->pause = 0;  // игра активна
  }
}

void RacingRestClient::convertJsonFieldToArray(const std::string& json,
                                               const std::string& field,
                                               bool fieldArray[HEIGHT][WIDTH]) {
  // Инициализируем массив нулями
  for (int y = 0; y < HEIGHT; ++y) {
    for (int x = 0; x < WIDTH; ++x) {
      fieldArray[y][x] = false;
    }
  }

  // Ищем поле в JSON
  std::string fieldPattern = "\"" + field + "\"\\s*:\\s*\\[";
  std::regex fieldRegex(fieldPattern);
  std::smatch fieldMatch;

  if (!std::regex_search(json, fieldMatch, fieldRegex)) {
    // Поле не найдено в JSON
    return;  // поле не найдено
  }

  size_t startPos = fieldMatch.position() + fieldMatch.length();
  int y = 0, x = 0;
  bool inRow = false;
  bool inValue = false;
  std::string currentValue = "";

  for (size_t i = startPos; i < json.size() && y < HEIGHT; ++i) {
    char c = json[i];

    if (c == '[' && !inRow) {
      // Начало новой строки
      inRow = true;
      x = 0;
    } else if (c == ']' && inRow) {
      // Конец строки
      inRow = false;
      y++;
      if (y >= HEIGHT) break;
    } else if (c == 't' && inRow && !inValue) {
      // Начало "true"
      inValue = true;
      currentValue = "t";
    } else if (c == 'f' && inRow && !inValue) {
      // Начало "false"
      inValue = true;
      currentValue = "f";
    } else if (inValue) {
      currentValue += c;
      if (currentValue == "true" || currentValue == "false") {
        // Завершили чтение значения
        if (x < WIDTH) {
          fieldArray[y][x] = (currentValue == "true");
        }
        x++;
        inValue = false;
        currentValue = "";
      }
    } else if (c == ',' && inRow && !inValue) {
      // Пропускаем запятые между значениями
    }
  }
}

void RacingRestClient::convertJsonColorsToArray(
    const std::string& json, const std::string& field,
    int colorsArray[HEIGHT][WIDTH]) {
  (void)json;
  (void)field;
  (void)colorsArray;
  // Не используется для Racing в терминале
}

bool RacingRestClient::getState(bool outField[HEIGHT][WIDTH],
                                GameInfo_t* meta) {
  if (!meta) return false;
  if (!gameStarted) return false;

  // Получаем состояние с сервера
  std::string resp = makeRequest("/state", "GET");

  if (resp.empty()) {
    // Если сервер недоступен, возвращаем тестовые данные
    for (int y = 0; y < HEIGHT; ++y)
      for (int x = 0; x < WIDTH; ++x) outField[y][x] = false;

    meta->score = 0;
    meta->high_score = 0;
    meta->level = 1;
    meta->speed = 1;
    meta->pause = 0;
    return true;
  }

  // Парсим мета-информацию
  convertJsonToGameInfo(resp, meta);

  // Если игра в состоянии GAMEOVER_STATE, оставляем последнее локальное поле
  if (meta->pause != GAMEOVER_STATE) {
    // Парсим поле только когда игра активна/на паузе
    convertJsonFieldToArray(resp, "field", outField);
  }

  return true;
}

}  // namespace s21