#ifndef tetris_back_H
#define tetris_back_H
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#include "../../common/common_file.h"
#include "../../common/defines.h"

/* FSM */
GameInfo_t *updateCurrentState(UserAction_t u_action);
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
void memory_free(GameInfo_t *game_info);
GameState_t draw_figure(GameInfo_t *game, Gfigure_t *figure,
                        UserAction_t *user_action, int create);
void draw_next(GameInfo_t *game, Gfigure_t *figure);
GameState_t check_matrix(GameInfo_t *game, const UserAction_t *user_action);
int get_bit(int num, int position);
void clear_field(int **matrix);
void copy_field(GameInfo_t *game);
g_figures select_figure();
void plus_level(GameInfo_t *game);
int full_line(GameInfo_t *game);
void check_high_score(GameInfo_t *game, GameState_t *g_state);
void move_lines_down(GameInfo_t *game, int num);
int read_file_high_score(void);
int write_to_file_high_score(int score);
long long current_timestamp();
#endif