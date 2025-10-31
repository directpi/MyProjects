#include "s_gui.hpp"

namespace snake {

static WINDOW *body_field = nullptr;
static WINDOW *game_field = nullptr;

static WINDOW *getBodyField() {
  if (!body_field) {
    body_field = newwin(25, 40, 0, 0);
    if (!body_field) throw std::runtime_error("Ошибка создания body_field");
    wbkgd(body_field, COLOR_PAIR(1));
    box(body_field, 0, 0);
    wrefresh(body_field);
  }
  return body_field;
}

static WINDOW *getGameField() {
  if (!game_field) {
    WINDOW *body = getBodyField();
    game_field = derwin(body, 22, 22, 1, 1);
    if (!game_field) throw std::runtime_error("Ошибка создания game_field");
    wbkgd(game_field, COLOR_PAIR(8));
    wrefresh(game_field);
  }
  return game_field;
}

/**
 * @brief Draw game windows
 */
void init_tetris_game() {
  wbkgd(body_field, COLOR_PAIR(8));
  box(body_field, 0, 0);
  wattron(body_field, A_BOLD | COLOR_PAIR(8));
  mvwprintw(body_field, 2, 25, "SCORE:");
  mvwprintw(body_field, 4, 25, "HIGH SCORE:");
  mvwprintw(body_field, 6, 25, "LEVEL:");
  mvwprintw(body_field, 8, 25, "NEXT:");
  mvwprintw(body_field, 15, 25, "MOVE: Arrows");
  mvwprintw(body_field, 17, 25, "PAUSE: P");
  mvwprintw(body_field, 19, 25, "ROTATE: Space");
  mvwprintw(body_field, 21, 25, "EXIT: Esc");
  wrefresh(body_field);
  wrefresh(game_field);
}

void init_windows() {
  WINDOW *body =
      getBodyField();  // Автоматически создает или возвращает body_field
  WINDOW *game =
      getGameField();  // Автоматически создает или возвращает game_field

  wbkgd(body, COLOR_PAIR(1));
  box(body, 0, 0);
  wrefresh(body);
  wbkgd(game, COLOR_PAIR(8));
  box(game, 0, 0);
  wrefresh(game);
}

void cleanup_windows() {
  if (game_field) {
    delwin(game_field);
    game_field = nullptr;
  }
  if (body_field) {
    delwin(body_field);
    body_field = nullptr;
  }
  endwin();
}

void draw_menu(int choice) {
  werase(body_field);
  wrefresh(body_field);
  werase(game_field);
  wrefresh(game_field);
  wbkgd(body_field, COLOR_PAIR(8));
  wattron(body_field, A_BOLD | COLOR_PAIR(8));
  box(body_field, 0, 0);
  wbkgd(game_field, COLOR_PAIR(8));
  box(game_field, 0, 0);
  wattron(game_field, A_BOLD | COLOR_PAIR(8));
  mvwprintw(game_field, 8, 6, "%-9s", "MAIN MENU");
  mvwprintw(game_field, 10, 4, "%-13s", "1. BRICK GAME");
  mvwprintw(game_field, 12, 4, "%-8s", "2. SNAKE");
  mvwprintw(game_field, 14, 4, "%-7s", "3. QUIT (Esc)");
  wattroff(game_field, A_BOLD | COLOR_PAIR(8));
  // Выделить выбранный пункт
  for (int i = 0; i < 3; i++) {
    if (choice == BRICK_GAME && i == 0) {
      wattron(game_field, A_REVERSE);
      mvwprintw(game_field, 10 + i * 2, 4, "%-13s", "1. BRICK GAME");
      wattroff(game_field, A_REVERSE);
    } else if (choice == SNAKE && i == 1) {
      wattron(game_field, A_REVERSE);
      mvwprintw(game_field, 10 + i * 2, 4, "%-8s", "2. SNAKE");
      wattroff(game_field, A_REVERSE);
    } else if (choice == QUIT && i == 2) {
      wattron(game_field, A_REVERSE);
      mvwprintw(game_field, 10 + i * 2, 4, "%-7s", "3. QUIT (Esc)");
      wattroff(game_field, A_REVERSE);
    }
  }
  wrefresh(body_field);
  wrefresh(game_field);
  wattroff(game_field, A_BOLD | COLOR_PAIR(8));
}

SnakeView::SnakeView() {
  if (!body_field || !game_field) {
    throw std::runtime_error("Не инициализированны окна в SnakeView");
  }
  wbkgd(body_field, COLOR_PAIR(1));
  box(body_field, 0, 0);
  wbkgd(game_field, COLOR_PAIR(1));
  box(game_field, 0, 0);
  wrefresh(game_field);
  wrefresh(body_field);
}

void init_library() {
  initscr();  // Инициализация ncurses
  cbreak();   // Линии ввода без буферизации
  noecho();  // Не показывать на экране вводимые символы
  timeout(0);  // Непрерывный опрос клавиатуры
  keypad(stdscr, TRUE);  // Включение поддержки специальных клавиш
  start_color();
  curs_set(0);  // скрывает курсор
  // Проверка, что значения цветов находятся в диапазоне от 0 до 1000
  init_color(COLOR_BLACK, 0, 0, 0);           // черный
  init_color(COLOR_WHITE, 1000, 1000, 1000);  // белый
  init_color(100, 670, 700, 670);             // светлосерый
  init_color(101, 200, 200, 200);             // серый
  init_color(102, 300, 300, 300);             //
  init_color(103, 600, 620, 600);             //
  init_color(104, 800, 800, 0);               // серый
  init_color(105, 300, 420, 600);             // синий
  init_color(200, 1000, 0, 0);                // красный
  init_pair(1, COLOR_BLACK, COLOR_WHITE);
  init_pair(2, COLOR_BLACK, 100);  // светлосерый
  init_pair(3, COLOR_WHITE, 101);  // темно-серый
  init_pair(4, COLOR_WHITE, 102);  // серый
  init_pair(5, COLOR_WHITE, 103);
  init_pair(6, COLOR_WHITE, 104);  // желтый
  init_pair(7, COLOR_WHITE, 105);  // синий
  init_pair(8, COLOR_BLUE, COLOR_WHITE);
  init_pair(9, COLOR_BLUE, 100);
  init_pair(10, COLOR_RED, COLOR_WHITE);  // красный для яблока
  bkgd(COLOR_PAIR(3));
  refresh();
}

int show_menu() {
  int choice = BRICK_GAME;
  draw_menu(choice);

  bool exit_loop = false;  // Флаг выхода из цикла
  while (!exit_loop) {
    int input = getch();
    switch (input) {
      case KEY_UP:
        if (choice > BRICK_GAME) choice--;
        break;
      case KEY_DOWN:
        if (choice < QUIT) choice++;
        break;
      case ENTER_KEY:
        exit_loop = true;  // Завершение цикла
        break;
      case ESCAPE_KEY:
        choice = -1;
        exit_loop = true;  // Завершение цикла
        break;
    }
    draw_menu(choice);
    napms(100);
  }
  return choice;
}

/**
 * @brief Print game field
 * @param *game
 */
void print_playground(const GameInfo_t *game) {
  // int x = 0, y = 0;
  // curs_set(0);
  for (int i = 0; i < 20; i++) {
    for (int j = 0; j < 10; j++) {
      if (game->field[i][j]) {
        wattron(game_field, COLOR_PAIR(7));
        mvwprintw(game_field, i + 1, 2 * j + 1, "  ");
      } else {
        wattron(game_field, COLOR_PAIR(5));
        mvwprintw(game_field, i + 1, 2 * j + 1, "  ");
      }
    }
  }
  mvwprintw(body_field, 3, 25, "%d      ", game->score);
  mvwprintw(body_field, 5, 25, "%d", game->high_score);
  mvwprintw(body_field, 7, 25, "%d", game->level);
  wattron(body_field, COLOR_PAIR(3));
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (game->next[i][j]) {
        wattron(body_field, COLOR_PAIR(7));
        mvwprintw(body_field, 10 + i, 2 * j + WIDTH * 2 + INFO_WIDTH / 2, "  ");
      } else {
        wattron(body_field, COLOR_PAIR(1));
        mvwprintw(body_field, i + 10, 2 * j + WIDTH * 2 + INFO_WIDTH / 2, "  ");
      }
    }
  }
  wattron(body_field, A_BOLD | COLOR_PAIR(8));
  wrefresh(body_field);
  wrefresh(game_field);
}

