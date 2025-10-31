#include "s_tetris_back.h"

// namespace s21{

static GameInfo_t game;
static GameState_t g_state;
static Gfigure_t figure;
static UserAction_t user_action;

void fsm_tetris() {
  action fsm_table[8][8] = {
      {spawn, NULL, game_exit, NULL, NULL, NULL, NULL, NULL},
      {spawn, NULL, game_exit, spawn, spawn, NULL, spawn, spawn},
      {NULL, game_pause, game_exit, game_move_left, game_move_right, NULL,
       game_move_down, game_rotate},
      {NULL, game_pause, game_exit, game_move_left, game_move_right, NULL, NULL,
       game_rotate},
      {spawn, game_pause, game_exit, spawn, spawn, spawn, spawn, spawn},
      {spawn, NULL, game_exit, NULL, NULL, NULL, NULL, NULL},
      {NULL, game_pause, game_exit, NULL, NULL, NULL, NULL, NULL},
      {game_exit, game_exit, game_exit, game_exit, game_exit, game_exit,
       game_exit, game_exit},
  };
  action act;
  act = fsm_table[g_state][user_action];
  if (act) act();
}
/* Example: figure "J" rotation 0
 * 0b1000111000000000
 *
 * 1 0 0 0
 * 1 1 1 0
 * 0 0 0 0
 * 0 0 0 0
 */
int bit_figure[][4] = {
    {0b0000111100000000, 0b0010001000100010, 0b0000000011110000,
     0b0100010001000100},  // I
    {0b1000111000000000, 0b0110010001000000, 0b0000111000100000,
     0b0100010011000000},  // J
    {0b0010111000000000, 0b0100010001100000, 0b0000111010000000,
     0b1100010001000000},  // L
    {0b0110011000000000, 0b0110011000000000, 0b0110011000000000,
     0b0110011000000000},  // O
    {0b0110110000000000, 0b0100011000100000, 0b0000011011000000,
     0b1000110001000000},  // S
    {0b0100111000000000, 0b0100011001000000, 0b0000111001000000,
     0b0100110001000000},  // T
    {0b1100011000000000, 0b0010011001000000, 0b0000110001100000,
     0b0100110010000000}  // Z
};

/**
 * @brief Initialize game_info structure
 * @param game_info GameInfo_t structure
 */
void game_info_init(GameInfo_t *game_info) {
  game_info->level = 1;
  game_info->score = 0;
  game_info->high_score = read_file_high_score(1);  // read_high_score();
  game_info->speed = GAME_SPEED;
  game_info->pause = 0;
  game_field_mem_alloc(game_info);
}

int game_field_mem_alloc(GameInfo_t *game_info) {
  int error_code = SUCCESS;
  game_info->field = (int **)calloc(20, sizeof(int *));
  if (game_info->field) {
    for (int i = 0; i < 20 && error_code == SUCCESS; i++) {
      game_info->field[i] = (int *)calloc(10, sizeof(int));
      if (!game_info->field[i]) error_code = ERROR;
    }
  } else {
    error_code = ERROR;
  }
  if (!error_code) {
    game_info->next = (int **)calloc(20, sizeof(int *));
    if (game_info->next) {
      for (int i = 0; i < 20 && error_code == SUCCESS; i++) {
        game_info->next[i] = (int *)calloc(10, sizeof(int));
        if (!game_info->next[i]) {
          error_code = ERROR;
        }
      }
    } else {
      error_code = ERROR;
    }
  }
  return error_code;
}

/*
 * @brief Memory free function
 * @param GameInfo_t *game
 */
void memory_free(GameInfo_t *game_info) {
  if (game_info->field) {
    for (int i = 0; i < HEIGHT; i++) {
      free(game_info->field[i]);
    }
    free(game_info->field);
  }
  if (game_info->next) {
    for (int i = 0; i < HEIGHT; i++) {
      free(game_info->next[i]);
    }
    free(game_info->next);
  }
  game_info->field = NULL;
  game_info->next = NULL;
}

/*
 * @brief "Draw" current figure in matrix
 * @param GameInfo_t *game
 * @param Gfigure_t *figure
 * @paramUserAction_t *user_action
 * @paramint create
 */
GameState_t draw_figure(GameInfo_t *game, const Gfigure_t *figure,
                        const UserAction_t *user_action, int create) {
  GameState_t state_code = MOVING_STATE;
  int position = 15;
  for (int i = figure->start_y; i < figure->start_y + 4; i++) {
    for (int j = figure->start_x; j < figure->start_x + 4; j++) {
      int bit = get_bit(bit_figure[figure->current][figure->rotate], position);
      if ((i > 19) && bit) {
        state_code = ATTACHING_STATE;
      } else if ((j > 9 || j < 0) && bit) {
        state_code = SHIFTING_STATE;
      } else if ((i > 19 || j > 9 || j < 0) && !bit) {
        ;
      } else if (bit) {
        if (create) {
          game->next[i][j] = bit;
        } else {
          game->field[i][j] = 0;
        }
      }
      position--;
    }
  }
  if (state_code == MOVING_STATE) {
    state_code = check_matrix(game, user_action);
  }
  if (state_code != MOVING_STATE) {
    clear_field(game->next);
  }
  return state_code;
}

