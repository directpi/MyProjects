#ifndef SNAKE_MODEL_HPP_
#define SNAKE_MODEL_HPP_

#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <deque>
#include <iostream>
#include <thread>
#include <unordered_set>
#include <utility>

#include "../../common/common_file.h"
#include "../../common/defines.h"

namespace snake {

class Game {
 private:
  // Поля класса
  GameInfo_t gameInfo;
  UserAction_t user_action;
  GameState_t currentState;

  // Приватный конструктор
  Game() {
    initField();
    gameInfo.score = 0;
    gameInfo.high_score = getMaxScore();
    gameInfo.level = 1;
    gameInfo.speed = 500;
    gameInfo.pause = 0;
    setCurrentState(START_STATE);
    setUserAction(Start);
  }

  // Запрещаем копирование и присваивание
  Game(const Game &) = delete;
  Game &operator=(const Game &) = delete;

 public:
  ~Game();

  // Метод для получения экземпляра
  static Game &getInstance() {
    static Game instance;
    return instance;
  }

  // Методы класса
  int **getField() const { return gameInfo.field; }
  void setField(int **newField) { gameInfo.field = newField; }

  size_t getScore() { return gameInfo.score; }
  void plusScore() { ++gameInfo.score; }
  void setScoreToZero() { gameInfo.score = 0; }
  size_t getMaxScore();
  void setMaxScore(size_t new_score);

  int getLevel() const { return gameInfo.level; }
  void setLevel(int newLevel) { gameInfo.level = newLevel; }
  static void increaseSpeedLevel();
  static void setLevelToOne();

  int getSpeed() const { return gameInfo.speed; }
  void setSpeed(int newSpeed) { gameInfo.speed = newSpeed; }

  bool isPaused() const { return gameInfo.pause; }
  void setPause(int pause) { gameInfo.pause = pause; }

  void initField();
  void game_field_mem_free(GameInfo_t *info);

  GameInfo_t *getGameInfo() { return &gameInfo; }
  void setGameInfo();

  GameState_t getCurrentState() { return currentState; }
  void setCurrentState(GameState_t state) { currentState = state; }

  UserAction_t getUserAction() { return user_action; }
  void setUserAction(UserAction_t action) { user_action = action; }

  void reset() {
    game_field_mem_free(&gameInfo);
    gameInfo.score = 0;
    gameInfo.high_score = 0;
    gameInfo.level = 1;
    gameInfo.speed = GAME_SPEED;
    gameInfo.pause = 0;
  }
};

class Snake {
 private:
  // Приватный конструктор
  Snake() {}

  // Запрещаем копирование и присваивание
  Snake(const Snake &) = delete;
  Snake &operator=(const Snake &) = delete;

 public:
  ~Snake() = default;

  // Метод для получения экземпляра
  static Snake &getInstance() {
    static Snake instance;
    return instance;
  }

  // Поля и методы класса
  std::deque<std::pair<int, int>> snakeBody = {};
  UserAction_t lastTurn = Down;
  std::pair<int, int> snake_direction = {-1, 0};
  bool justAteApple = false;

  void createSnake();
  void moveSnake();

  static std::pair<int, int> getSnakeDirection();
  static void setSnakeDirection(const std::pair<int, int> &direction);
};

struct pair_hash {
  template <class T1, class T2>
  std::size_t operator()(const std::pair<T1, T2> &pair) const {
    return std::hash<T1>()(pair.first) ^
           std::hash<T2>()(pair.second);  //(std::hash<T2>()(pair.second)<<1)
  }
};

class Field {
 private:
  // Приватный конструктор
  Field() {}

  // Запрещаем копирование и присваивание
  Field(const Field &) = delete;
  Field &operator=(const Field &) = delete;

 public:
  ~Field() = default;

  // Метод для получения экземпляра
  static Field &getInstance() {
    static Field instance;
    return instance;
  }

  // Поля и методы класса
  std::pair<int, int> appleXY;
  std::unordered_set<std::pair<int, int>, pair_hash> freeCells;

  void snake_clear_field(int **matrix);
  void placeApple();
  void initializeFreeCells();
  void updateFreeCells(const std::pair<int, int> &cell, bool isOccupied);
};

/* FSM */
GameInfo_t *updateCurrentState();
// FSM таблица: состояния и действия(события)
typedef void (*FSMAction)();
void fsm();

void snake_spawn();
void moving();
void turn();
void snake_game_exit();
void snake_game_over();
void snake_game_win();
void snake_pause();

void setGameOver(bool gameover);
void setGameWon();
bool isPaused();   // Проверка паузы
bool isGameWon();  // Проверка победы
bool isGameOver(); // Проверка окончания игры
void timer_move();
void snake_check_high_score(GameInfo_t *game_info);

typedef struct Direction {
  std::pair<int, int> UpDirection = {-1, 0};  // Движение вверх: уменьшение Y
  std::pair<int, int> DownDirection = {1, 0};  // Движение вниз: увеличение Y
  std::pair<int, int> RightDirection = {0, 1};  // Движение вправо: увеличение X
  std::pair<int, int> LeftDirection = {0, -1};  // Движение влево: уменьшение X
} Direction;

}  // namespace snake
#endif  // SNAKE_MODEL_HPP_