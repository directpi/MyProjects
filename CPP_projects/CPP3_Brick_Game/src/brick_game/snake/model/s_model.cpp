#include "s_model.hpp"

#include <time.h>

#include <algorithm>  // для std::count
#include <cstddef>
#include <ctime>  // для time()
#include <set>
#include <utility>

#include "../../common/common_file.h"

namespace snake {

static auto prevTime = std::chrono::system_clock::now();

void Game::setGameInfo() {
  if (gameInfo.field != nullptr) {
    Field &field = Field::getInstance();
    Snake &snake = Snake::getInstance();

    // Очистка клеток, занятых змейкой
    for (const auto &cell : snake.snakeBody) {
      assert(cell.first >= 0 && cell.first < HEIGHT);
      assert(cell.second >= 0 && cell.second < WIDTH);
      gameInfo.field[cell.first][cell.second] = 0;  // Очищаем клетку змейки
    }

    // Очистка клетки, занимаемой яблоком
    assert(field.appleXY.first >= 0 && field.appleXY.first < HEIGHT);
    assert(field.appleXY.second >= 0 && field.appleXY.second < WIDTH);
    gameInfo.field[field.appleXY.first][field.appleXY.second] =
        0;  // Очищаем клетку с яблоком

    // Проверка на пустое тело змейки
    // Обновление положения змейки на поле
    if (!snake.snakeBody.empty()) {
      for (const auto &segment : snake.snakeBody) {
        gameInfo.field[segment.first][segment.second] = 1;
      }
    }

    // Установка положения яблока, если игра активна
    if (currentState != ATTACHING_STATE && currentState != GAMEOVER_STATE) {
      const auto &apple = field.appleXY;
      if (apple.first < 0 || apple.first >= HEIGHT || apple.second < 0 ||
          apple.second >= WIDTH) {
        throw std::out_of_range("Error: Apple position out of bounds!");
      }
      gameInfo.field[apple.first][apple.second] = 2;
    }
  }
}

std::pair<int, int> Snake::getSnakeDirection() {
  Snake &snake = Snake::getInstance();
  return snake.snake_direction;
}

void turn() {
  Direction direction;
  Snake &snake = Snake::getInstance();

  // Если движемся вверх
  if (snake.snake_direction == direction.UpDirection) {
    if (snake.lastTurn != UserAction_t::Down) {
      if (snake.lastTurn == UserAction_t::Left) {
        snake.snake_direction = direction.LeftDirection;  // Поворот направо
      } else if (snake.lastTurn == UserAction_t::Right) {
        snake.snake_direction = direction.RightDirection;  // Поворот налево
      }
    }
  }
  // Если движемся вниз
  else if (snake.snake_direction == direction.DownDirection) {
    if (snake.lastTurn != UserAction_t::Up) {
      if (snake.lastTurn == UserAction_t::Left) {
        snake.snake_direction = direction.LeftDirection;  // Поворот направо
      } else if (snake.lastTurn == UserAction_t::Right) {
        snake.snake_direction = direction.RightDirection;  // Поворот налево
      }
    }
  }
  // Если движемся влево
  else if (snake.snake_direction == direction.LeftDirection) {
    if (snake.lastTurn != UserAction_t::Right) {
      if (snake.lastTurn == UserAction_t::Up) {
        snake.snake_direction = direction.UpDirection;  // Поворот направо
      } else if (snake.lastTurn == UserAction_t::Down) {
        snake.snake_direction = direction.DownDirection;  // Поворот налево
      }
    }
  }
  // Если движемся вправо
  else if (snake.snake_direction == direction.RightDirection) {
    if (snake.lastTurn != UserAction_t::Left) {
      if (snake.lastTurn == UserAction_t::Up) {
        snake.snake_direction = direction.UpDirection;  // Поворот налево
      } else if (snake.lastTurn == UserAction_t::Down) {
        snake.snake_direction = direction.DownDirection;  // Поворот направо
      }
    }
  }
}

void Snake::setSnakeDirection(const std::pair<int, int> &direction) {
  Snake &snake = Snake::getInstance();
  // Запрещаем движение в обратную сторону
  if (direction.first != -snake.snake_direction.first ||
      direction.second != -snake.snake_direction.second) {
    snake.snake_direction = direction;
  }
}

void Snake::createSnake() {
  Game &game = Game::getInstance();
  Field &field = Field::getInstance();
  Snake &snake = Snake::getInstance();
  for (int i = 0; i < SNAKE_LEN; i++) {
    // Добавление сегмента
    snake.snakeBody.push_back({HEIGHT / 2 + i, WIDTH / 2});
    // Запись в поле (gameInfo.field) на позиции сегмента змейки
    game.getField()[snake.snakeBody[i].first][snake.snakeBody[i].second] = 1;
    // Обновление списка свободных клеток
    field.updateFreeCells({HEIGHT / 2 + i, WIDTH / 2}, true);
  }
}

void snake_pause(void) {
  Game &game = Game::getInstance();
  if (isPaused() && !isGameOver()) {
    game.setPause((int)START_STATE);
    game.setCurrentState(MOVING_STATE);
  } else if (!isPaused() && !isGameOver()) {
    game.setPause((int)PAUSE_STATE);
    game.setCurrentState(PAUSE_STATE);
  } else if (!isPaused() && isGameOver()) {
    setGameOver(false);
  }
}

void setGameOver(bool gameover) {
  Game &game = Game::getInstance();
  Field &field = Field::getInstance();
  if (gameover && isGameWon()) {
    setGameWon();
  } else if (gameover && !isGameWon()) {
    // field.snake_clear_field(game.getGameInfo()->field);
    game.setPause((int)GAMEOVER_STATE);
    game.setCurrentState(GAMEOVER_STATE);
    game.setUserAction(Terminate);
  } else {
    field.snake_clear_field(game.getGameInfo()->field);
    field.updateFreeCells({field.appleXY.first, field.appleXY.second}, false);
    game.setCurrentState(START_STATE);
    game.setUserAction(Start);
    game.setScoreToZero();
    game.setLevelToOne();
    game.setSpeed(GAME_SPEED);
    game.setPause((int)START_STATE);
  }
  Snake &snake = Snake::getInstance();
  snake.snakeBody.clear();
  snake.snake_direction = {-1, 0};
}

void Snake::moveSnake() {
  Game &game = Game::getInstance();
  Field &field = Field::getInstance();
  int newHeadY = snakeBody.front().first + snake_direction.first;
  int newHeadX = snakeBody.front().second + snake_direction.second;
  if (newHeadY < 0 || newHeadY >= HEIGHT || newHeadX < 0 || newHeadX >= WIDTH) {
    game.setCurrentState(ATTACHING_STATE);
  } else if (!snakeBody.empty() && game.getCurrentState() != ATTACHING_STATE &&
             game.getCurrentState() != PAUSE_STATE &&
             game.getCurrentState() != GAMEOVER_STATE) {
    snakeBody.push_front({newHeadY, newHeadX});
    field.updateFreeCells({newHeadY, newHeadX}, true);
    // Проверяем на столкновение с самим собой
    auto head = snakeBody.front();
    if (snakeBody.size() > 1) {
      for (size_t i = 1; i < snakeBody.size(); ++i) {
        if (snakeBody[i] == head) {
          game.setCurrentState(ATTACHING_STATE);
          i = snakeBody.size();
        }
      }
    }
    if (!snakeBody.empty() && game.getCurrentState() != ATTACHING_STATE &&
        game.getCurrentState() != PAUSE_STATE &&
        game.getCurrentState() != GAMEOVER_STATE) {
      if (game.getField()[snakeBody.front().first][snakeBody.front().second] ==
          2) {
        justAteApple = true;  // Устанавливаем флаг, что яблоко было съедено
        game.plusScore();
        if (game.getScore() == 200) setGameWon();
        game.increaseSpeedLevel();
        game.getField()[snakeBody.front().first][snakeBody.front().second] = 1;
        field.updateFreeCells(
            {snakeBody.front().first, snakeBody.front().second}, true);
        field.placeApple();
      }
      if (!justAteApple) {
        auto tail = snakeBody.back();
        if (tail.first >= 0 && tail.first < HEIGHT && tail.second >= 0 &&
            tail.second < WIDTH) {
          game.getField()[tail.first][tail.second] = 0;
          field.updateFreeCells({tail.first, tail.second}, false);
        }
        snakeBody.pop_back();
      } else {
        justAteApple = false;  // После съедения яблока сбрасываем флаг
      }
    }
  }
}

GameInfo_t *updateCurrentState() {
  Game &game = Game::getInstance();

  if (game.getField() == NULL) {
    game.initField();
  }
  timer_move();
  fsm();
  game.setGameInfo();
  return game.getGameInfo();
}

void fsm() {
  FSMAction snake_fsm_table[9][8] = {
      //{Start, Pause, Terminate, Left, Right, Up, Down, Action}
      {snake_spawn, nullptr, snake_game_exit, nullptr, nullptr, nullptr,
       nullptr, nullptr},  // Start
      {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
       nullptr},  // Spawn
      {nullptr, snake_pause, snake_game_exit, turn, turn, turn, turn,
       moving},  // Moving
      {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
       nullptr},  // Shifting
      {snake_game_over, snake_game_over, snake_game_over, snake_game_over,
       snake_game_over, snake_game_over, snake_game_over,
       snake_game_over},  // Attaching
      {nullptr, nullptr, snake_game_over, nullptr, nullptr, nullptr, nullptr,
       nullptr},  // GameOver
      {nullptr, snake_pause, snake_game_exit, nullptr, nullptr, nullptr,
       nullptr, nullptr},  // Pause
      {snake_game_exit, snake_game_exit, snake_game_exit, snake_game_exit,
       snake_game_exit, snake_game_exit, snake_game_exit,
       snake_game_exit},  // Exit
      {nullptr, nullptr, nullptr, nullptr, snake_game_win, snake_game_win,
       snake_game_win, snake_game_win}  // WIN

  };
  Game &game = Game::getInstance();
  FSMAction act = snake_fsm_table[game.getCurrentState()][game.getUserAction()];
  if (act) act();
}

void snake_spawn() {
  Game &game = Game::getInstance();
  Snake &snake = Snake::getInstance();
  Field &field = Field::getInstance();
  field.snake_clear_field(game.getGameInfo()->field);
  field.updateFreeCells({field.appleXY.first, field.appleXY.second}, false);
  snake.createSnake();
  game.setCurrentState(MOVING_STATE);
  field.placeApple();
}

bool isPaused() {
  Game &game = Game::getInstance();
  return game.getCurrentState() == GameState_t::PAUSE_STATE;
}

bool isGameOver() {
  Game &game = Game::getInstance();
  return game.getCurrentState() == GameState_t::GAMEOVER_STATE;
}

bool isGameWon() {
  Game &game = Game::getInstance();
  return game.getCurrentState() == GameState_t::WIN_STATE;
}

void setGameWon() {
  Game &game = Game::getInstance();
  game.setCurrentState(WIN_STATE);
  game.getGameInfo()->pause = (int)WIN_STATE;
}

void moving() {
  Snake &snake = Snake::getInstance();
  snake.moveSnake();
}

void timer_move() {
  Game &game = Game::getInstance();
  if (game.getCurrentState() == MOVING_STATE) {
    auto currentTime = std::chrono::system_clock::now();
    size_t moveSpeed = game.getSpeed();  // Получаем текущую скорость
    if (currentTime > prevTime + std::chrono::milliseconds(moveSpeed)) {
      moving();
      prevTime = currentTime;
    }
  }
}

void snake_game_exit() {
  Game &game = Game::getInstance();
  game.setPause((int)EXIT_STATE);
  game.setCurrentState(EXIT_STATE);
  game.setUserAction(Start);
}
void snake_game_over() { setGameOver(true); }

void snake_game_win() { setGameWon(); }

void Field::snake_clear_field(int **matrix) {
  if (!matrix) {
    std::cerr << "Ошибка: указатель на матрицу равен nullptr" << std::endl;
    return;
  }
  for (int i = 0; i < ROWS_; i++) {
    if (!matrix[i]) {
      std::cerr << "Ошибка: строка матрицы равна nullptr" << std::endl;
      return;
    }
    std::fill(matrix[i], matrix[i] + COLS_, 0);
  }
}

void Game::game_field_mem_free(GameInfo_t *info) {
  if (!info) {
    std::cerr << "Ошибка: указатель на GameInfo_t равен nullptr" << std::endl;
    return;
  }
  // Освобождаем память для info->field
  if (info->field) {
    for (int i = 0; i < HEIGHT; ++i) {
      if (info->field[i]) {
        delete[] info->field[i];
        info->field[i] = nullptr;
      }
    }
    delete[] info->field;
    info->field = nullptr;
  }
}

Game::~Game() { game_field_mem_free(&gameInfo); }

void Game::initField() {
  try {
    // Выделение памяти для поля игры
    gameInfo.field = new int *[HEIGHT] {};
    for (int i = 0; i < HEIGHT; ++i) {
      gameInfo.field[i] = new int[WIDTH]{};  // Инициализация нулями
    }
  } catch (const std::exception &e) {
    // Освобождение уже выделенной памяти
    if (gameInfo.field) {
      for (int i = 0; i < HEIGHT; ++i) {
        if (gameInfo.field[i]) {       // Проверка на nullptr
          delete[] gameInfo.field[i];  // Освобождение строки
        }
      }
      delete[] gameInfo.field;  // Освобождение массива строк
      gameInfo.field = nullptr;
    }
    std::cerr << "Ошибка при инициализации Snake: " << e.what() << std::endl;
    throw;  // Повторное пробрасывание исключения
  }
  Field &field = Field::getInstance();
  field.initializeFreeCells();
}

void Field::placeApple() {
  Game &game = Game::getInstance();
  // Проверяем, что поле инициализировано
  if (!game.getField()) {
    throw std::runtime_error("Game field is not initialized! ");
  }
  if (freeCells.empty()) {
    throw std::runtime_error("No free cells available to place an apple!");
  }

  // Случайный выбор из доступных клеток
  std::random_device rd;
  std::mt19937 gen(rd());  // Генератор случайных чисел
  std::uniform_int_distribution<> dist(0,
                                       static_cast<int>(freeCells.size()) - 1);

  int randomIndex = dist(gen);  // Получение случайного индекса

  // Получаем итератор на случайный элемент
  auto it = freeCells.begin();
  std::advance(it, randomIndex);  // Быстро перемещаем итератор на нужный индекс

  appleXY = *it;  // Разыменовываем итератор для получения координат

  // Проверяем корректность координат
  if (appleXY.first < 0 || appleXY.first >= HEIGHT || appleXY.second < 0 ||
      appleXY.second >= WIDTH) {
    throw std::out_of_range("Apple coordinates are out of bounds!");
  }
  // Устанавливаем яблоко на поле
  game.getField()[appleXY.first][appleXY.second] = 2;

  // Обновляем свободные клетки
  updateFreeCells(appleXY, true);
}

void Field::initializeFreeCells() {
  Snake &snake = Snake::getInstance();
  freeCells.clear();
  for (int y = 0; y < HEIGHT; ++y) {
    for (int x = 0; x < WIDTH; ++x) {
      freeCells.insert({y, x});
    }
  }
  // Удаляем клетки, занятые змейкой
  for (const auto &segment : snake.snakeBody) {
    assert(segment.first >= 0 && segment.first < HEIGHT);
    assert(segment.second >= 0 && segment.second < WIDTH);
    updateFreeCells(segment, true);
  }
}

void Field::updateFreeCells(const std::pair<int, int> &cell, bool isOccupied) {
  if (cell.first < 0 || cell.first > HEIGHT || cell.second < 0 ||
      cell.second >= WIDTH) {
    std::cerr << "Error: Ячейка за пределами координат: (" << cell.first << ", "
              << cell.second << ")" << std::endl;
    return;
  }
  if (isOccupied) {
    // Убираем клетку из списка свободных
    freeCells.erase(cell);
  } else {
    // Добавляем клетку в список свободных
    freeCells.insert(cell);
  }
}

size_t Game::getMaxScore() {
  // if (!gameInfo.high_score)
  int high_score = read_file_high_score(2);
  return high_score;
}

void Game::setMaxScore(size_t new_score) { gameInfo.high_score = new_score; }

void Game::increaseSpeedLevel() {  // Увеличение скорости и уровня
  Game &game = Game::getInstance();

  if (game.getScore() != 0 && game.getScore() % 5 == 0 &&
      game.getLevel() < MAX_LEVEL) {
    game.setLevel(game.getLevel() + 1);
    game.setSpeed(std::max<size_t>(
        MIN_SPEED, game.getSpeed() - game.getLevel() - SPEED_INCREMENT));
  }
}

void Game::setLevelToOne() {
  Game &game = Game::getInstance();
  game.setLevel(1);
  game.setSpeed(GAME_SPEED);
}
void snake_check_high_score(GameInfo_t *game_info) {
  int high_score = read_file_high_score(2);
  if (high_score != -1) {
    if (game_info->score > high_score) {
      write_to_file_high_score(2, game_info->score);
      game_info->high_score = game_info->score;
    }
  }
}
}  // namespace snake
