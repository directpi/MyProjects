/**
 * @file: CompatibleExample.java
 * @description: Пример использования совместимого API согласно спецификации
 * @dependencies: src.brick_game.race.fsm.*, src.brick_game.client_lib.*
 * @created: 2025-10-05
 */
package src.brick_game.client_lib.examples;

import src.brick_game.race.fsm.GameFsm;
import src.brick_game.race.fsm.RacingFsm;
import src.brick_game.race.fsm.GameFsm.State;
import src.brick_game.client_lib.Action;
import src.brick_game.client_lib.ColorGenerator;

/**
 * Пример использования API по спецификации library-specification_RUS.md
 */
public class CompatibleExample {

  private final GameFsm game;
  private final ColorGenerator colorGenerator;

  public CompatibleExample() {
    this.game = new RacingFsm();
    this.colorGenerator = new ColorGenerator();
  }

  /**
   * Основной игровой цикл согласно спецификации
   */
  public void runGame() {
    System.out.println("=== Compatible Game Example ===");
    System.out.println("Using library-specification_RUS.md API");

    // Начало игры
    game.userInput(Action.Start, false);

    // Игровой цикл
    for (int i = 0; i < 100; i++) {
      // Получаем состояние по спецификации
      State state = game.updateCurrentState();

      // Отрисовываем поле
      drawField(state);

      // Симуляция пользовательского ввода
      if (i % 10 == 0) {
        game.userInput(Action.Left, false);
      } else if (i % 15 == 0) {
        game.userInput(Action.Right, false);
      } else if (i % 20 == 0) {
        game.userInput(Action.Up, true); // Ускорение
      }

      // Пауза между кадрами
      try {
        Thread.sleep(100);
      } catch (InterruptedException e) {
        Thread.currentThread().interrupt();
        break;
      }
    }

    // Завершение игры
    game.userInput(Action.Terminate, false);
  }

  /**
   * Отрисовка игрового поля с цветами
   */
  private void drawField(State state) {
    System.out.println("\n--- Game State ---");
    System.out.println("Score: " + state.score() + " | High: " + state.highScore() + " | Level: "
        + state.level() + " | Speed: " + state.speed() + " | Pause: " + state.pause());

    // Отрисовка поля с цветами
    for (int y = 0; y < state.field().length; y++) {
      for (int x = 0; x < state.field()[y].length; x++) {
        if (state.field()[y][x]) {
          int color = colorGenerator.getColorForCell(state.field(), x, y);
          char symbol = getColorSymbol(color);
          System.out.print(symbol);
        } else {
          System.out.print('.');
        }
      }
      System.out.println();
    }
  }

  /**
   * Получить символ для цвета
   */
  private char getColorSymbol(int color) {
    return switch (color) {
      case ColorGenerator.COLOR_PLAYER -> 'P';
      case ColorGenerator.COLOR_OPPONENT_1 -> '1';
      case ColorGenerator.COLOR_OPPONENT_2 -> '2';
      case ColorGenerator.COLOR_OPPONENT_3 -> '3';
      default -> 'X';
    };
  }

  public static void main(String[] args) {
    new CompatibleExample().runGame();
  }
}
