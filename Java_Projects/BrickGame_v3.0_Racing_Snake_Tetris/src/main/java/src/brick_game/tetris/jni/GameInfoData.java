/**
 * @file: GameInfoData.java
 * @description: Java представление C структуры GameInfo_t
 * @dependencies: common_file.h
 * @created: 2025-10-05
 */
package src.brick_game.tetris.jni;

/**
 * Java класс, соответствующий C структуре GameInfo_t. Используется для передачи данных между JNI и
 * Java кодом.
 */
public class GameInfoData {
  /** Игровое поле 20x10 (1 = заполнено, 0 = пусто) */
  public int[][] field;

  /** Следующая фигура 4x4 */
  public int[][] next;

  /** Текущий счёт */
  public int score;

  /** Рекорд */
  public int highScore;

  /** Текущий уровень */
  public int level;

  /** Скорость игры (миллисекунды) */
  public int speed;

  /** Флаг паузы (1 = пауза, 0 = игра) */
  public int pause;

  public GameInfoData() {
    this.field = new int[20][10];
    this.next = new int[4][4];
  }

  @Override
  public String toString() {
    return String.format("GameInfo{score=%d, level=%d, speed=%d, pause=%d}", score, level, speed,
        pause);
  }
}
