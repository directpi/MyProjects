#include "s_controller.hpp"
namespace snake {

void SnakeController::userInput(UserAction_t action, bool hold) {
  Snake &snake = Snake::getInstance();

  if (!snake.snakeBody.empty()) {
    // Логика обработки направлений
    if (action == UserAction_t::Up || action == UserAction_t::Down ||
        action == UserAction_t::Left || action == UserAction_t::Right) {
      snake.lastTurn = action;
      turn();
    }
  }
  // Обработка паузы
  if (action == UserAction_t::Pause) {
    snake_pause();
  }

  // Обработка ускорения (пробел)
  if (action == UserAction_t::Action) {
    if (hold)  // Повторное выполнение действия при удерживании
      moving();
  }

  // Завершение игры
  if (action == UserAction_t::Terminate) {
    setGameOver(true);
  }

  if (action == UserAction_t::Start &&
      (getGameInfo()->pause == (int)GAMEOVER_STATE ||
       getGameInfo()->pause == (int)EXIT_STATE)) {
    setGameOver(false);
  }
}

const GameInfo_t *SnakeController::getGameInfo() {
  Game &game = Game::getInstance();
  return game.getGameInfo();
}

}  // namespace snake
