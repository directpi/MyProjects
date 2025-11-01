#ifndef TETRIS_H
#define TETRIS_H

#include "brick_game/tetris/s21_tetris_back.h"
#include "common/common_file.h"
#include "common/defines.h"
#include "gui/cli/s21_tetris_front.h"

void main_game_loop();
void print_playground(const GameInfo_t* game);
UserAction_t get_signal(int user_input);
bool pause_loop(GameInfo_t* game_info);
int game_over_loop(GameInfo_t* game_info, bool* flag);
#endif