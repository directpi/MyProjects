#ifndef GUI_H
#define GUI_H

#include <curses.h>

#include <chrono>

#include "../../brick_game/common/common_file.h"
#include "../../brick_game/common/defines.h"
#include "../../brick_game/snake/controller/s21_controller.hpp"
#include "../../brick_game/snake/model/s21_model.hpp"
#include "../../brick_game/racing/s21_racing_rest_client.hpp"
extern "C" {
#include "../../brick_game/tetris/s21_tetris_back.h"
}
namespace s21 {
// Перечисление для выбора игры
enum MenuChoice { BRICK_GAME = 1, SNAKE, RACING, QUIT };

class SnakeView {
  public:
   SnakeView();
   ~SnakeView();
   static void draw();
   static void drawField();
   static void drawScore();
};

namespace RacingView {
  void draw(bool field[HEIGHT][WIDTH], GameInfo_t *meta);
  void drawField(bool field[HEIGHT][WIDTH]);
  void drawScore(GameInfo_t *meta);
  }   
void init_library();
void init_tetris_game();
void print_game_over(GameInfo_t *game, bool winFlag);
void print_pause(bool flag);
void draw_menu(int choice);
void print_playground(const GameInfo_t *game);
int show_menu();
void init_windows();
void cleanup_windows();
void main_brick_game_loop();
void main_snake_game_loop();
/**
 * @brief Main racing game loop for CLI interface
 * @description Implements racing game logic with REST API communication
 * @dependencies RacingRestClient, ncurses
 */
void main_racing_game_loop();

/**
 * @brief Draw racing field with cars and obstacles
 * @param game_info Pointer to game metadata
 * @description Renders the racing field using ncurses
 */
void drawRacingField(GameInfo_t *game_info);

/**
 * @brief Draw racing score and game information
 * @param game_info Pointer to game metadata
 * @description Displays score, level, speed and controls
 */
void drawRacingScore(GameInfo_t *game_info);

/**
 * @brief Draw racing score with custom speed display
 * @param game_info Pointer to game metadata
 * @param displaySpeed Custom speed value to display
 * @description Helper for UI overlay speed (not part of public API)
 */
void drawRacingScoreWith(GameInfo_t *game_info, int displaySpeed);

/**
 * @brief Draw racing field using simple rendering
 * @param field 2D array representing game field
 * @description Renders cars and obstacles on the field
 */
void drawRacingFieldSimple(bool field[HEIGHT][WIDTH]);

/**
 * @brief Draw simple racing score display
 * @param score Current game score
 * @param level Current game level
 * @description Simplified score display for racing game
 */
void drawRacingScoreSimple(int score, int level);

/**
 * @brief Convert user input to game action
 * @param user_input Raw input from keyboard
 * @return UserAction_t Corresponding game action
 * @description Maps keyboard input to game actions
 */
UserAction_t get_signal(int user_input);

/**
 * @brief Handle game over screen and user input
 * @param game_info Pointer to game metadata
 * @param fileFlag Flag for file operations
 * @param winFlag Flag indicating win condition
 * @return int User choice (restart/exit)
 * @description Manages game over state and restart logic
 */
int game_over_loop(GameInfo_t *game_info, int fileFlag, bool winFlag);

}  // namespace s21
#endif