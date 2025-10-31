#include "tests_run.hpp"

TEST(Suite_Game_class, plusScore) {
  Game &game = Game::getInstance();
  for (int i = 0; i < 5; ++i) game.plusScore();
  EXPECT_EQ(game.getScore(), 5U);
  game.reset();
}

TEST(Suite_Game_class, getScore) {
  Game &game = Game::getInstance();
  size_t score = game.getScore();
  EXPECT_EQ(game.getScore(), score);
  game.reset();
}

TEST(Suite_Game_class, getMaxScore) {
  Game &game = Game::getInstance();
  size_t maxScore = game.getMaxScore();
  EXPECT_EQ(game.getMaxScore(), maxScore);
  game.reset();
}
TEST(Suite_Game_class, setMaxScore) {
  Game &game = Game::getInstance();
  game.setMaxScore(1000);
  game.setScoreToZero();
  game.getGameInfo()->score = game.getGameInfo()->high_score;
  snake_check_high_score(game.getGameInfo());
  EXPECT_EQ(game.getMaxScore(), 1000);
  game.reset();
}

TEST(Suite_Game_class, getLevel) {
  Game &game = Game::getInstance();
  EXPECT_EQ(game.getLevel(), 1U);
  game.reset();
}

TEST(Suite_Game_class, setLevel0) {
  Game &game = Game::getInstance();
  game.setLevelToOne();
  EXPECT_EQ(game.getLevel(), 1U);
  game.reset();
}

TEST(Suite_Game_class, setLevelUp) {
  Game &game = Game::getInstance();
  for (int i = 0; i < 5; ++i) game.plusScore();
  game.increaseSpeedLevel();
  EXPECT_EQ(game.getLevel(), 2U);
  game.reset();
}

TEST(Suite_Game_class, setSpeed) {
  Game &game = Game::getInstance();
  game.setSpeed(600);
  EXPECT_EQ(game.getSpeed(), 600U);
  game.reset();
}

TEST(Suite_Game_class, getSpeed) {
  Game &game = Game::getInstance();
  size_t speed = game.getSpeed();
  EXPECT_EQ(speed, 500U);
  game.reset();
}

TEST(Suite_Game_class, getGameInfo) {
  Game &game = Game::getInstance();
  GameInfo_t *gameInfo2 = game.getGameInfo();

  ASSERT_NE(gameInfo2, nullptr) << "gameInfo2 is a null pointer";
  EXPECT_EQ(game.getGameInfo()->level, gameInfo2->level) << "Level mismatch";
  EXPECT_EQ(game.getGameInfo()->score, gameInfo2->score) << "Score mismatch";
  EXPECT_EQ(game.getGameInfo()->high_score, gameInfo2->high_score)
      << "High score mismatch";
  EXPECT_EQ(game.getGameInfo()->speed, gameInfo2->speed) << "Speed mismatch";
  EXPECT_EQ(game.getGameInfo()->pause, gameInfo2->pause) << "Pause mismatch";
}

TEST(Suite_Game_class, setGameInfo) {
  Game &game = Game::getInstance();
  for (int i = 0; i < 15; ++i) {
    game.plusScore();
    game.increaseSpeedLevel();
  }
  game.setSpeed(550);

  EXPECT_NO_THROW(game.setGameInfo());

  GameInfo_t *gameInfo2 = game.getGameInfo();
  ASSERT_NE(gameInfo2, nullptr) << "gameInfo2 is a null pointer";
  EXPECT_EQ(gameInfo2->level, 4) << "Level mismatch";
  EXPECT_EQ(gameInfo2->score, 15) << "Score mismatch";
  EXPECT_EQ(gameInfo2->speed, 550) << "Speed mismatch";
  EXPECT_EQ(gameInfo2->pause, game.getGameInfo()->pause) << "Pause mismatch";

  game.reset();
}

TEST(Suite_Game_class, getCurrentState) {
  Game &game = Game::getInstance();
  GameState_t state = game.getCurrentState();
  EXPECT_EQ(state, START_STATE);
}

TEST(Suite_Game_class, setCurrentState) {
  Game &game = Game::getInstance();
  game.setCurrentState(SPAWN_STATE);
  GameState_t state = game.getCurrentState();
  EXPECT_EQ(state, SPAWN_STATE);
}

TEST(Suite_Game_class, initField) {
  Game &game = Game::getInstance();
  EXPECT_NO_THROW(game.initField())
      << "Field initialization threw an unexpected exception";

  // Проверяем, что память выделена
  ASSERT_NE(game.getGameInfo()->field, nullptr)
      << "Field pointer is nullptr after initialization";

  // Проверяем, что все значения инициализированы в ноль
  for (int i = 0; i < HEIGHT; ++i) {
    ASSERT_NE(game.getGameInfo()->field[i], nullptr)
        << "Row " << i << " is nullptr after initialization";
    for (int j = 0; j < WIDTH; ++j) {
      EXPECT_EQ(game.getGameInfo()->field[i][j], 0)
          << "Field cell (" << i << "," << j << ") is not zero-initialized";
    }
  }
  game.reset();
}

