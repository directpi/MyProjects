/**
 * @file: NativeGameAdapter.java
 * @description: Адаптер для интеграции нативных C/C++ игр в Java GUI
 * @dependencies: src.brick_game.race.fsm.GameFsm
 * @created: 2025-10-05
 */
package src.brick_game.race.fsm;

import java.io.*;
import java.util.concurrent.CompletableFuture;
import src.brick_game.client_lib.Action;

public class NativeGameAdapter implements GameFsm {
  private Process nativeProcess;
  private final String gameScript;
  private final int gameId;
  private GameSnapshot currentSnapshot;
  private boolean gameRunning = false;
  private boolean paused = false;
  private int score = 0;
  private int level = 1;
  private int speed = 1;
  private int highScore = 0;
  private int tickCounter = 0;
  private boolean[][] field = new boolean[20][10];
  private int[][] colors = new int[20][10];
  private int movementSpeed = 1;

  public NativeGameAdapter(int gameId, String gameScript) {
    this.gameId = gameId;
    this.gameScript = gameScript;
    this.currentSnapshot = new GameSnapshot();
    updateSnapshot();
  }

  @Override
  public void startGame(int gameId) {
    try {
      if (nativeProcess != null && nativeProcess.isAlive()) {
        nativeProcess.destroy();
      }

      // Запускаем нативную игру
      ProcessBuilder pb = new ProcessBuilder("bash", "-c", gameScript);
      pb.directory(new File("/home/directpi/TEMP/BrickGame_v3.0_Jv/BrickGame"));
      nativeProcess = pb.start();

      gameRunning = true;
      paused = false;
      score = 0;
      level = 1;
      speed = 1;
      tickCounter = 0;

      // Инициализируем поле
      for (int y = 0; y < 20; y++) {
        for (int x = 0; x < 10; x++) {
          field[y][x] = false;
          colors[y][x] = 0;
        }
      }

      updateSnapshot();
    } catch (Exception e) {
      throw new RuntimeException("Failed to start native game: " + e.getMessage());
    }
  }

  @Override
  public void applyAction(int actionId, boolean hold) {
    if (!gameRunning || paused)
      return;

    // Отправляем команды в нативную игру через stdin
    if (nativeProcess != null && nativeProcess.isAlive()) {
      try {
        PrintWriter writer = new PrintWriter(nativeProcess.getOutputStream());
        switch (actionId) {
          case 1: // PAUSE
            paused = !paused;
            break;
          case 2: // DOWN
            writer.println("DOWN");
            break;
          case 3: // LEFT
            writer.println("LEFT");
            break;
          case 4: // RIGHT
            writer.println("RIGHT");
            break;
          case 5: // ACCELERATE
            if (hold) {
              writer.println("ACCELERATE");
            }
            break;
        }
        writer.flush();
      } catch (Exception e) {
        // Игнорируем ошибки ввода
      }
    }
  }

  @Override
  public GameSnapshot getSnapshot() {
    updateSnapshot();
    return currentSnapshot;
  }

  @Override
  public boolean isGameRunning() {
    return gameRunning && nativeProcess != null && nativeProcess.isAlive();
  }

  private void updateSnapshot() {
    currentSnapshot.field = deepCopy(field);
    currentSnapshot.next = new boolean[4][4]; // Заглушка для next фигуры
    currentSnapshot.colors = deepCopyColors(colors);
    currentSnapshot.score = score;
    currentSnapshot.highScore = highScore;
    currentSnapshot.level = level;
    currentSnapshot.speed = speed;
    currentSnapshot.pause = paused;
    currentSnapshot.movementSpeed = movementSpeed;
    currentSnapshot.tickCounter = tickCounter;

    // Увеличиваем счетчик тиков
    tickCounter++;
  }

  private boolean[][] deepCopy(boolean[][] src) {
    if (src == null)
      return null;
    boolean[][] dst = new boolean[src.length][];
    for (int i = 0; i < src.length; i++) {
      dst[i] = new boolean[src[i].length];
      System.arraycopy(src[i], 0, dst[i], 0, src[i].length);
    }
    return dst;
  }

  private int[][] deepCopyColors(int[][] src) {
    if (src == null)
      return null;
    int[][] dst = new int[src.length][];
    for (int i = 0; i < src.length; i++) {
      dst[i] = new int[src[i].length];
      System.arraycopy(src[i], 0, dst[i], 0, src[i].length);
    }
    return dst;
  }

  @Override
  public void reset() {
    stopGame();
    score = 0;
    level = 1;
    speed = 1;
    tickCounter = 0;
    paused = false;
    gameRunning = false;

    // Очищаем поле
    for (int y = 0; y < 20; y++) {
      for (int x = 0; x < 10; x++) {
        field[y][x] = false;
        colors[y][x] = 0;
      }
    }
    updateSnapshot();
  }

  public void stopGame() {
    if (nativeProcess != null && nativeProcess.isAlive()) {
      nativeProcess.destroy();
    }
    gameRunning = false;
  }

  // Методы для соответствия спецификации library-specification_RUS.md

  @Override
  public void userInput(Action action, boolean hold) {
    // Преобразуем Action в actionId для внутренней логики
    int actionId = action.getId();
    applyAction(actionId, hold);
  }

  @Override
  public State updateCurrentState() {
    updateSnapshot();
    // Создаем State точно по спецификации
    return new State(currentSnapshot.field, currentSnapshot.next, currentSnapshot.score,
        currentSnapshot.highScore, currentSnapshot.level, currentSnapshot.speed,
        currentSnapshot.pause);
  }
}