/**
 * @brief Print game over info
 * @param *game
 */
void print_game_over(GameInfo_t *game, bool winFlag) {
  // curs_set(0);
  werase(game_field);
  werase(body_field);
  wrefresh(body_field);
  wrefresh(game_field);
  wattron(game_field, A_BOLD | COLOR_PAIR(8));
  // wbkgd(game_field, COLOR_PAIR(1));
  box(game_field, 0, 0);
  if (winFlag)
    mvwprintw(game_field, 7, (WIDTH * 2) / 2 - 4, "YOU WIN!!!");
  else
    mvwprintw(game_field, 7, (WIDTH * 2) / 2 - 4, "GAME OVER");
  mvwprintw(game_field, 9, (WIDTH * 2) / 2 - 8, "YOUR SCORE IS %d",
            game->score);
  mvwprintw(game_field, 11, (WIDTH * 2) / 2 - 8, "ENTER TO TRY AGAIN");
  mvwprintw(game_field, 13, (WIDTH * 2) / 2 - 6, "ESCAPE TO EXIT");
  wrefresh(game_field);
}

/**
 * @brief Print pause info
 */
void print_pause(bool flag) {
  // curs_set(0);
  if (flag) {
    wattron(body_field, A_BOLD | COLOR_PAIR(8));
    mvwprintw(body_field, 23, (WIDTH * 2 + INFO_WIDTH * 2) / 2 - 14,
              "PRESS P TO CONTINUE");
    wrefresh(body_field);
  } else {
    {
      wattron(body_field, A_BOLD | COLOR_PAIR(8));
      mvwprintw(body_field, 23, (WIDTH * 2 + INFO_WIDTH * 2) / 2 - 14,
                "                    ");
      wrefresh(body_field);
    }
  }
}