TEST(Suite_Snake_class, createSnake) {
  Game &game = Game::getInstance();
  Snake &snake = Snake::getInstance();
  game.initField();
  snake.createSnake();
  EXPECT_EQ(snake.snakeBody.size(), 4);
  snake.snakeBody.clear();
  game.reset();
}

TEST(Suite_Snake_class, moveSnake) {
  Game &game = Game::getInstance();
  Snake &snake = Snake::getInstance();
  game.initField();
  snake.createSnake();
  snake.moveSnake();
  EXPECT_EQ(snake.snakeBody.front().first, 9);
  EXPECT_EQ(snake.snakeBody.front().second, 5);
  snake.snakeBody.clear();
  game.reset();
}
TEST(Suite_Snake_class, growSnake) {
  Game &game = Game::getInstance();
  Snake &snake = Snake::getInstance();
  Field &field = Field::getInstance();
  game.initField();
  snake.createSnake();
  field.appleXY = {9, 5};
  game.getGameInfo()->field[field.appleXY.first][field.appleXY.second] = 2;
  snake.moveSnake();
  EXPECT_EQ(snake.snakeBody.size(), 5);
  snake.snakeBody.clear();
  game.reset();
}

TEST(Suite_Snake_class, getSnakeDirection) {
  Snake &snake = Snake::getInstance();
  std::pair<int, int> direction = snake.getSnakeDirection();
  EXPECT_EQ(direction.first, -1);
  EXPECT_EQ(direction.second, 0);
}

TEST(Suite_Snake_class, setSnakeDirection) {
  Snake &snake = Snake::getInstance();
  snake.setSnakeDirection({0, -1});
  std::pair<int, int> direction = snake.getSnakeDirection();
  EXPECT_EQ(direction.first, 0);
  EXPECT_EQ(direction.second, -1);
}

TEST(Suite_Field_class, snake_clear_field) {
  Game &game = Game::getInstance();
  game.initField();
  for (int i = 0; i < 20; ++i) {
    for (int j = 0; j < 10; ++j) {
      game.getGameInfo()->field[i][j] = 1;
    }
  }
  Field &field = Field::getInstance();
  field.snake_clear_field(game.getGameInfo()->field);
  EXPECT_EQ(game.getGameInfo()->field[10][5], 0);
  EXPECT_EQ(game.getGameInfo()->field[0][0], 0);
  EXPECT_EQ(game.getGameInfo()->field[19][9], 0);
  EXPECT_EQ(game.getGameInfo()->field[1][1], 0);
  EXPECT_EQ(game.getGameInfo()->field[10][8], 0);
  game.reset();
}

TEST(Suite_Field_class, placeApple) {
  Game &game = Game::getInstance();
  game.initField();
  Field &field = Field::getInstance();
  field.placeApple();
  EXPECT_EQ(
      game.getGameInfo()->field[field.appleXY.first][field.appleXY.second], 2);
  game.reset();
}

TEST(Suite_Field_class, initializeFreeCells) {
  Game &game = Game::getInstance();
  game.initField();
  Field &field = Field::getInstance();
  field.initializeFreeCells();
  EXPECT_EQ(field.freeCells.size(), 200);
  game.reset();
}

TEST(Suite_Field_class, updateFreeCells) {
  Game &game = Game::getInstance();
  Snake &snake = Snake::getInstance();
  Field &field = Field::getInstance();
  game.initField();
  snake.createSnake();
  EXPECT_EQ(field.freeCells.size(), 196);
  snake.snakeBody.clear();
  game.reset();
}

TEST(Suite_FSM_logic, SPAWN_STATE) {
  Game &game = Game::getInstance();
  Snake &snake = Snake::getInstance();
  Field &field = Field::getInstance();
  game.initField();
  snake_spawn();

  EXPECT_EQ(snake.snakeBody.size(), 4);
  EXPECT_EQ(
      game.getGameInfo()->field[field.appleXY.first][field.appleXY.second], 2);
  GameState_t state = game.getCurrentState();
  EXPECT_EQ(state, MOVING_STATE);
  snake.snakeBody.clear();
  game.reset();
}

