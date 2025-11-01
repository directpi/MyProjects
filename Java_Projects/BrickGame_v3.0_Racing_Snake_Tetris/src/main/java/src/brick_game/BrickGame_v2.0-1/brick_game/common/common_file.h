#ifndef CPP3_BRICKGAME_V2_0_1_SRC_BRICK_GAME_COMMON_
#define CPP3_BRICKGAME_V2_0_1_SRC_BRICK_GAME_COMMON_

#ifdef __cplusplus
extern "C" {
#endif

// Общие включения и определения для C и C++
#include <stddef.h>  // (для NULL)
#include <stdlib.h>

// Общие макросы для C и C++
#ifdef __cplusplus
#define NULL_PTR nullptr
#else
#define NULL_PTR NULL
#endif

// Структуры и перечисления, доступные для C и C++
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
  Start = 0,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

typedef enum {
  START_STATE = 0,  // Начальное состояние.
  SPAWN_STATE = 1,  // Состояние появления фигуры или змейки и яблока
  MOVING_STATE = 2,  //  Состояние движения фигуры или змейки.
  SHIFTING_STATE = 3,   // Состояние сдвига.
  ATTACHING_STATE = 4,  // Состояние столкновения фигуры или прикрепления частей
                        // змейки после поедания яблока.
  GAMEOVER_STATE = 5,  // Конец Игры
  PAUSE_STATE = 6,     // Пауза
  EXIT_STATE = 7,      // Выход из игры
  WIN_STATE = 8  // Победа, когда змейка достигла максимальной длины
} GameState_t;

// Функции, доступные как для C, так и для C++

int read_file_high_score(int flag);
void write_to_file_high_score(int flag, int score);

#ifdef __cplusplus
}  // Завершение extern "C"
#endif

#ifdef __cplusplus
// Включения для C++
#include <iostream>
#include <random>
#include <stdexcept>
#include <utility>

// Дополнительные структуры для C++

#endif

#endif  // CPP3_BRICKGAME_V2_0_1_SRC_BRICK_GAME_COMMON_