SnakeView::~SnakeView() {
  delwin(body_field);
  delwin(game_field);
}

void SnakeView::draw() {
  drawField();
  wrefresh(game_field);
  drawScore();
  wrefresh(body_field);
}

void SnakeView::drawField() {
  if (!game_field || !body_field) {
    std::cerr << "Ошибка: game_field или body_field не инициализировано."
              << std::endl;
    return;
  }
  int max_y = 0, max_x = 0;
  getmaxyx(game_field, max_y, max_x);
  // Очистка игрового поля
  for (int i = 0; i < HEIGHT; ++i) {
    for (int j = 0; j < WIDTH; ++j) {
      if ((i + 1) < max_y && (j * 2 + 1) < max_x) {
        wattron(game_field, COLOR_PAIR(8));
        mvwprintw(game_field, i + 1, j * 2 + 1, "  ");
        wattroff(game_field, COLOR_PAIR(8));
      }
    }
  }

  // Прорисовка тела змейки и яблока
  for (int i = 0; i < HEIGHT; ++i) {
    for (int j = 0; j < WIDTH; ++j) {
      if ((i + 1) < max_y && (j * 2 + 1) < max_x) {
        if (SnakeController::getGameInfo()->field[i][j] == 1) {
          wattron(game_field, COLOR_PAIR(7));  // Цвет тела змейки
          mvwprintw(game_field, i + 1, j * 2 + 1, "[]");
          wattroff(game_field, COLOR_PAIR(7));
        } else if (SnakeController::getGameInfo()->field[i][j] == 2) {
          wattron(game_field, COLOR_PAIR(10));  // Цвет яблока
          mvwprintw(game_field, i + 1, j * 2 + 1, "@");
          wattroff(game_field, COLOR_PAIR(10));
        }
      }
    }
  }
  // Обновление экранов
  wrefresh(body_field);
  wrefresh(game_field);
}

void SnakeView::drawScore() {
  if (!body_field) {
    std::cerr << "Ошибка: body_field не инициализировано." << std::endl;
    return;
  }
  wattron(body_field, A_BOLD | COLOR_PAIR(8));
  // Отображение текущего счета, рекорда и уровня
  mvwprintw(body_field, 2, 25, "SCORE:");
  mvwprintw(body_field, 3, 25, "%d", SnakeController::getGameInfo()->score);
  mvwprintw(body_field, 4, 25, "HIGH SCORE:");
  if (SnakeController::getGameInfo()->score >
      SnakeController::getGameInfo()->high_score) {
    mvwprintw(body_field, 5, 25, "%d", SnakeController::getGameInfo()->score);
  } else {
    mvwprintw(body_field, 5, 25, "%d",
              SnakeController::getGameInfo()->high_score);
  }
  mvwprintw(body_field, 6, 25, "LEVEL:");
  mvwprintw(body_field, 7, 25, "%d", SnakeController::getGameInfo()->level);
  mvwprintw(body_field, 13, 25, "MOVE: Arrows");
  mvwprintw(body_field, 15, 25, "PAUSE: P");
  mvwprintw(body_field, 17, 25, "SPEED: Space");
  mvwprintw(body_field, 19, 25, "EXIT: Esc");
  wattroff(body_field, A_BOLD | COLOR_PAIR(8));
  wrefresh(body_field);
}

