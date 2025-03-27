#ifndef TETRIS_BACK_H
#define TETRIS_BACK_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#include "../common/common_file.h"
#include "../common/defines.h"

// namespace snake {

#ifdef __cplusplus
extern "C" {
#endif

/* FSM */
GameInfo_t *updateCurrentState(UserAction_t action);
typedef void (*action)();
void fsm_tetris();

#ifdef __cplusplus
}
#endif
typedef enum {
  I_FIGURE = 0,
  J_FIGURE,
  L_FIGURE,
  O_FIGURE,
  S_FIGURE,
  T_FIGURE,
  Z_FIGURE
} g_figures;

// Tetris figure state
typedef enum { pos_1 = 0, pos_2, pos_3, pos_4 } figure_state;

typedef struct {
  g_figures current, next;
  figure_state rotate;
  int start_x;
  int start_y;
} Gfigure_t;

void spawn();
void game_move_down();
void game_move_left();
void game_move_right();
void game_rotate();
void game_pause();
void game_exit();
void game_over();

/* Backend */
void game_info_init(GameInfo_t *game_info);
int game_field_mem_alloc(GameInfo_t *game_info);
GameState_t draw_figure(GameInfo_t *game, const Gfigure_t *figure,
                        const UserAction_t *user_action, int create);
void draw_next(GameInfo_t *game, const Gfigure_t *figure);
GameState_t check_matrix(const GameInfo_t *game,
                         const UserAction_t *user_action);
int get_bit(int num, int position);
void clear_field(int **matrix);
void copy_field(GameInfo_t *game);
g_figures select_figure();
void plus_level(GameInfo_t *game);
int full_line(GameInfo_t *game);
void check_high_score(GameInfo_t *game, GameState_t *g_state);
void move_lines_down(GameInfo_t *game, int num);
void memory_free(GameInfo_t *game_info);
long long current_timestamp();

// }  // namespace snake
#endif
