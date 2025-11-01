/**
 * @file: GameService.java
 * @description: Сервисный слой: управление текущей игрой и интеграция с FSM
 * @dependencies: src.brick_game.race.fsm.*, src.brick_game.server.api.dto.*
 * @created: 2025-10-05
 */
package src.brick_game.server.service;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import src.brick_game.race.fsm.GameFsm;
import src.brick_game.race.fsm.RacingFsm;
// import src.brick_game.race.fsm.NativeGameAdapter;
import src.brick_game.tetris.fsm.TetrisFsm;
import src.brick_game.snake.fsm.SnakeFsm;
import src.brick_game.server.api.dto.GameStateDto;
import src.brick_game.server.repository.HighScoreRepository;
import java.util.List;
import java.util.Arrays;
import src.brick_game.server.api.dto.GameInfoDto;

@Service
public class GameService {
  private final GameFsm racingFsm;
  private final GameFsm tetrisFsm; // Singleton для Tetris
  private final GameFsm snakeFsm; // Singleton для Snake
  private final HighScoreRepository highScoreRepository;
  private Integer currentGameId = null;
  private GameFsm currentFsm = null;
  private final List<GameInfoDto> registry = Arrays.asList(new GameInfoDto(1, "Racing"),
      new GameInfoDto(2, "Tetris"), new GameInfoDto(3, "Snake"));

  @Autowired
  public GameService(HighScoreRepository highScoreRepository) {
    this.racingFsm = new RacingFsm();
    this.highScoreRepository = highScoreRepository;

    // Инициализируем TetrisFsm один раз (singleton)
    try {
      this.tetrisFsm = new TetrisFsm();
      System.out.println("[GameService] TetrisFsm singleton created successfully");
    } catch (UnsatisfiedLinkError | Exception e) {
      System.err.println("[GameService] Failed to create TetrisFsm: " + e.getMessage());
      e.printStackTrace();
      throw new RuntimeException("Tetris JNI integration not available", e);
    }

    // Инициализируем SnakeFsm один раз (singleton)
    try {
      this.snakeFsm = new SnakeFsm();
      System.out.println("[GameService] SnakeFsm singleton created successfully");
    } catch (UnsatisfiedLinkError | Exception e) {
      System.err.println("[GameService] Failed to create SnakeFsm: " + e.getMessage());
      e.printStackTrace();
      throw new RuntimeException("Snake JNI integration not available", e);
    }
  }

  public List<GameInfoDto> getGames() {
    return registry;
  }

  public void startGame(int gameId) {
    System.out.println("[GameService] startGame called with gameId=" + gameId
        + ", currentGameId=" + currentGameId);

    // Всегда сбрасываем текущую игру перед запуском новой (даже если это та же игра
    // - рестарт)
    if (currentGameId != null && currentFsm != null) {
      try {
        System.out.println("[GameService] Stopping previous game: id=" + currentGameId);
        currentFsm.reset();
      } catch (Exception e) {
        System.err.println("[GameService] Error while stopping previous game: " + e.getMessage());
      } finally {
        currentFsm = null;
        currentGameId = null;
      }
    }

    if (gameId == 1) {
      System.out.println("[GameService] Starting Racing");
      currentFsm = racingFsm;
      currentFsm.startGame(gameId);
      currentGameId = gameId;
    } else if (gameId == 2) {
      System.out.println("[GameService] Starting Tetris (using singleton FSM)");
      // Используем singleton TetrisFsm, как и для Racing
      currentFsm = tetrisFsm;
      currentFsm.startGame(gameId);
      currentGameId = gameId;
      System.out.println("[GameService] Tetris started successfully");
    } else if (gameId == 3) {
      System.out.println("[GameService] Starting Snake (using singleton FSM)");
      // Используем singleton SnakeFsm, как и для Racing/Tetris
      currentFsm = snakeFsm;
      currentFsm.startGame(gameId);
      currentGameId = gameId;
      System.out.println("[GameService] Snake started successfully");
    } else {
      throw new IllegalArgumentException("GAME_NOT_FOUND");
    }
  }

  public void performAction(int actionId, boolean hold) {
    // 400: игра не запущена
    if (currentGameId == null || currentFsm == null) {
      throw new IllegalStateException("NO_GAME_RUNNING");
    }
    currentFsm.applyAction(actionId, hold);
  }

  public GameStateDto getState() {
    if (currentGameId == null || currentFsm == null) {
      throw new IllegalStateException("NO_GAME_RUNNING");
    }
    GameFsm.GameSnapshot s = currentFsm.getSnapshot();
    GameStateDto dto = new GameStateDto();
    dto.setField(s.field);
    dto.setNext(s.next);
    dto.setColors(s.colors);
    dto.setScore(s.score);

    // High Score обрабатывается по-разному для разных игр:
    // - Racing (1): использует highScoreRepository (data/highscore.json)
    // - Tetris (2): использует свой файл tetris_record_score.txt (управляется C
    // кодом)
    // - Snake (3): использует свой файл snake_record_score.txt (управляется C++
    // кодом)
    if (currentGameId != null && currentGameId.equals(1)) {
      // Racing: используем общий репозиторий
      int currentStored = highScoreRepository.getHighScore();
      if (s.highScore > currentStored) {
        highScoreRepository.saveHighScore(s.highScore);
        dto.setHigh_score(s.highScore);
      } else {
        dto.setHigh_score(currentStored);
      }
    } else if (currentGameId != null && (currentGameId.equals(2) || currentGameId.equals(3))) {
      // Tetris/Snake: берём High Score из FSM, показываем максимум в реальном времени
      // Сохранение в файл происходит в C/C++ коде при достижении рекорда
      int displayHighScore = Math.max(s.score, s.highScore);
      dto.setHigh_score(displayHighScore);
    } else {
      // Fallback: если gameId неизвестен, берём из FSM
      dto.setHigh_score(s.highScore);
    }
    dto.setLevel(s.level);
    dto.setSpeed(s.speed);
    dto.setMovementSpeed(s.movementSpeed);
    dto.setPause(s.pause);
    dto.setGameRunning(currentFsm.isGameRunning());
    dto.setTickCounter(s.tickCounter);
    return dto;
  }
}
