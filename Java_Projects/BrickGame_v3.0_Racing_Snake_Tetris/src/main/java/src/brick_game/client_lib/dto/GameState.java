/**
 * @file: GameState.java
 * @description: Клиентский DTO состояния игры для десериализации ответов
 * @dependencies: lombok.Data, com.fasterxml.jackson.annotation.JsonProperty
 * @created: 2025-10-05
 */
package src.brick_game.client_lib.dto;

import com.fasterxml.jackson.annotation.JsonProperty;
import lombok.Data;

@Data
public class GameState {
  private boolean[][] field;
  private boolean[][] next;
  private int[][] colors; // Цвета клеток: 0=пусто, 1=игрок, 2-4=соперники
  private int score;

  @JsonProperty("high_score")
  private int highScore;

  private int level;
  private int speed;

  @JsonProperty("movement_speed")
  private int movementSpeed; // Единая скорость движения для машин и краев дороги

  private boolean pause;

  @JsonProperty("pause_state")
  private Integer pauseState; // 0=игра, 1=пауза, 2=выход, 3=game over (для Tetris/Snake)

  @JsonProperty("game_running")
  private boolean gameRunning;

  @JsonProperty("tick_counter")
  private int tickCounter; // Счетчик тиков FSM для синхронизации
}
