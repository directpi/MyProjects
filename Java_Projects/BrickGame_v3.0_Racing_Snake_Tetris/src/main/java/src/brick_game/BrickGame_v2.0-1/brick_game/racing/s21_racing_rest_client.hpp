/**
 * @file: s21_racing_rest_client.hpp
 * @description: C++ REST клиент для Racing игры
 * @dependencies: common_file.h, defines.h
 * @created: 2025-10-07
 */

#ifndef S21_RACING_REST_CLIENT_HPP
#define S21_RACING_REST_CLIENT_HPP

#include <curl/curl.h>

#include <atomic>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

#include "../common/common_file.h"
#include "../common/defines.h"

namespace s21 {

class RacingRestClient {
 private:
  std::string baseUrl;
  int gameId;
  bool gameStarted;
  std::thread startThread;
  std::atomic<bool> startInFlight{false};
  std::atomic<bool> shouldStop{false};

  // Вспомогательные методы
  std::string makeRequest(const std::string& endpoint,
                          const std::string& method = "GET",
                          const std::string& data = "");
  std::string parseJsonField(const std::string& json, const std::string& field);
  void convertJsonToGameInfo(const std::string& json, GameInfo_t* gameInfo);
  void convertJsonFieldToArray(const std::string& json,
                               const std::string& field,
                               bool fieldArray[HEIGHT][WIDTH]);
  void convertJsonColorsToArray(const std::string& json,
                                const std::string& field,
                                int colorsArray[HEIGHT][WIDTH]);
  int discoverRacingGameId();

 public:
  RacingRestClient(const std::string& url = "http://localhost:8080/api");
  ~RacingRestClient();

  // Основные методы
  bool startGame();
  bool ensureStarted(int retries = 3, int sleepMs = 150);
  void resetGame();
  void userInput(UserAction_t action, bool hold);
  GameInfo_t getGameInfo();
  bool isGameRunning();
  void setPaused(bool pause);
  void stopGame();

  // Получить состояние и поле в локальный буфер
  bool getState(bool outField[HEIGHT][WIDTH], GameInfo_t* meta);
};

}  // namespace s21

#endif  // S21_RACING_REST_CLIENT_HPP
