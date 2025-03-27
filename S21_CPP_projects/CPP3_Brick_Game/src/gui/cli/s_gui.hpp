#ifndef GUI_H
#define GUI_H

#include <curses.h>

#include <chrono>

#include "../../brick_game/common/common_file.h"
#include "../../brick_game/common/defines.h"
#include "../../brick_game/snake/controller/s_controller.hpp"
#include "../../brick_game/snake/model/s_model.hpp"
extern "C" {
#include "../../brick_game/tetris/s_tetris_back.h"
}
namespace snake {
// Перечисление для выбора игры
enum MenuChoice { BRICK_GAME = 1, SNAKE, QUIT };

class SnakeView {
 public:
  SnakeView();
  ~SnakeView();
  static void draw();
  static void drawField();
  static void drawScore();
};

void init_library();
void init_tetris_game();
void print_game_over(GameInfo_t *game, bool winFlag);
void print_pause(bool flag);
void draw_menu(int choice);
void print_playground(const GameInfo_t *game);
int show_menu();
void init_windows();
void cleanup_windows();
void main_brick_game_loop();
void main_snake_game_loop();
UserAction_t get_signal(int user_input);
int game_over_loop(GameInfo_t *game_info,  int fileFlag,
                   bool winFlag);

}  // namespace snake
#endif