/*
 * @brief "Draw" next figure in matrix
 * @param GameInfo_t *game
 * @param Gfigure_t *figure
 */
void draw_next(GameInfo_t *game, const Gfigure_t *figure) {
  int position = 15;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      int bit = get_bit(bit_figure[figure->next][0], position);
      game->next[i][j] = bit;
      position--;
    }
  }
}

/*
 * @brief Check matrix for collision
 * @param GameInfo_t *game
 * @param UserAction_t *user_action
 * @returns GameState_t state_code
 */
GameState_t check_matrix(const GameInfo_t *game,
                         const UserAction_t *user_action) {
  GameState_t state_code = MOVING_STATE;
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      if (game->field[i][j] && game->next[i][j] &&
          (*user_action == Down || *user_action == Start)) {
        state_code = ATTACHING_STATE;
      } else if (game->field[i][j] && game->next[i][j] &&
                 (*user_action == Left || *user_action == Right ||
                  *user_action == Action)) {
        state_code = SHIFTING_STATE;
      }
    }
  }
  return state_code;
}

/*
 * @brief Get bit from figure mask
 * @param int num
 * @param int position(15-0)
 */
int get_bit(int num, int position) {
  int mask = 1 << position;
  return !!(num & mask);
}

/*
 * @brief Clear matrix
 * @param int **matrix
 */
void clear_field(int **matrix) {
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      matrix[i][j] = 0;
    }
  }
}

/*
 * @brief Copy game.next to game.field
 * @param GameInfo_t *game
 */
void copy_field(GameInfo_t *game) {
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      if (game->next[i][j]) {
        game->field[i][j] = game->next[i][j];
      }
    }
  }
}

/*
 * @brief Random select figure function
 */
g_figures select_figure() {
  int n = 0;
  srand(time(NULL));
  n = rand() % 7;
  return (g_figures)n;
}
int full_line(GameInfo_t *game) {
  int full_line_count = 0;
  for (int i = 0; i < HEIGHT; i++) {
    int line = 1;
    for (int j = 0; j < WIDTH; j++) {
      if (!game->field[i][j]) line = 0;
    }
    if (line) {
      move_lines_down(game, i);
      full_line_count++;
    }
  }
  if (full_line_count == 1) {
    game->score += 100;
  } else if (full_line_count == 2) {
    game->score += 300;
  } else if (full_line_count == 3) {
    game->score += 700;
  } else if (full_line_count >= 4) {
    game->score += 1500;
  }
  return full_line_count;
}

/*
 * @brief Level Up function
 * @param GameInfo_t *game
 */
void plus_level(GameInfo_t *game) {
  if (game->level < 10 && game->score >= 600) {
    game->speed = 500 - 50 * (game->score / 600);
    game->level = 1 + game->score / 600;
  }
}

/*
 * @brief Compare score with high_score, and if it gtater then high score write
 * score to file
 * @param GameInfo_t *game
 * @param GameState_t *g_state
 */
void check_high_score(GameInfo_t *game, GameState_t *g_state) {
  int high_score = read_file_high_score(1);
  if (high_score != -1) {
    if (game->score > high_score) {
      write_to_file_high_score(1, game->score);
      game->high_score = game->score;
    }
  } else {
    *g_state = EXIT_STATE;
  }
}
/*
 * @brief Move full line down in game matrix
 * @param GameInfo_t *game
 * @param num
 */
void move_lines_down(GameInfo_t *game, int num) {
  for (int i = num; i > 0; i--) {
    for (int j = 0; j < 10; j++) {
      game->field[i][j] = game->field[i - 1][j];
    }
  }
}

/*
 * @brief Read high score from high_score_file
 * @returms high score
 */
int read_file_high_score(int flag) {
  const char *tetris_file = "tetris_record_score.txt";
  const char *snake_file = "snake_record_score.txt";
  const char *filename = NULL_PTR;

  if (flag == 1) {
    filename = tetris_file;
  } else if (flag == 2) {
    filename = snake_file;
  }
  FILE *myfile = fopen(filename, "r");
  if (!myfile) {
    // Если файл не существует, создаём его с записью начального значения
    write_to_file_high_score(flag, 0);
    myfile = fopen(filename, "r");
  }
  int high_score = 0;
  if (myfile) {
    if (fscanf(myfile, "%d", &high_score) != 1) {
      printf("Failed to read high score from %s\n", filename);
      high_score = 0;  // Устанавливаем значение по умолчанию
    }
    fclose(myfile);
  } else {
    printf("File %s could not be opened\n", filename);
    high_score = -1;  // Указываем на ошибку
  }
  return high_score;
}

/*
 * @brief Write score to high_score_file
 * @param score
 */
