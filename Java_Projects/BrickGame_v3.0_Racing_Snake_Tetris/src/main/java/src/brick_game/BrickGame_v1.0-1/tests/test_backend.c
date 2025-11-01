#include "test.h"

// Draw I_FIGURE pos_1 and next O_FIGURE
START_TEST(test_1) {
  UserAction_t user_action = Down;
  GameInfo_t game;
  game_info_init(&game);
  // GameState_t g_state = MOVING_STATE;
  Gfigure_t figure;
  figure.current = I_FIGURE;
  figure.rotate = pos_1;
  figure.start_x = 3;
  figure.start_y = 0;
  draw_figure(&game, &figure, &user_action, 1);
  copy_field(&game);
  figure.next = O_FIGURE;
  draw_next(&game, &figure);

  ck_assert_msg(game.field[1][3] == 1, "field draw failed");
  ck_assert_msg(game.field[1][4] == 1, "field draw failed");
  ck_assert_msg(game.field[1][5] == 1, "field draw failed");
  ck_assert_msg(game.field[1][6] == 1, "field draw failed");
  ck_assert_msg(game.next[0][1] == 1, "next draw failed");
  ck_assert_msg(game.next[0][2] == 1, "next draw failed");
  ck_assert_msg(game.next[1][1] == 1, "next draw failed");
  ck_assert_msg(game.next[1][2] == 1, "next draw failed");
  ck_assert_msg(game.next[0][0] == 0, "next draw failed");
  ck_assert_msg(game.next[0][3] == 0, "next draw failed");
  ck_assert_msg(game.next[2][0] == 0, "next draw failed");
  ck_assert_msg(game.next[2][1] == 0, "next draw failed");

  clear_field(game.field);
  memory_free(&game);
}
END_TEST
// Memory allocation
START_TEST(test_2) {
  GameInfo_t game;
  game_field_mem_alloc(&game);

  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      ck_assert_msg(game.field[i][j] == 0, "field mem allocation failed");
      ck_assert_msg(game.next[i][j] == 0, "next mem allocation failed");
    }
  }

  memory_free(&game);
}
END_TEST
// Game init
START_TEST(test_3) {
  GameInfo_t game;
  game_info_init(&game);
  ck_assert_msg(game.level == 1, "Init level failed");
  ck_assert_msg(game.score == 0, "Init score failed");
  ck_assert_msg(game.high_score = read_file_high_score(),
                "Init high score failed");
  ck_assert_msg(game.speed == GAME_SPEED, "Init speed failed");
  ck_assert_msg(game.pause == 0, "Init pause failed");
  memory_free(&game);
}
END_TEST
// Select figure
START_TEST(test_4) {
  g_figures figure;
  figure = select_figure();
  ck_assert_msg(figure >= 0 && figure <= 6, "Select figure failed");
}
END_TEST
// Full_line test
START_TEST(test_5) {
  GameInfo_t game;
  int full_line_count = 0;
  game_info_init(&game);
  for (int i = 0; i < 20; i++) {
    for (int j = 0; j < 10; j++) {
      game.field[i][j] = 1;
    }
  }
  full_line_count = full_line(&game);
  ck_assert_msg(full_line_count == 20, "full line failed");
  memory_free(&game);
}
END_TEST
// test plus level
START_TEST(test_6) {
  GameInfo_t game;
  game_info_init(&game);
  game.level = 5;
  game.score = 601;
  plus_level(&game);
  ck_assert_msg(game.level == 1 + game.score / 600, "Plus level failed");
  memory_free(&game);
}
END_TEST
// Check high score test and write high score file
START_TEST(test_7) {
  GameInfo_t game;
  GameState_t g_state;
  game_info_init(&game);
  game.score = 10000;
  g_state = MOVING_STATE;
  check_high_score(&game, &g_state);
  ck_assert_msg(game.high_score == 10000, "Check high score failed");
  memory_free(&game);
}
END_TEST
// Read file high score files
START_TEST(test_8) {
  GameInfo_t game;
  game_info_init(&game);
  game.high_score = read_file_high_score();
  ck_assert_msg(game.high_score == 10000, "Read file high score failed");
  memory_free(&game);
}
END_TEST
// Write file high score files
START_TEST(test_9) {
  GameInfo_t game;
  game_info_init(&game);
  game.score = 2000;
  int err = write_to_file_high_score(game.score);
  ck_assert_msg(read_file_high_score() == 2000, "Write file high score failed");
  ck_assert_msg(err == 0, "Write file high score failed");
  memory_free(&game);
}
END_TEST
// test timestamp
START_TEST(test_10) {
  long long time_ = current_timestamp();
  ck_assert_msg(time_ > 0, "Timestamp failed");
}
END_TEST
// Full_line test2
START_TEST(test_11) {
  GameInfo_t game;
  int full_line_count = 0;
  game_info_init(&game);
  for (int j = 0; j < 10; j++) {
    game.field[0][j] = 1;
  }
  full_line_count = full_line(&game);
  ck_assert_msg(full_line_count == 1, "full line failed");
  memory_free(&game);
}
END_TEST
// Full_line test3
START_TEST(test_12) {
  GameInfo_t game;
  int full_line_count = 0;
  game_info_init(&game);
  for (int j = 0; j < 10; j++) {
    game.field[0][j] = 1;
    game.field[1][j] = 1;
  }
  full_line_count = full_line(&game);
  ck_assert_msg(full_line_count == 2, "full line failed");
  memory_free(&game);
}
END_TEST
// Full_line test4
START_TEST(test_13) {
  GameInfo_t game;
  int full_line_count = 0;
  game_info_init(&game);
  for (int j = 0; j < 10; j++) {
    game.field[0][j] = 1;
    game.field[1][j] = 1;
    game.field[2][j] = 1;
  }
  full_line_count = full_line(&game);
  ck_assert_msg(full_line_count == 3, "full line failed");
  memory_free(&game);
}
END_TEST
START_TEST(test_1_fsm) {
  GameInfo_t game;
  game_info_init(&game);
  ck_assert_msg(game.score == 0, "game info init failed");
  ck_assert_msg(game.high_score == read_file_high_score(),
                "game info init failed");
  ck_assert_msg(game.level == 1, "game info init failed");
  ck_assert_msg(game.pause == 0, "game info init failed");
  ck_assert_msg(game.speed == GAME_SPEED, "game info init failed");
  memory_free(&game);
}
END_TEST

