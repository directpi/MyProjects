#include "s21_tetris.h"

int main(void) {
  init_library();

  init_window();
  main_game_loop();
  endwin();
}
void main_game_loop() {
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
      signal = game_over_loop(game_info, &break_flag);
    } else {
      signal = getch();
    }
  }
  memory_free(game_info);
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
int game_over_loop(GameInfo_t* game_info, bool* break_flag) {
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
    game_info->high_score = read_file_high_score();  // read_high_score();
    game_info->speed = GAME_SPEED;
    game_info->pause = 0;
  }

  return signal;
}