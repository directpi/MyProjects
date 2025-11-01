#include "s21_gui.hpp"

#include <deque>
#include <mutex>

namespace s21 {

// Последний считанный код клавиши (для отображения в правой панели)
// static int g_lastKeyCode = ERR;

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
  keypad(body, TRUE);
  keypad(game, TRUE);
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
  // поле и рамку инициализируем один раз при старте, без полного стирания
  // каждый кадр
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
  mvwprintw(game_field, 14, 4, "%-8s", "3. RACING");
  mvwprintw(game_field, 16, 4, "%-7s", "4. QUIT (Esc)");
  wattroff(game_field, A_BOLD | COLOR_PAIR(8));
  // Выделить выбранный пункт
  for (int i = 0; i < 4; i++) {
    if (choice == BRICK_GAME && i == 0) {
      wattron(game_field, A_REVERSE);
      mvwprintw(game_field, 10 + i * 2, 4, "%-13s", "1. BRICK GAME");
      wattroff(game_field, A_REVERSE);
    } else if (choice == SNAKE && i == 1) {
      wattron(game_field, A_REVERSE);
      mvwprintw(game_field, 10 + i * 2, 4, "%-8s", "2. SNAKE");
      wattroff(game_field, A_REVERSE);
    } else if (choice == RACING && i == 2) {
      wattron(game_field, A_REVERSE);
      mvwprintw(game_field, 10 + i * 2, 4, "%-8s", "3. RACING");
      wattroff(game_field, A_REVERSE);
    } else if (choice == QUIT && i == 3) {
      wattron(game_field, A_REVERSE);
      mvwprintw(game_field, 10 + i * 2, 4, "%-7s", "4. QUIT (Esc)");
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
  set_escdelay(25);
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
  // В меню — блокирующий ввод для стабильной обработки Enter/цифр/букв
  flushinp();
  timeout(-1);
  while (!exit_loop) {
    int input = getch();
    switch (input) {
      case KEY_UP:
        if (choice > BRICK_GAME) choice--;
        break;
      case KEY_DOWN:
        if (choice < QUIT) choice++;
        break;
      case '\n':
      case '\r':
      case KEY_ENTER:
        timeout(0);
        return choice;  // немедленный выход по Enter
      case '1':
        timeout(0);
        return BRICK_GAME;
      case '2':
        timeout(0);
        return SNAKE;
      case '3':
        timeout(0);
        return RACING;  // мгновенный выбор Racing
      case 'r':
      case 'R':
        timeout(0);
        return RACING;  // горячая клавиша Racing
      case '4':
        timeout(0);
        return QUIT;
      case ESCAPE_KEY:
        timeout(0);
        return -1;
    }
    draw_menu(choice);
  }
  // Вернём неблокирующий режим для игровых циклов
  timeout(0);
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
void main_racing_game_loop() {
  bool break_flag = false;
  s21::RacingRestClient client;

  // Подключаемся и стартуем игру один раз перед циклом (как в Desktop)
  if (!client.ensureStarted()) {
    printw("SERVER: Not available!\n");
    refresh();
    napms(2000);
    cleanup_windows();
    return;
  }
  client.userInput(UserAction_t::Action, false);

  // Буферы состояния (как в Snake/Desktop)
  bool frameField[HEIGHT][WIDTH] = {};
  GameInfo_t frameMeta{};

  while (!break_flag) {
    // 1) Получаем состояние
    client.getState(frameField, &frameMeta);

    // 2) Отрисовываем
    drawRacingScore(&frameMeta);
    drawRacingFieldSimple(frameField);
    box(game_field, 0, 0);
    wnoutrefresh(game_field);
    wnoutrefresh(body_field);
    doupdate();

    // Пауза-индикатор
    if (frameMeta.pause == (int)PAUSE_STATE) {
      print_pause(true);
    } else {
      print_pause(false);
    }

    // 3) Ввод
    if (frameMeta.pause == EXIT_STATE) {
      break_flag = true;
    } else if (frameMeta.pause == GAMEOVER_STATE ||
               frameMeta.pause == WIN_STATE) {
      int ch = game_over_loop(&frameMeta, 1, frameMeta.pause == WIN_STATE);
      if (ch == ESCAPE_KEY) {
        break_flag = true;
      } else if (ch == ENTER_KEY || ch == SPACE_KEY) {
        client.stopGame();
        (void)client.ensureStarted(2, 100);
        client.userInput(UserAction_t::Action, false);
      }
    } else {
      int ch = getch();
      if (ch == KEY_ENTER || ch == '\n' || ch == '\r') ch = ENTER_KEY;
      UserAction_t action = get_signal(ch);
      if (action != Start) {
        bool hold = (action == Up);
        client.userInput(action, hold);
        if (action == Terminate) break_flag = true;
      }
    }

    // 4) Тайминг
    napms(25);
  }

  client.stopGame();
  cleanup_windows();
}

// void main_racing_game_loop() {
//   // МАРКЕР: Начало инициализации
//   printw("INIT: Starting Racing game\n");
//   refresh();

//   // Minimal ncurses input init (как в Snake/Tetris)
//   keypad(stdscr, TRUE);
//   // Также включаем keypad на окнах, чтобы спецклавиши обрабатывались везде
//   if (body_field) keypad(body_field, TRUE);
//   if (game_field) keypad(game_field, TRUE);
//   // Неблокирующий режим только на окне ввода (чтобы не перетирался)
//   if (game_field) {
//     wtimeout(game_field, 0);
//     nodelay(game_field, TRUE);
//   }
//   // stdscr не используем для ввода — не меняем его режимы
//   // Основные настройки ввода
//   cbreak();
//   noecho();
//   set_escdelay(25);  // как в Snake
//   curs_set(0);

//   // МАРКЕР: Инициализация завершена
//   printw("INIT: ncurses setup done\n");
//   refresh();

//   s21::RacingRestClient client;

//   // МАРКЕР: Проверка сервера
//   printw("SERVER: Checking server connection\n");
//   refresh();

//   if (!client.ensureStarted()) {
//     printw("SERVER: Not available!\n");
//     refresh();
//     napms(2000);
//     cleanup_windows();
//     return;
//   }

//   // МАРКЕР: Сервер подключен
//   printw("SERVER: Connected successfully\n");
//   refresh();

//   // МАРКЕР: Запуск игры
//   printw("GAME: Starting game on server\n");
//   refresh();

//   // Запускаем игру на сервере (как в Desktop)
//   client.userInput(UserAction_t::Action, false);

//   // Начальная отрисовка
//   werase(body_field);
//   wbkgd(body_field, COLOR_PAIR(8));
//   box(body_field, 0, 0);
//   wrefresh(body_field);
//   wrefresh(game_field);

//   // int signal = ERR;
//   bool break_flag = false;

//   // Локальные буферы (как в Desktop)
//   bool frameField[HEIGHT][WIDTH];
//   GameInfo_t frameMeta{};

//   // Убрано: upHeld больше не нужен в Snake-стиле логике

//   // МАРКЕР: Начало главного цикла
//   printw("LOOP: Starting main game loop\n");
//   refresh();

//   // ===== ГЛАВНЫЙ ЦИКЛ (как в Snake) =====
//   while (!break_flag) {
//     // ===== 1) GET STATE (как в Snake) =====
//     client.getState(frameField, &frameMeta);

//     // ===== 2) DRAW (как в Snake) =====
//     // Полная перерисовка (стабильный режим без исчезновения объектов)
//     drawRacingScore(&frameMeta);
//     drawRacingFieldSimple(frameField);
//     box(game_field, 0, 0);
//     wnoutrefresh(game_field);
//     wnoutrefresh(body_field);
//     doupdate();

//     // ===== 3) INPUT (как в Snake) =====
//     if (frameMeta.pause == (int)EXIT_STATE) {
//       break_flag = true;
//     } else if (frameMeta.pause == (int)GAMEOVER_STATE) {
//       // Обработка Game Over как в Snake
//       int ch = game_over_loop(&frameMeta, 1, false);
//       if (ch == ESCAPE_KEY) {
//         break_flag = true;
//       } else if (ch == ENTER_KEY || ch == SPACE_KEY) {
//         client.stopGame();
//         (void)client.ensureStarted(2, 100);
//         client.userInput(UserAction_t::Action, false);
//       }
//     } else {
//       // Читаем из game_field (куда приходят клавиши)
//       int ch = wgetch(game_field ? game_field : stdscr);
//       if (ch == KEY_ENTER || ch == '\n' || ch == '\r') ch = ENTER_KEY;
//       g_lastKeyCode = ch;

//       // Если вообще ничего не пришло — явно покажем это один раз в правой
//       // панели
//       if (ch == ERR && body_field) {
//         int by = 0, bx = 0;
//         getmaxyx(body_field, by, bx);
//         mvwprintw(body_field, (by > 1 ? by - 2 : 0), 1,
//                   "KEY code: ERR        ");
//         wnoutrefresh(body_field);
//       }

//       // Маркер: печатаем код клавиши под правой панелью (ниже body_field)
//       if (body_field) {
//         int by = 0, bx = 0;
//         getmaxyx(body_field, by, bx);
//         mvwprintw(body_field, (by > 1 ? by - 2 : 0), 1,
//                   "KEY code: %4d  (LEFT=%d RIGHT=%d UP=%d ESC=%d)        ",
//                   g_lastKeyCode, KEY_LEFT, KEY_RIGHT, KEY_UP, ESCAPE_KEY);
//         wnoutrefresh(body_field);
//       }

//       UserAction_t act = get_signal(ch);
//       bool holdNow = (act == Up);
//       client.userInput(act, holdNow);
//       if (act == Terminate) {
//         break_flag = true;
//       }
//     }

//     // ===== 4) TIMING (ускорено в 2 раза) =====
//     napms(25);  // было 50ms

//     if (frameMeta.pause == (int)PAUSE_STATE) {
//       print_pause(true);
//     } else {
//       print_pause(false);
//     }

//     if (frameMeta.pause == EXIT_STATE) {
//       break_flag = true;
//     } else if (frameMeta.pause == GAMEOVER_STATE) {
//       int ch = game_over_loop(&frameMeta, 1, false);
//       if (ch == ESCAPE_KEY) {
//         break_flag = true;
//       } else if (ch == ENTER_KEY || ch == SPACE_KEY) {
//         client.stopGame();
//         (void)client.ensureStarted(2, 100);
//         client.userInput(UserAction_t::Action, false);
//         flushinp();
//       }
//     }
//   }

//   client.stopGame();
//   cleanup_windows();
// }

void drawRacingField(GameInfo_t *game_info) {
  if (!game_info || !game_info->field) return;

  // Рисуем границы поля
  printw("+");
  for (int x = 0; x < WIDTH; x++) {
    printw("-");
  }
  printw("+\n");

  // Рисуем игровое поле
  for (int y = 0; y < HEIGHT; y++) {
    printw("|");
    for (int x = 0; x < WIDTH; x++) {
      if (game_info->field[y] && game_info->field[y][x]) {
        printw("X");  // Простой символ для машин
      } else {
        printw(" ");
      }
    }
    printw("|\n");
  }

  // Рисуем нижнюю границу
  printw("+");
  for (int x = 0; x < WIDTH; x++) {
    printw("-");
  }
  printw("+\n");
}

void drawRacingScore(GameInfo_t *game_info) {
  if (!game_info) return;
  static int last_score = -1;
  static int last_high = -1;
  static int last_level = -1;
  static int last_speed = -1;

  const bool changed = (game_info->score != last_score) ||
                       (game_info->high_score != last_high) ||
                       (game_info->level != last_level) ||
                       (game_info->speed != last_speed);

  if (!changed) return;

  last_score = game_info->score;
  last_high = game_info->high_score;
  last_level = game_info->level;
  last_speed = game_info->speed;

  werase(body_field);
  wbkgd(body_field, COLOR_PAIR(8));
  box(body_field, 0, 0);
  wattron(body_field, A_BOLD | COLOR_PAIR(8));
  mvwprintw(body_field, 2, 25, "SCORE:");
  mvwprintw(body_field, 3, 25, "%d      ", game_info->score);
  mvwprintw(body_field, 5, 25, "HIGH SCORE:");
  mvwprintw(body_field, 6, 25, "%d      ", game_info->high_score);
  mvwprintw(body_field, 8, 25, "LEVEL:");
  mvwprintw(body_field, 9, 25, "%d      ", game_info->level);
  mvwprintw(body_field, 11, 25, "SPEED:");
  mvwprintw(body_field, 12, 25, "%d      ", game_info->speed);
  mvwprintw(body_field, 14, 25, "MOVE:Left/Right");
  mvwprintw(body_field, 16, 25, "ACCEL: Up");
  mvwprintw(body_field, 18, 25, "PAUSE: P");
  mvwprintw(body_field, 20, 25, "EXIT: Esc");
  // Отложенный вывод, обновляем в главном цикле через doupdate
  wnoutrefresh(body_field);
}

void drawRacingScoreWith(GameInfo_t *game_info, int displaySpeed) {
  if (!game_info) return;
  werase(body_field);
  wbkgd(body_field, COLOR_PAIR(8));
  box(body_field, 0, 0);
  wattron(body_field, A_BOLD | COLOR_PAIR(8));
  mvwprintw(body_field, 2, 25, "SCORE:");
  mvwprintw(body_field, 3, 25, "%d      ", game_info->score);
  mvwprintw(body_field, 5, 25, "HIGH SCORE:");
  mvwprintw(body_field, 6, 25, "%d      ", game_info->high_score);
  mvwprintw(body_field, 8, 25, "LEVEL:");
  mvwprintw(body_field, 9, 25, "%d      ", game_info->level);
  mvwprintw(body_field, 11, 25, "SPEED:");
  mvwprintw(body_field, 12, 25, "%d      ", displaySpeed);
  wnoutrefresh(body_field);
}

void drawRacingFieldSimple(bool field[HEIGHT][WIDTH]) {
  // Полная отрисовка — возвращаем стабильный режим
  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      if (field[y][x]) {
        wattron(game_field, COLOR_PAIR(7));
        mvwprintw(game_field, y + 1, 2 * x + 1, "[]");
        wattroff(game_field, COLOR_PAIR(7));
      } else {
        wattron(game_field, COLOR_PAIR(8));
        mvwprintw(game_field, y + 1, 2 * x + 1, "  ");
        wattroff(game_field, COLOR_PAIR(8));
      }
    }
  }
  wnoutrefresh(game_field);
}

void drawRacingScoreSimple(int score, int level) {
  // Стираем и рисуем правую панель как у Tetris
  werase(body_field);
  wbkgd(body_field, COLOR_PAIR(8));
  box(body_field, 0, 0);
  wattron(body_field, A_BOLD | COLOR_PAIR(8));
  mvwprintw(body_field, 2, 25, "SCORE:");
  mvwprintw(body_field, 3, 25, "%d      ", score);
  mvwprintw(body_field, 5, 25, "LEVEL:");
  mvwprintw(body_field, 6, 25, "%d      ", level);
  wrefresh(body_field);
}

UserAction_t get_signal(int user_input) {
  // Маркер в одну строку: не спамим -1
  if (user_input != ERR) {
    move(1, 0);
    clrtoeol();
    printw("GET_SIGNAL: input=%d", user_input);
    refresh();
  }

  UserAction_t key_code = Start;
  if (user_input == KEY_DOWN) {
    key_code = Down;
    printw("GET_SIGNAL: KEY_DOWN -> Down\n");
  } else if (user_input == KEY_UP) {
    key_code = Up;
    printw("GET_SIGNAL: KEY_UP -> Up\n");
  } else if (user_input == KEY_LEFT) {
    key_code = Left;
  } else if (user_input == KEY_RIGHT) {
    key_code = Right;
  } else if (user_input == 'a' || user_input == 'A') {
    key_code = Left;
    printw("GET_SIGNAL: 'a'/'A' -> Left\n");
  } else if (user_input == 'd' || user_input == 'D') {
    key_code = Right;
    printw("GET_SIGNAL: 'd'/'D' -> Right\n");
  } else if (user_input == 's' || user_input == 'S') {
    key_code = Down;
    printw("GET_SIGNAL: 's'/'S' -> Down\n");
  } else if (user_input == ESCAPE_KEY) {
    key_code = Terminate;
  } else if (user_input == P_KEY || user_input == p_KEY) {
    key_code = Pause;
  } else if (user_input == ENTER_KEY) {
    key_code = Start;
  } else if (user_input == SPACE_KEY) {
    key_code = Action;
  }

  // Возвращаем распознанный код и закрываем функцию
  return key_code;
}

int game_over_loop(GameInfo_t *game_info, int fileFlag, bool winFlag) {
  int signal = 0;
  print_game_over(game_info, winFlag);

  // Переходим во временно блокирующий режим для надёжного захвата Enter
  // не очищаем буфер ввода в игровом цикле
  timeout(-1);
  while (true) {
    signal = getch();
    if (signal == ENTER_KEY || signal == KEY_ENTER || signal == '\n' ||
        signal == '\r' || signal == ESCAPE_KEY) {
      break;
    }
  }
  // Восстанавливаем неблокирующий режим для игрового цикла
  timeout(50);

  if (signal == ENTER_KEY || signal == KEY_ENTER || signal == '\n' ||
      signal == '\r') {
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

// Реализация RacingView (аналогично SnakeView)
void RacingView::draw(bool field[HEIGHT][WIDTH], GameInfo_t *meta) {
  drawField(field);
  wrefresh(game_field);
  drawScore(meta);
  wrefresh(body_field);
}

void RacingView::drawField(bool field[HEIGHT][WIDTH]) {
  if (!game_field || !body_field) {
    std::cerr << "Ошибка: game_field или body_field не инициализировано."
              << std::endl;
    return;
  }

  int max_y = 0, max_x = 0;
  getmaxyx(game_field, max_y, max_x);

  // Инкрементальная отрисовка без стирания окна и рамки

  // Прорисовка машин
  for (int i = 0; i < HEIGHT; ++i) {
    for (int j = 0; j < WIDTH; ++j) {
      if ((i + 1) < max_y && (j * 2 + 1) < max_x) {
        if (field[i][j]) {
          wattron(game_field, COLOR_PAIR(7));  // Цвет машин
          mvwprintw(game_field, i + 1, j * 2 + 1, "[]");
          wattroff(game_field, COLOR_PAIR(7));
        } else {
          // Очищаем пустые клетки
          wattron(game_field, COLOR_PAIR(8));
          mvwprintw(game_field, i + 1, j * 2 + 1, "  ");
          wattroff(game_field, COLOR_PAIR(8));
        }
      }
    }
  }
}

void RacingView::drawScore(GameInfo_t *meta) {
  if (!body_field) {
    std::cerr << "Ошибка: body_field не инициализировано." << std::endl;
    return;
  }

  // Очищаем и перерисовываем панель счета
  werase(body_field);
  wbkgd(body_field, COLOR_PAIR(8));
  box(body_field, 0, 0);

  wattron(body_field, A_BOLD | COLOR_PAIR(8));
  // Отображение текущего счета, рекорда и уровня
  mvwprintw(body_field, 2, 25, "SCORE:");
  mvwprintw(body_field, 3, 25, "%d", meta->score);
  mvwprintw(body_field, 4, 25, "HIGH SCORE:");
  mvwprintw(body_field, 5, 25, "%d", meta->high_score);
  mvwprintw(body_field, 6, 25, "LEVEL:");
  mvwprintw(body_field, 7, 25, "%d", meta->level);
  mvwprintw(body_field, 8, 25, "SPEED:");
  mvwprintw(body_field, 9, 25, "%d", meta->speed);
  mvwprintw(body_field, 13, 25, "MOVE: Arrows");
  mvwprintw(body_field, 15, 25, "ACCEL: Up");
  mvwprintw(body_field, 17, 25, "PAUSE: P");
  mvwprintw(body_field, 19, 25, "EXIT: Esc");
  wattroff(body_field, A_BOLD | COLOR_PAIR(8));
}

}  // namespace s21