START_TEST(test_2_fsm) {
  g_figures figure;
  figure = select_figure(0);
  ck_assert_msg((int)figure >= 0 && (int)figure <= 6, "select_figure failed");
}
END_TEST

START_TEST(test_3_fsm) {
  g_figures figure = 2;
  figure = select_figure(figure);
  ck_assert_msg((int)figure >= 0 && (int)figure <= 6, "select_figure failed");
}
END_TEST

START_TEST(test_4_fsm) {
  GameState_t g_state = START_STATE;
  GameInfo_t *game = updateCurrentState(Start);
  ck_assert_msg(game->level == 1, "level failed");
  ck_assert_msg(game->score == 0, "score failed");
  ck_assert_msg(game->high_score == read_file_high_score(),
                "high score read failed");
  ck_assert_msg(game->speed == GAME_SPEED, "speed failed");
  ck_assert_msg(game->pause == 0, "pause failed");
  ck_assert_msg(g_state == START_STATE, "g_state failed");

  game = updateCurrentState(Up);
  ck_assert_msg(game->field[1][1] == 0, "game field failed");
  ck_assert_msg(game->field[1][2] == 0, "game field failed");
  ck_assert_msg(game->field[1][3] == 1, "game field failed");
  ck_assert_msg(game->field[1][4] == 1, "game field failed");
  ck_assert_msg(game->field[1][5] == 1, "game field failed");
  ck_assert_msg(game->field[1][6] == 1, "game field failed");
  ck_assert_msg(game->field[1][7] == 0, "game field failed");
  game = updateCurrentState(Left);
  ck_assert_msg(game->field[1][1] == 0, "game field failed");
  ck_assert_msg(game->field[1][2] == 1, "game field failed");
  ck_assert_msg(game->field[1][3] == 1, "game field failed");
  ck_assert_msg(game->field[1][4] == 1, "game field failed");
  ck_assert_msg(game->field[1][5] == 1, "game field failed");
  ck_assert_msg(game->field[1][6] == 0, "game field failed");
  game = updateCurrentState(Down);
  ck_assert_msg(game->field[1][2] == 0, "game field failed");
  ck_assert_msg(game->field[1][3] == 0, "game field failed");
  ck_assert_msg(game->field[1][4] == 0, "game field failed");
  ck_assert_msg(game->field[1][5] == 0, "game field failed");
  ck_assert_msg(game->field[2][2] == 1, "game field failed");
  ck_assert_msg(game->field[2][3] == 1, "game field failed");
  ck_assert_msg(game->field[2][4] == 1, "game field failed");
  ck_assert_msg(game->field[2][5] == 1, "game field failed");
  game = updateCurrentState(Right);
  ck_assert_msg(game->field[2][2] == 0, "game field failed");
  ck_assert_msg(game->field[2][3] == 1, "game field failed");
  ck_assert_msg(game->field[2][4] == 1, "game field failed");
  ck_assert_msg(game->field[2][5] == 1, "game field failed");
  ck_assert_msg(game->field[2][6] == 1, "game field failed");
  ck_assert_msg(game->field[2][7] == 0, "game field failed");
  game = updateCurrentState(Action);
  game = updateCurrentState(Action);

  ck_assert_msg(game->field[2][2] == 0, "game field failed");
  ck_assert_msg(game->field[2][3] == 0, "game field failed");
  ck_assert_msg(game->field[2][4] == 0, "game field failed");
  ck_assert_msg(game->field[2][5] == 0, "game field failed");
  ck_assert_msg(game->field[2][6] == 0, "game field failed");
  ck_assert_msg(game->field[2][7] == 0, "game field failed");

  ck_assert_msg(game->field[3][3] == 1, "game field failed");
  ck_assert_msg(game->field[3][4] == 1, "game field failed");
  ck_assert_msg(game->field[3][5] == 1, "game field failed");
  ck_assert_msg(game->field[3][6] == 1, "game field failed");

  game = updateCurrentState(Pause);
  ck_assert_msg(game->field[3][3] == 1, "game field failed");
  ck_assert_msg(game->field[3][4] == 1, "game field failed");
  ck_assert_msg(game->field[3][5] == 1, "game field failed");
  ck_assert_msg(game->field[3][6] == 1, "game field failed");

  game = updateCurrentState(Pause);
  game = updateCurrentState(Down);
  ck_assert_msg(game->field[4][3] == 1, "Pause failed");
  ck_assert_msg(game->field[4][4] == 1, "Pause failed");
  ck_assert_msg(game->field[4][5] == 1, "Pause failed");
  ck_assert_msg(game->field[4][6] == 1, "Pause failed");
  game = updateCurrentState(Terminate);
  ck_assert_msg(game->pause == 2, "exit failed");

  game_over();
  memory_free(game);
}

Suite *test_backend(void) {
  Suite *suite = suite_create("test_backend");
  TCase *tcase_core = tcase_create("test_backend");

  tcase_add_test(tcase_core, test_1);
  tcase_add_test(tcase_core, test_2);
  tcase_add_test(tcase_core, test_3);
  tcase_add_test(tcase_core, test_4);
  tcase_add_test(tcase_core, test_5);
  tcase_add_test(tcase_core, test_6);
  tcase_add_test(tcase_core, test_7);
  tcase_add_test(tcase_core, test_8);
  tcase_add_test(tcase_core, test_9);
  tcase_add_test(tcase_core, test_10);
  tcase_add_test(tcase_core, test_11);
  tcase_add_test(tcase_core, test_12);
  tcase_add_test(tcase_core, test_13);
  tcase_add_test(tcase_core, test_1_fsm);
  tcase_add_test(tcase_core, test_2_fsm);
  tcase_add_test(tcase_core, test_3_fsm);
  tcase_add_test(tcase_core, test_4_fsm);

  suite_add_tcase(suite, tcase_core);
  return suite;
}