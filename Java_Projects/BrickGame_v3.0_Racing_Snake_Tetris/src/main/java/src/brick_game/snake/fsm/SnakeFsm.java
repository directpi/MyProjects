/**
 * @file: SnakeFsm.java
 * @description: FSM для Snake через JNI интеграцию с C++ кодом
 * @dependencies: SnakeJniBridge, GameInfoData
 * @created: 2025-10-06
 */
package src.brick_game.snake.fsm;

import src.brick_game.client_lib.Action;
import src.brick_game.race.fsm.GameFsm;
import src.brick_game.snake.jni.GameInfoData;
import src.brick_game.snake.jni.SnakeJniBridge;

public class SnakeFsm implements GameFsm {
    private final SnakeJniBridge bridge;
    private long gamePtr = 0;
    private GameInfoData currentState = null;
    private boolean gameRunning = false;
    private long lastUpdate = System.currentTimeMillis();

    public SnakeFsm() {
        this.bridge = new SnakeJniBridge();
        System.out.println("[SnakeFsm] Instance created");
    }

    @Override
    public void userInput(Action action, boolean hold) {
        applyAction(action.getId(), hold);
    }

    @Override
    public State updateCurrentState() {
        GameSnapshot snapshot = getSnapshot();
        return new State(snapshot.field, snapshot.next, snapshot.score, snapshot.highScore, snapshot.level,
                snapshot.speed, snapshot.pause);
    }

    @Override
    public void startGame(int gameId) {
        startGame(); // Вызываем перегруженный метод
    }

    private void startGame() {
        System.out.println("[SnakeFsm] startGame() called, current gamePtr=" + gamePtr + ", gameRunning="
                + gameRunning);
        if (gamePtr != 0) {
            System.out.println("[SnakeFsm] Freeing existing game, ptr=" + gamePtr);
            bridge.freeGame(gamePtr);
            gamePtr = 0;
        }
        System.out.println("[SnakeFsm] Calling bridge.initGame()");
        gamePtr = bridge.initGame();
        System.out.println("[SnakeFsm] initGame() returned ptr=" + gamePtr);

        if (gamePtr == 0) {
            throw new RuntimeException("[SnakeFsm] Failed to initialize game - initGame returned 0");
        }

        // После initGame получаем начальное состояние
        System.out.println("[SnakeFsm] Getting initial state");
        currentState = bridge.updateState(gamePtr, 0, false); // Start = 0
        lastUpdate = System.currentTimeMillis();
        gameRunning = true;
        System.out.println("[SnakeFsm] Game started successfully! ptr=" + gamePtr + ", gameRunning="
                + gameRunning + ", score=" + currentState.score + ", pause=" + currentState.pause);
    }

    @Override
    public void applyAction(int actionId, boolean hold) {
        if (gamePtr == 0 || !gameRunning) {
            System.out
                    .println("[SnakeFsm] applyAction skipped: gamePtr=" + gamePtr + ", gameRunning=" + gameRunning);
            return;
        }
        System.out.println("[SnakeFsm] applyAction: actionId=" + actionId + ", hold=" + hold);
        try {
            currentState = bridge.updateState(gamePtr, actionId, hold); // ✅ Передаем hold!
            // ВАЖНО: обновляем lastUpdate, чтобы автодвижение не произошло сразу после
            // действия
            lastUpdate = System.currentTimeMillis();
            System.out.println("[SnakeFsm] applyAction complete, score=" + currentState.score + ", pause="
                    + currentState.pause);
        } catch (Exception e) {
            System.err.println("[SnakeFsm] ERROR in applyAction: " + e.getMessage());
            e.printStackTrace();
            gameRunning = false;
        }
    }

    @Override
    public GameSnapshot getSnapshot() {
        if (gamePtr == 0 || currentState == null) {
            return createEmptySnapshot();
        }

        // Автодвижение змейки по таймеру (если игра не на паузе и не Game
        // Over/Exit/Win)
        // ВАЖНО: C++ код Snake имеет ВСТРОЕННЫЙ таймер в timer_move()
        // Вызываем Start (0) чтобы C++ таймер сработал, но без дополнительных действий
        long now = System.currentTimeMillis();
        if (currentState.pause == 0 && gameRunning) { // pause == 0 значит игра активна
            int speed = (currentState.speed > 0) ? currentState.speed : 500;
            if (now - lastUpdate >= speed) {
                try {
                    // Start (0) безопасно - не делает ничего, но timer_move() в C++ отработает
                    currentState = bridge.updateState(gamePtr, 0, false); // Start = 0
                    lastUpdate = now;
                    System.out.println("[SnakeFsm] Auto-move (timer tick): score=" + currentState.score
                            + ", speed=" + currentState.speed + ", pause=" + currentState.pause);
                } catch (Exception e) {
                    System.err.println("[SnakeFsm] ERROR in auto-move: " + e.getMessage());
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

        boolean[][] next = new boolean[4][4]; // Не используется в Snake

        GameSnapshot snapshot = new GameSnapshot();
        snapshot.field = field;
        snapshot.next = next;
        snapshot.score = currentState.score;
        snapshot.highScore = currentState.highScore;
        snapshot.level = currentState.level;
        snapshot.speed = currentState.speed;
        snapshot.pause = currentState.pause == 6; // PAUSE_STATE = 6
        snapshot.pauseState = currentState.pause; // 0=игра, 5=game over, 6=пауза, 7=выход, 8=победа
        if (currentState.pause == 5 || currentState.pause == 7 || currentState.pause == 8) {
            gameRunning = false; // сигнализируем клиенту о Game Over/Exit/Win
        }
        snapshot.colors = null; // не используется в Snake (пока)
        snapshot.movementSpeed = 1;
        snapshot.tickCounter = 0;
        return snapshot;
    }

    @Override
    public boolean isGameRunning() {
        return gameRunning;
    }

    @Override
    public void reset() {
        System.out.println("[SnakeFsm] reset() called");
        if (gamePtr != 0) {
            bridge.freeGame(gamePtr);
            gamePtr = 0;
        }
        currentState = null;
        gameRunning = false;
        lastUpdate = System.currentTimeMillis();
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
        snapshot.pauseState = 0;
        snapshot.colors = null;
        snapshot.movementSpeed = 1;
        snapshot.tickCounter = 0;
        return snapshot;
    }
}
