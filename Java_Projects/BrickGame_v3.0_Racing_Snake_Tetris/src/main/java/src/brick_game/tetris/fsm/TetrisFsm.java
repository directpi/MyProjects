/**
 * @file: TetrisFsm.java
 * @description: FSM для игры Tetris, использующий нативную C библиотеку через JNI
 * @dependencies: TetrisJniBridge, GameFsm
 * @created: 2025-10-05
 */
package src.brick_game.tetris.fsm;

import src.brick_game.race.fsm.GameFsm;
import src.brick_game.tetris.jni.GameInfoData;
import src.brick_game.tetris.jni.TetrisJniBridge;
import src.brick_game.client_lib.Action;

/**
 * Реализация FSM для Tetris с использованием нативной C библиотеки.
 */
public class TetrisFsm implements GameFsm {
  private final TetrisJniBridge bridge;
  private long gamePtr = 0;
  private GameInfoData currentState;
  private boolean gameRunning = false;
  private long lastAutoFall = 0; // Время последнего автопадения

  public TetrisFsm() {
    this.bridge = new TetrisJniBridge();
    this.currentState = new GameInfoData();
  }

  private void startGame() {
    System.out.println("[TetrisFsm] startGame() called, current gamePtr=" + gamePtr
        + ", gameRunning=" + gameRunning);
    if (gamePtr != 0) {
      System.out.println("[TetrisFsm] Freeing existing game, ptr=" + gamePtr);
      bridge.freeGame(gamePtr);
      gamePtr = 0;
    }
    System.out.println("[TetrisFsm] Calling bridge.initGame()");
    gamePtr = bridge.initGame();
    System.out.println("[TetrisFsm] initGame() returned ptr=" + gamePtr);

    if (gamePtr == 0) {
      throw new RuntimeException("[TetrisFsm] Failed to initialize game - initGame returned 0");
    }

    // После initGame получаем начальное состояние через updateState с Down
    // (это вызовет spawn первой фигуры)
    System.out.println("[TetrisFsm] Getting initial state with Down action");
    currentState = bridge.updateState(gamePtr, 6); // Down = 6
    lastAutoFall = System.currentTimeMillis();
    gameRunning = true;
    System.out.println("[TetrisFsm] Game started successfully! ptr=" + gamePtr
        + ", gameRunning=" + gameRunning
        + ", score=" + currentState.score
        + ", pause=" + currentState.pause
        + ", field has content: " + hasNonEmptyField(currentState));
  }

  private boolean hasNonEmptyField(GameInfoData state) {
    if (state == null || state.field == null)
      return false;
    for (int y = 0; y < 20; y++) {
      for (int x = 0; x < 10; x++) {
        if (state.field[y][x] != 0)
          return true;
      }
    }
    return false;
  }

  @Override
  public void userInput(Action action, boolean hold) {
    if (gamePtr == 0 || !gameRunning) {
      return;
    }

    int actionCode = mapActionToCode(action);
    currentState = bridge.updateState(gamePtr, actionCode);
  }

  @Override
  public State updateCurrentState() {
    if (gamePtr == 0 || !gameRunning) {
      return createEmptyState();
    }

    // Вызываем update с действием "нет действия" для обновления по таймеру
    currentState = bridge.updateState(gamePtr, -1);

    return convertToState(currentState);
  }

  @Override
  public void applyAction(int actionId, boolean hold) {
    if (gamePtr == 0 || !gameRunning) {
      System.out.println("[TetrisFsm] applyAction skipped: gamePtr=" + gamePtr
          + ", gameRunning=" + gameRunning);
      return;
    }
    System.out.println("[TetrisFsm] applyAction: actionId=" + actionId + ", hold=" + hold);
    try {
      currentState = bridge.updateState(gamePtr, actionId);
      // ВАЖНО: сбрасываем таймер автопадения после ЛЮБОГО действия пользователя
      lastAutoFall = System.currentTimeMillis();
      System.out.println("[TetrisFsm] applyAction complete, score=" + currentState.score
          + ", pause=" + currentState.pause + ", timer reset");
    } catch (Exception e) {
      System.err.println("[TetrisFsm] ERROR in applyAction: " + e.getMessage());
      e.printStackTrace();
      gameRunning = false;
    }
  }

