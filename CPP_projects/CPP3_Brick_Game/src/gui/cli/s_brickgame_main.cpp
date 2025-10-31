#include "s_gui.hpp"

using namespace snake;

int main(void) {
  setlocale(LC_ALL, "ru");
  init_library();
  int choice = 0;
  while (choice != -1) {
    init_windows();
    choice = show_menu();
    if (choice == 1) {
      main_brick_game_loop();
      choice = 0;
    } else if (choice == 2) {
      main_snake_game_loop();
      choice = 0;
    } else if (choice == 3) {
      choice = -1;
    }
  }
  curs_set(1);
  endwin();
  return 0;
}
