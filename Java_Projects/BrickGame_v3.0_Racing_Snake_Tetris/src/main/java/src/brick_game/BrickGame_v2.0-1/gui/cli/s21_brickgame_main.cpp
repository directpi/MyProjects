#include "s21_gui.hpp"

using namespace s21;

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
      // Очистим экран меню и буфер ввода перед запуском гонок
      // не очищаем буфер ввода — мешает чтению ESC последовательностей
      timeout(0);
      werase(stdscr);
      refresh();
      main_racing_game_loop();
      choice = 0;
    } else if (choice == 4) {
      choice = -1;
    }
  }
  curs_set(1);
  endwin();
  return 0;
}
