#ifndef SNAKE_CONTROLLER_HPP
#define SNAKE_CONTROLLER_HPP

#include <chrono>
#include <iostream>
#include <thread>

#include "../../common/common_file.h"
#include "../../common/defines.h"
#include "../model/s_model.hpp"

namespace snake {
class SnakeController {
public:
  static void userInput(UserAction_t action, bool hold);
  static const GameInfo_t *getGameInfo();
};
}  // namespace snake
#endif // SNAKE_CONTROLLER_HPP