  @Override
  public void startGame(int gameId) {
    startGame(); // Вызываем перегруженный метод
  }

  @Override
  public GameSnapshot getSnapshot() {
    if (gamePtr == 0 || currentState == null) {
      return createEmptySnapshot();
    }

    // Автопадение фигур по таймеру (аналогично консольной версии s21_tetris.c)
    long now = System.currentTimeMillis();
    if (gameRunning && currentState.pause == 0) { // pause == 0 значит игра активна
      int speed = (currentState.speed > 0) ? currentState.speed : 500;

      if (now - lastAutoFall >= speed) {
        try {
          currentState = bridge.updateState(gamePtr, 6); // Down = 6
          lastAutoFall = now;
          System.out.println("[TetrisFsm] Auto-fall: score=" + currentState.score);
        } catch (Exception e) {
          System.err.println("[TetrisFsm] ERROR in auto-fall: " + e.getMessage());
          e.printStackTrace();
          gameRunning = false;
        }
      }
    }

    // Конвертируем int[][] в boolean[][]
    boolean[][] field = new boolean[20][10];
    for (int y = 0; y < 20; y++) {
      for (int x = 0; x < 10; x++) {
        field[y][x] = currentState.field[y][x] != 0;
      }
    }

    boolean[][] next = new boolean[4][4];
    for (int y = 0; y < 4; y++) {
      for (int x = 0; x < 4; x++) {
        next[y][x] = currentState.next[y][x] != 0;
      }
    }

    GameSnapshot snapshot = new GameSnapshot();
    snapshot.field = field;
    snapshot.next = next;
    snapshot.score = currentState.score;
    snapshot.highScore = currentState.highScore;
    snapshot.level = currentState.level;
    snapshot.speed = currentState.speed;
    snapshot.pause = currentState.pause == 1;
    snapshot.pauseState = currentState.pause; // 0=игра, 1=пауза, 2=выход, 3=game over
    if (currentState.pause == 3) {
      gameRunning = false; // сигнализируем клиенту о Game Over
    }
    snapshot.colors = null; // не используется в Tetris
    snapshot.movementSpeed = 1;
    snapshot.tickCounter = 0;
    return snapshot;
  }

  @Override
  public void reset() {
    if (gamePtr != 0) {
      bridge.freeGame(gamePtr);
      gamePtr = 0;
    }
    gameRunning = false;
    currentState = new GameInfoData();
  }

  @Override
  public boolean isGameRunning() {
    return gameRunning;
  }

  /**
   * Маппинг Action enum в коды действий C библиотеки.
   */
  private int mapActionToCode(Action action) {
    switch (action) {
      case Start:
        return 0;
      case Pause:
        return 1;
      case Terminate:
        return 2;
      case Left:
        return 3;
      case Right:
        return 4;
      case Up:
        return 5;
      case Down:
        return 6;
      case Action:
        return 7; // Rotate in Tetris
      default:
        return -1;
    }
  }

  private State createEmptyState() {
    return new State(new boolean[20][10], new boolean[4][4], 0, 0, 1, 500, false);
  }

  private State convertToState(GameInfoData data) {
    boolean[][] field = new boolean[20][10];
    for (int y = 0; y < 20; y++) {
      for (int x = 0; x < 10; x++) {
        field[y][x] = data.field[y][x] != 0;
      }
    }

    boolean[][] next = new boolean[4][4];
    for (int y = 0; y < 4; y++) {
      for (int x = 0; x < 4; x++) {
        next[y][x] = data.next[y][x] != 0;
      }
    }

    return new State(field, next, data.score, data.highScore, data.level, data.speed,
        data.pause == 1);
  }

  private GameSnapshot createEmptySnapshot() {
    GameSnapshot snapshot = new GameSnapshot();
    snapshot.field = new boolean[20][10];
    snapshot.next = new boolean[4][4];
    snapshot.score = 0;
    snapshot.highScore = 0;
    snapshot.level = 1;
    snapshot.speed = 500;
    snapshot.pause = false;
    snapshot.colors = null;
    snapshot.movementSpeed = 1;
    snapshot.tickCounter = 0;
    return snapshot;
  }

}