TEST(Suite_FSM_logic, MOVING_STATE) {
  Game &game = Game::getInstance();
  Snake &snake = Snake::getInstance();
  Field &field = Field::getInstance();
  game.initField();
  snake_spawn();
  int newHeadY = snake.snakeBody.front().first + snake.snake_direction.first;
  int newHeadX = snake.snakeBody.front().second + snake.snake_direction.second;
  moving();
  EXPECT_EQ(snake.snakeBody.front().first, newHeadY);
  EXPECT_EQ(snake.snakeBody.front().second, newHeadX);
  EXPECT_EQ(
      game.getGameInfo()->field[field.appleXY.first][field.appleXY.second], 2);
  GameState_t state = game.getCurrentState();
  EXPECT_EQ(state, MOVING_STATE);
  snake.snakeBody.clear();
  game.reset();
}
TEST(Suite_FSM_logic, ATTACHING_STATE) {
  Game &game = Game::getInstance();
  Snake &snake = Snake::getInstance();
  game.initField();
  snake_spawn();
  snake.setSnakeDirection({0, 1});
  turn();
  turn();
  turn();
  moving();

  GameState_t state = game.getCurrentState();
  EXPECT_EQ(state, ATTACHING_STATE);
  snake.snakeBody.clear();
  game.reset();
}
TEST(Suite_FSM_logic, GAMEOVER_STATE) {
  Game &game = Game::getInstance();
  Snake &snake = Snake::getInstance();
  game.initField();
  snake_spawn();
  snake.setSnakeDirection({0, 1});
  turn();
  turn();
  turn();
  moving();
  updateCurrentState();
  GameState_t state = game.getCurrentState();
  EXPECT_EQ(state, GAMEOVER_STATE);
  snake_game_over();
  EXPECT_EQ(game.getUserAction(), Terminate);
  EXPECT_EQ(game.getGameInfo()->pause, (int)GAMEOVER_STATE);
  snake.snakeBody.clear();
  game.reset();
}

TEST(Suite_FSM_logic, PAUSE_STATE) {
  Game &game = Game::getInstance();
  game.initField();
  snake_spawn();
  game.setUserAction(Pause);
  updateCurrentState();
  EXPECT_EQ(game.getCurrentState(), PAUSE_STATE);
  EXPECT_EQ(game.getGameInfo()->pause, (int)PAUSE_STATE);
  game.setUserAction(Pause);
  updateCurrentState();
  EXPECT_EQ(game.getCurrentState(), MOVING_STATE);
  EXPECT_EQ(game.getGameInfo()->pause, (int)START_STATE);

  game.reset();
}
TEST(Suite_FSM_logic, EXIT_STATE) {
  Game &game = Game::getInstance();
  game.initField();
  snake_game_exit();

  EXPECT_EQ(game.getCurrentState(), EXIT_STATE);
  EXPECT_EQ(game.getGameInfo()->pause, (int)EXIT_STATE);

  game.reset();
}
TEST(Suite_FSM_logic, WIN_STATE) {
  Game &game = Game::getInstance();
  game.initField();
  game.getGameInfo()->score = 199;
  game.plusScore();
  setGameWon();
  updateCurrentState();
  EXPECT_EQ(game.getCurrentState(), WIN_STATE);
  EXPECT_EQ(game.getGameInfo()->pause, (int)WIN_STATE);

  game.reset();
}
TEST(Suite_FSM_logic, WIN_STATE_2) {
  Game &game = Game::getInstance();
  game.initField();
  game.getGameInfo()->score = 199;
  game.plusScore();
  game.setUserAction(Action);
  game.setCurrentState(WIN_STATE);
  updateCurrentState();
  EXPECT_EQ(game.getCurrentState(), WIN_STATE);
  EXPECT_EQ(game.getGameInfo()->pause, (int)WIN_STATE);

  game.reset();
}
TEST(Suite_Controller, Controller_getGameInfo) {
  Game &game = Game::getInstance();

  EXPECT_EQ(game.getGameInfo()->level, SnakeController::getGameInfo()->level)
      << "Level mismatch";
  EXPECT_EQ(game.getGameInfo()->score, SnakeController::getGameInfo()->score)
      << "Score mismatch";
  EXPECT_EQ(game.getGameInfo()->high_score,
            SnakeController::getGameInfo()->high_score)
      << "High score mismatch";
  EXPECT_EQ(game.getGameInfo()->speed, SnakeController::getGameInfo()->speed)
      << "Speed mismatch";
  EXPECT_EQ(game.getGameInfo()->pause, SnakeController::getGameInfo()->pause)
      << "Pause mismatch";
}
TEST(Suite_Controller, Controller_userInput) {
  Game &game = Game::getInstance();

  SnakeController::userInput(Pause, false);
  EXPECT_EQ(game.getGameInfo()->pause, (int)PAUSE_STATE) << "Pause mismatch";
  SnakeController::userInput(Terminate, false);
  EXPECT_EQ(game.getGameInfo()->pause, (int)GAMEOVER_STATE) << "Pause mismatch";
}
