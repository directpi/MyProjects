/**
 * @file: GameStateDto.java
 * @description: DTO текущего состояния игры
 * @dependencies: lombok.Data
 * @created: 2025-10-05
 */
package src.brick_game.server.api.dto;

import lombok.Data;

@Data
public class GameStateDto {
  private boolean[][] field;
  private boolean[][] next;
  private int[][] colors; // Цвета клеток: 0=пусто, 1=игрок, 2-4=соперники
  private int score;
  private int high_score;
  private int level;
  private int speed;
  private int movementSpeed; // Единая скорость движения для машин и краев дороги
  private boolean pause;
  private boolean gameRunning;
  private int tickCounter; // Счетчик тиков FSM для синхронизации
}