void write_to_file_high_score(int flag, int score) {
  const char *tetris_file = "tetris_record_score.txt";
  const char *snake_file = "snake_record_score.txt";
  const char *filename = NULL_PTR;

  if (flag == 1) {
    filename = tetris_file;
  } else if (flag == 2) {
    filename = snake_file;
  }
  FILE *myfile = NULL;
  myfile = fopen(filename, "w");
  if (myfile) {
    fprintf(myfile, "%d", score);
    fclose(myfile);
  } else {
    printf("File %s does not exist\n", filename);
  }
}

/*
 * @brief Get timeofday
 * @returns milliseconds
 */
long long current_timestamp() {
  struct timeval te;
  gettimeofday(&te, NULL);
  long long miliseconds = te.tv_sec * 1000LL + te.tv_usec / 1000;
  return miliseconds;
}

/* FSM function */
/*
 * @brief Calls function from the condition matrix, based on user input
 * @param user_action
 * @returns GameInfo_t game
 */
GameInfo_t *updateCurrentState(UserAction_t u_action) {
  if (u_action == Start && (!game.field)) {
    g_state = START_STATE;
    game_info_init(&game);
  }
  if (u_action == Start && game.pause == (int)GAMEOVER_STATE) {
    game.pause = (int)START_STATE;
  }
  clear_field(game.next);
  fsm_tetris();
  user_action = u_action;
  copy_field(&game);
  clear_field(game.next);
  if (g_state == SPAWN_STATE && full_line(&game)) {
    plus_level(&game);
    check_high_score(&game, &g_state);
  }
  draw_next(&game, &figure);
  if (g_state == GAMEOVER_STATE) {
    game_over();
  }
  return &game;
}

/*
 * @brief Spawn figure function
 */
void spawn() {
  if (g_state == START_STATE) {
    figure.current = I_FIGURE;
  } else {
    figure.current = figure.next;
  }
  figure.next = select_figure();
  figure.rotate = pos_1;
  figure.start_x = 3;
  figure.start_y = 0;
  g_state = draw_figure(&game, &figure, &user_action, 1);
  if (g_state == ATTACHING_STATE || g_state == SHIFTING_STATE) {
    g_state = GAMEOVER_STATE;

  } else {
    g_state = MOVING_STATE;
  }
}

/*
 * @brief Move down figure function
 */
void game_move_down() {
  draw_figure(&game, &figure, &user_action, 0);
  figure.start_y++;
  g_state = draw_figure(&game, &figure, &user_action, 1);

  if (g_state == ATTACHING_STATE) {  //
    g_state = SPAWN_STATE;
    figure.start_y--;
    draw_figure(&game, &figure, &user_action, 1);
  } else {
    g_state = MOVING_STATE;
  }
}

/*
 * @brief Move left figure function
 */
void game_move_left() {
  draw_figure(&game, &figure, &user_action, 0);
  figure.start_x--;
  g_state = draw_figure(&game, &figure, &user_action, 1);
  if (g_state == SHIFTING_STATE) {
    figure.start_x++;
    draw_figure(&game, &figure, &user_action, 1);
    g_state = MOVING_STATE;
  }
}

/*
 * @brief Move right figure function
 */
void game_move_right() {
  draw_figure(&game, &figure, &user_action, 0);
  figure.start_x++;
  g_state = draw_figure(&game, &figure, &user_action, 1);
  if (g_state == SHIFTING_STATE) {
    figure.start_x--;
    draw_figure(&game, &figure, &user_action, 1);
    g_state = MOVING_STATE;
  }
}

/*
 * @brief Rotate figure function
 */
void game_rotate() {
  draw_figure(&game, &figure, &user_action, 0);
  if (figure.rotate == pos_4) {
    figure.rotate = pos_1;
  } else {
    figure.rotate = (figure_state)((int)figure.rotate + 1);
  }
  g_state = draw_figure(&game, &figure, &user_action, 1);
  if (g_state == SHIFTING_STATE || g_state == ATTACHING_STATE) {
    if (figure.rotate == pos_1) {
      figure.rotate = pos_4;
    } else {
      figure.rotate = (figure_state)((int)figure.rotate - 1);
    }
    draw_figure(&game, &figure, &user_action, 1);
    g_state = MOVING_STATE;
  }
}

/*
 * @brief Exit game function
 */
void game_exit() {
  game.pause = (int)EXIT_STATE;  // Escape
  g_state = EXIT_STATE;
}

/*
 * @brief pause game function
 */
void game_pause() {
  if (g_state != (int)PAUSE_STATE) {
    game.pause = (int)PAUSE_STATE;  // Game pause
    g_state = PAUSE_STATE;
  } else {
    game.pause = (int)START_STATE;  // Game pause
    g_state = MOVING_STATE;
  }
}

/*
 * @brief Game over function
 */
void game_over() {
  game.pause = (int)GAMEOVER_STATE;  // Game over
  g_state = START_STATE;
}
// } // namespace s21
