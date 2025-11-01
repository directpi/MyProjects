#ifndef TETRIS_H
#define TETRIS_H

#include <thread>
#include <chrono>

#include "brick_game/common/common_file.h"
#include "brick_game/common/defines.h"
#include "brick_game/tetris/s21_tetris_back.h"
#include "gui/cli/s21_gui.h"
#include "brick_game/snake/model/s21_model.h"
#include "brick_game/snake/controller/s21_controller.h"

// using namespace s21;
void main_brick_game_loop();
void main_snake_game_loop(SnakeController& controller, SnakeView& view);
UserAction_t get_signal(int user_input);
bool pause_loop(GameInfo_t* game_info);
int game_over_loop(GameInfo_t* game_info, bool* flag, int fileFlag);
#endif