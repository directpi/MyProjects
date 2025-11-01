/**
 * @file: ConsoleExample.java
 * @description: Пример использования клиентской библиотеки в консоли
 * @dependencies: src.brick_game.client_lib.RestBrickGameClient
 * @created: 2025-10-05
 */
package src.brick_game.client_lib.examples;

import java.util.Scanner;
import src.brick_game.client_lib.BrickGameClient;
import src.brick_game.client_lib.RestBrickGameClient;
import src.brick_game.client_lib.dto.GameState;

public class ConsoleExample {
  public static void main(String[] args) throws Exception {
    BrickGameClient client = new RestBrickGameClient("http://localhost:8080/api");

    // Запуск игры Racing (id=1 как пример)
    client.startGame(1).join();

    Scanner scanner = new Scanner(System.in);
    System.out.println("Use A/D to move left/right, W to accelerate (hold), Q to quit");
    boolean hold = false;
    while (true) {
      if (System.in.available() > 0) {
        String line = scanner.nextLine();
        if (line.equalsIgnoreCase("q"))
          break;
        if (line.equalsIgnoreCase("a"))
          client.sendAction(1, false).join();
        if (line.equalsIgnoreCase("d"))
          client.sendAction(2, false).join();
        if (line.equalsIgnoreCase("w"))
          hold = !hold;
      }

      client.sendAction(3, hold).join();
      GameState state = client.getState().join();
      System.out.printf("score=%d high=%d level=%d speed=%d\n", state.getScore(),
          state.getHighScore(), state.getLevel(), state.getSpeed());
      Thread.sleep(500); // Увеличиваем задержку
    }
    scanner.close();
  }
}
