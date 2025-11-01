/**
 * @file: GameFsm.java
 * @description: Контракт FSM для игр BrickGame, соответствующий library-specification_RUS.md
 * @dependencies: src.brick_game.client_lib.Action
 * @created: 2025-10-05
 */
package src.brick_game.race.fsm;

import src.brick_game.client_lib.Action;

/**
 * Интерфейс FSM, полностью соответствующий спецификации library-specification_RUS.md
 */
public interface GameFsm {

  /**
   * Обработка пользовательского ввода согласно спецификации
   * 
   * @param action - действие пользователя
   * @param hold - зажата ли клавиша
   */
  void userInput(Action action, boolean hold);

  /**
   * Получение текущего состояния для отрисовки согласно спецификации
   * 
   * @return State с полями: field, next, score, highScore, level, speed, pause
   */
  State updateCurrentState();

  /**
   * Структура состояния по спецификации
   */
  record State(
      boolean[][] field,
      boolean[][] next,
      int score,
      int highScore,
      int level,
      int speed,
      boolean pause) {
  }

  // Дополнительные методы для внутренней логики
  void startGame(int gameId);

  void applyAction(int actionId, boolean hold);

  GameSnapshot getSnapshot();

  void reset();

  boolean isGameRunning();

  /** Снимок состояния для внутренней логики (расширенный) */
  final class GameSnapshot {
    public boolean[][] field;
    public boolean[][] next;
    public int[][] colors; // Цвета клеток: 0=пусто, 1=игрок, 2-4=соперники
    public int score;
    public int highScore;
    public int level;
    public int speed;
    public int movementSpeed; // Единая скорость движения для машин и краев дороги
    public boolean pause;
    public Integer pauseState; // 0=игра, 1=пауза, 2=выход, 3=game over (для Tetris/Snake)
    public int tickCounter; // Счетчик тиков FSM для синхронизации
  }
}
