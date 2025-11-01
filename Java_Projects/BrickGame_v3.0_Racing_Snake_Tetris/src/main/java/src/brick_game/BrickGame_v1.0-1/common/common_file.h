#ifndef COMMON_H
#define COMMON_H

typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

typedef enum {
  START_STATE = 0,
  SPAWN_STATE,
  MOVING_STATE,
  SHIFTING_STATE,
  ATTACHING_STATE,
  GAMEOVER_STATE,
  PAUSE_STATE,
  EXIT_STATE
} GameState_t;

typedef enum {
  I_FIGURE = 0,
  J_FIGURE,
  L_FIGURE,
  O_FIGURE,
  S_FIGURE,
  T_FIGURE,
  Z_FIGURE
} g_figures;

typedef enum { pos_1 = 0, pos_2, pos_3, pos_4 } figure_state;

typedef struct g_figures {
  g_figures current, next;
  figure_state rotate;
  int start_x;
  int start_y;
} Gfigure_t;

#endif
