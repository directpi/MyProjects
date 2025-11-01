#ifndef SNAKE_CONTROLLER_HPP
#define SNAKE_CONTROLLER_HPP

#include <chrono>
#include <iostream>
#include <thread>

#include "../../common/common_file.h"
#include "../../common/defines.h"
#include "../model/s21_model.hpp"

namespace s21 {
class SnakeController {
public:
  static void userInput(UserAction_t action, bool hold);
  static const GameInfo_t *getGameInfo();
};
}  // namespace s21
#endif // SNAKE_CONTROLLER_HPP
