/**
 * @file: BrickGameCli.java
 * @description: CLI интерфейс для всех игр BrickGame v3.0
 * @dependencies: src.brick_game.client_lib.*
 * @created: 2025-10-05
 */
package src.brick_game.cli;

import src.brick_game.client_lib.BrickGameClient;
import src.brick_game.client_lib.RestBrickGameClient;
import src.brick_game.client_lib.Action;
import src.brick_game.client_lib.dto.GameState;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.IOException;
import java.time.Duration;
import java.time.Instant;

public class BrickGameCli {
  private final BrickGameClient client;
  private boolean running = true;

  public BrickGameCli() {
    this.client = new RestBrickGameClient("http://localhost:8080/api");
  }

  public void run() {
    System.out.println("=== BrickGame v3.0 CLI ===");
    System.out.println("© Copyright by Directpi");
    System.out.println();

    System.out.println("Available games:");
    System.out.println("1. Racing");
    System.out.println("2. Tetris");
    System.out.println("3. Snake");
    System.out.println();
    System.out.println("Usage: ./gradlew runCliRacing | runCliTetris | runCliSnake");
  }

  public void runWithArgs(String[] args) {
    if (args.length == 0) {
      run();
      return;
    }

    try {
      int gameId = Integer.parseInt(args[0]);
      switch (gameId) {
        case 1:
          playRacing();
          break;
        case 2:
          playTetris();
          break;
        case 3:
          playSnake();
          break;
        default:
          System.out.println("Invalid game ID. Use 1, 2, or 3.");
          break;
      }
    } catch (NumberFormatException e) {
      System.out.println("Invalid argument. Please provide a number (1, 2, or 3).");
    }
  }

  private void playRacing() {
    System.out.println("\n=== Racing (CLI) ===");
    System.out.println("Controls: A/D - Move, W - Accelerate (hold), P - Pause, Q - Quit");
    System.out.println("Tip: ввод построчный. Нажмите клавишу и Enter.");

    try {
      client.startGame(1).join();
      loopRacingCli();
    } catch (Exception e) {
      System.out.println("Error starting Racing: " + e.getMessage());
    }
  }

  private void playTetris() {
    System.out.println("\n=== Tetris Game ===");
    System.out.println("Starting Tetris...");
    try {
      client.startGame(2).join();
      System.out.println("Tetris game started!");
      System.out.println(
          "Controls: ←/→/↓ - Move, ↑/Space - Rotate, P - Pause, Q - Quit (GUI/Web рекомендуются)");
    } catch (Exception e) {
      System.out.println("Error starting Tetris: " + e.getMessage());
    }
  }

  private void playSnake() {
    System.out.println("\n=== Snake Game ===");
    System.out.println("Starting Snake...");
    try {
      client.startGame(3).join();
      System.out.println("Snake game started!");
      System.out.println("Controls: ←/→/↑/↓ - Move, P - Pause, Q - Quit (GUI/Web рекомендуются)");
    } catch (Exception e) {
      System.out.println("Error starting Snake: " + e.getMessage());
    }
  }

  private void loopRacingCli() throws IOException {
    BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
    boolean accelerateHold = false;
    Instant lastFrame = Instant.now();

    while (running) {
      // Обработка ввода (неблокирующая, построчно)
      if (reader.ready()) {
        String line = reader.readLine();
        if (line != null && !line.isEmpty()) {
          char c = Character.toLowerCase(line.charAt(0));
          switch (c) {
            case 'a':
              client.userInput(Action.Left, false).join();
              break;
            case 'd':
              client.userInput(Action.Right, false).join();
              break;
            case 'w':
              accelerateHold = !accelerateHold; // переключаем удержание
              client.userInput(Action.Up, accelerateHold).join();
              break;
            case 'p':
              client.userInput(Action.Pause, false).join();
              break;
            case 'q':
              client.userInput(Action.Terminate, false).join();
              running = false;
              break;
            default:
              break;
          }
        }
      }

      // Получаем состояние и рисуем
      GameState state = safeGetState();
      renderRacingAscii(state);

      // Ограничение FPS ~20
      long sleepMs = 50 - Duration.between(lastFrame, Instant.now()).toMillis();
      if (sleepMs > 0) {
        try { Thread.sleep(sleepMs); } catch (InterruptedException ie) { Thread.currentThread().interrupt(); }
      }
      lastFrame = Instant.now();

      if (state != null && !state.isGameRunning()) {
        // Переходим в ожидание рестарта
        System.out.println("Game Over. Press 'q' + Enter to quit or 'p' + Enter to toggle pause.");
      }
    }
  }

  private GameState safeGetState() {
    try {
      return client.getState().join();
    } catch (Exception e) {
      return null;
    }
  }

  private void renderRacingAscii(GameState state) {
    StringBuilder sb = new StringBuilder();
    sb.append("\033[H\033[2J");
    sb.append("Racing (CLI)  ");
    if (state != null) {
      sb.append("Score: ").append(state.getScore())
        .append("  Hi: ").append(state.getHighScore())
        .append("  Lvl: ").append(state.getLevel())
        .append("  Spd: ").append(state.getSpeed())
        .append(state.isGameRunning() ? "" : "  [GAME OVER]")
        .append(state.isPause() ? "  [PAUSED]" : "");
    }
    sb.append("\n");

    if (state != null && state.getField() != null) {
      boolean[][] field = state.getField();
      for (int y = 0; y < field.length; y++) {
        sb.append('|');
        for (int x = 0; x < field[y].length; x++) {
          sb.append(field[y][x] ? '#' : ' ');
        }
        sb.append('|').append('\n');
      }
    } else {
      sb.append("(no state)\n");
    }

    sb.append("\nControls: A/D - Move, W - Accelerate(toggle), P - Pause, Q - Quit\n");
    System.out.print(sb.toString());
    System.out.flush();
  }

  public static void main(String[] args) {
    new BrickGameCli().runWithArgs(args);
  }
}