void main_brick_game_loop() {
  bool break_flag = true;
  int signal = ERR;
  GameInfo_t *game_info = nullptr;
  long long prev_time = current_timestamp();
  while (break_flag) {
    init_tetris_game();
    game_info = updateCurrentState(get_signal(signal));
    print_playground(game_info);
    if (game_info->pause == (int)PAUSE_STATE) {
      print_pause(true);
    } else {
      print_pause(false);
    }
    long long current_time = current_timestamp();
    if (game_info->pause != (int)EXIT_STATE &&
        game_info->pause != (int)GAMEOVER_STATE &&
        game_info->pause != (int)PAUSE_STATE &&
        current_time > prev_time + game_info->speed) {
      game_info = updateCurrentState(Down);
      print_playground(game_info);
      prev_time = current_time;
    }
    if (game_info->pause == (int)EXIT_STATE) {  // Escape
      break_flag = false;
    } else if (game_info->pause == (int)GAMEOVER_STATE) {  // Game over
      signal = game_over_loop(game_info, 1, 0);
      if (signal == ESCAPE_KEY) break_flag = false;
      clear_field(game_info->field);
      clear_field(game_info->next);
    } else {
      signal = getch();
    }
  }
  memory_free(game_info);
}

void main_snake_game_loop() {
  SnakeView view;
  int signal = ERR;
  bool break_flag = false;
  while (!break_flag) {
    // 1. Обновляем состояние игры
    GameInfo_t *game_info = updateCurrentState();
    // 2. Отрисовываем текущее состояние
    if (game_info->pause == 0) {
      SnakeView::draw();
    }
    if (game_info->pause == (int)PAUSE_STATE) {
      print_pause(true);
    } else {
      print_pause(false);
    }
    // 3. Обрабатываем пользовательский ввод
    if (game_info->pause == EXIT_STATE) {  // Escape
      break_flag = true;
    } else if (game_info->pause == GAMEOVER_STATE ||
               game_info->pause == WIN_STATE) {  // Game over or Win
      bool winFlag = game_info->pause == WIN_STATE ? 1 : 0;
      if (signal == ESCAPE_KEY) {
        break_flag = true;
      } else {
        signal = game_over_loop(game_info, 2, winFlag);
      }
      snake_check_high_score(game_info);
      setGameOver(false);
    } else {
      signal = getch();
    }
    UserAction_t action = get_signal(signal);
    bool hold = (action == UserAction_t::Action);
    SnakeController::userInput(action, hold);
  }
  // Завершаем игру
  setGameOver(false);
  cleanup_windows();
}

UserAction_t get_signal(int user_input) {
  UserAction_t key_code = Start;
  if (user_input == KEY_DOWN)
    key_code = Down;
  else if (user_input == KEY_UP)
    key_code = Up;
  else if (user_input == KEY_LEFT)
    key_code = Left;
  else if (user_input == KEY_RIGHT)
    key_code = Right;
  else if (user_input == ESCAPE_KEY)
    key_code = Terminate;
  else if (user_input == P_KEY || user_input == p_KEY)
    key_code = Pause;
  else if (user_input == ENTER_KEY)
    key_code = Start;
  else if (user_input == SPACE_KEY)
    key_code = Action;
  return key_code;
}

int game_over_loop(GameInfo_t *game_info, int fileFlag, bool winFlag) {
  int signal = 0;
  print_game_over(game_info, winFlag);
  while (signal != ENTER_KEY && signal != ESCAPE_KEY) {
    signal = getch();
    // if (signal == ESCAPE_KEY) {
    //   break_flag = true;
    // }
  }
  if (signal == ENTER_KEY) {
    if (fileFlag == 1) {
      game_info->level = 1;
      game_info->score = 0;
      game_info->high_score = read_file_high_score(1);
      game_info->speed = GAME_SPEED;
      game_info->pause = 0;
    }
  }
  game_info->pause = 0;
  return signal;
}
}  // namespace snake
