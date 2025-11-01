#include "s21_brickgame.h"

int main(void) {
  // WINDOW* body_field = NULL;
  init_library();
  init_windows();
  int choice = 0;
  while (choice != -1) {
    choice = show_menu();
    if (choice == 1) {
      init_tetris_game();
      main_brick_game_loop();
      choice = 0;
    } else if (choice == 2) {
      // init_snake_game();
      // Создание объектов игры, контроллера и представления
      SnakeGame game;
      game.snake_state = START_STATE;
      SnakeController controller(game);
      SnakeView view;

      main_snake_game_loop(controller, view);
      choice = 0;
    } else if (choice == 3) {
      choice = -1;
    }
  }
  endwin();
}

void main_brick_game_loop() {
  bool break_flag = TRUE;
  int signal = ERR;
  GameInfo_t* game_info;
  long long prev_time = current_timestamp();
  while (break_flag) {
    game_info = updateCurrentState(get_signal(signal));
    print_playground(game_info);
    long long current_time = current_timestamp();
    if (game_info->pause != 2 && game_info->pause != 3 &&
        game_info->pause != 1 && current_time > prev_time + game_info->speed) {
      game_info = updateCurrentState(Down);
      print_playground(game_info);
      prev_time = current_time;
    }
    if (game_info->pause == 2) {  // Escape
      break_flag = FALSE;
    } else if (game_info->pause == 1) {  // Pause
      break_flag = pause_loop(game_info);
      signal = 0;
    } else if (game_info->pause == 3) {  // Game over
      signal = game_over_loop(game_info, &break_flag, 1);
    } else {
      signal = getch();
    }
  }
  memory_free(game_info);
}

void main_snake_game_loop(SnakeController& controller, SnakeView& view) {
  while (true) {
    auto lastUpdate = std::chrono::steady_clock::now();
    while (!controller.isGameOver()) {
      // 1. Обновляем состояние игры
      controller.updateGame();

      // 2. Отрисовываем текущее состояние
      view.draw(controller.getGameState());

      // 3. Обрабатываем пользовательский ввод
      UserAction_t action;
      updateCurrentState(action);
      bool hold = (action == UserAction_t::Action);
      controller.processInput(action, hold);

      // 4. Ждем немного перед следующим обновлением
      std::this_thread::sleep_for(
          std::chrono::milliseconds(250));  // Устанавливаем нужную задержку
    }
    // Если игра завершена, выводим сообщение и предлагаем начать заново
    mvprintw(5, 6, "Game Over!");
    mvprintw(6, 2, "Press 'r' to restart");
    mvprintw(7, 4, "or 'q' to quit.");
    refresh();

    // Ожидание ввода пользователя для перезапуска или выхода
    int ch;
    while (true) {
      // ch = getch();  // Получаем ввод от пользователя

      if (ch == 'q') {
        // Если нажата клавиша 'q', выходим из цикла
        return;  // Выход из основного игрового цикла
      } else if (ch == 'r') {
        controller.resetGame();  // Перезапуск игры
        // Очищаем сообщение "Game Over" на строке 5
        move(5, 2);  // Перемещаем курсор на строку 5, позиция 2
        // clrtoeol();  // Очищаем до конца строки
        refresh();   // Обновляем экран
        break;  // Выход из цикла ожидания ввода для начала новой игры
      }
    }
  }
  // bool break_flag = TRUE;
  // int signal = ERR;
  // GameInfo_t* gameInfo;
  // long long prev_time = current_timestamp();
  // while (break_flag) {
  //   gameInfo = updateCurrentState(get_signal(signal));
  //   print_playground(gameInfo);
  //   long long current_time = current_timestamp();
  //   if (gameInfo->pause != 2 && gameInfo->pause != 3 &&
  //       gameInfo->pause != 1 && current_time > prev_time + gameInfo->speed)
  //       {
  //     gameInfo = updateCurrentState(Down);
  //     print_playground(gameInfo);
  //     prev_time = current_time;
  //   }
  //   if (gameInfo->pause == 2) {  // Escape
  //     break_flag = FALSE;
  //   } else if (gameInfo->pause == 1) {  // Pause
  //     break_flag = pause_loop(gameInfo);
  //     signal = 0;
  //   } else if (gameInfo->pause == 3) {  // Game over
  //     signal = game_over_loop(gameInfo, &break_flag, 2);
  //   } else {
  //     signal = getch();
  //   }
  // }
  // memory_free(gameInfo);
}
UserAction_t get_signal(int user_input) {
  UserAction_t key_code = Start;
  if (user_input == KEY_DOWN)
    key_code = Down;
  else if (user_input == KEY_LEFT)
    key_code = Left;
  else if (user_input == KEY_RIGHT)
    key_code = Right;
  else if (user_input == ESCAPE_KEY)
    key_code = Terminate;
  else if (user_input == ENTER_KEY)
    key_code = Pause;
  else if (user_input == SPACE_KEY)
    key_code = Action;
  return key_code;
}
bool pause_loop(GameInfo_t* game) {
  bool break_flag = TRUE;
  while (game->pause) {
    int signal = ERR;
    print_pause();
    signal = getch();
    if (signal == ENTER_KEY || signal == ESCAPE_KEY) {
      game->pause = 0;
      if (signal == ESCAPE_KEY) break_flag = FALSE;
      remove_pause();
    }
  }
  return break_flag;
}
int game_over_loop(GameInfo_t* game_info, bool* break_flag, int fileFlag) {
  int signal = 0;
  print_game_over(game_info);
  while (signal != ENTER_KEY && signal != ESCAPE_KEY) {
    signal = getch();
    if (signal == ESCAPE_KEY) {
      *break_flag = FALSE;
    }
  }
  if (signal == ENTER_KEY) {
    signal = 0;
    game_info->level = 1;
    game_info->score = 0;
    game_info->high_score =
        read_file_high_score(fileFlag);  // read_high_score();
    game_info->speed = GAME_SPEED;
    game_info->pause = 0;
  }

  return signal;
}