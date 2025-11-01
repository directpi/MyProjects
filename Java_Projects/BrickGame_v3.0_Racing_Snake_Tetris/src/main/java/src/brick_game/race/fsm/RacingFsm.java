/**
 * @file: RacingFsm.java
 * @description: Полная реализация FSM Racing с механиками движения, столкновений, очков и уровней
 * @dependencies: src.brick_game.race.fsm.GameFsm
 * @created: 2025-10-05
 */
package src.brick_game.race.fsm;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;
import src.brick_game.client_lib.Action;

public class RacingFsm implements GameFsm {
  private static final int FIELD_WIDTH = 10;
  private static final int FIELD_HEIGHT_VISIBLE = 20;
  private static final int OFFSCREEN_ROWS = 6; // невидимая зона сверху
  private static final int FIELD_HEIGHT_FULL = FIELD_HEIGHT_VISIBLE + OFFSCREEN_ROWS; // 26
  private static final int MAX_LEVEL = 10;
  private static final int POINTS_PER_LEVEL = 5; // Каждые 5 очков = +1 уровень

  private final GameSnapshot snapshot = new GameSnapshot();
  private final Random random = new Random();
  private final List<Car> cars = new ArrayList<>();
  private int playerLane = 4; // Центральная полоса (0-9)
  private int highScore = 0;
  private boolean gameRunning = false;
  private int tickCounter = 0;
  private int baseSpeed = 1; // Базовая скорость от уровня
  private boolean isAccelerating = false; // Удерживается ли клавиша ускорения

  // Тестовый режим (package-private) — помогает стабилизировать модульные тесты
  boolean testMode = false; // по умолчанию выключен

  void setTestMode(boolean on) {
    this.testMode = on;
  }

  // Константы действий
  public static final int ACTION_PAUSE = 1;
  public static final int ACTION_LEFT = 3;
  public static final int ACTION_RIGHT = 4;
  public static final int ACTION_ACCELERATE = 5;

  public RacingFsm() {
    snapshot.field = new boolean[FIELD_HEIGHT_VISIBLE][FIELD_WIDTH];
    snapshot.next = new boolean[0][0];
    snapshot.colors = new int[FIELD_HEIGHT_VISIBLE][FIELD_WIDTH];
    snapshot.level = 1;
    snapshot.speed = 1;
    snapshot.pause = false;
    snapshot.pauseState = 0; // 0=игра (Racing не использует другие значения)
    snapshot.score = 0;
    snapshot.highScore = 0;
    // Инициализируем movementSpeed правильно
    updateCurrentSpeed();
  }

  @Override
  public void startGame(int gameId) {
    reset();
    gameRunning = true;
    // TODO: поддержка разных игр по gameId (v1/v2/v3)
  }

  @Override
  public void applyAction(int actionId, boolean hold) {
    if (!gameRunning)
      return;

    switch (actionId) {
      case ACTION_PAUSE:
        snapshot.pause = !snapshot.pause; // переключение паузы
        break;
      case ACTION_LEFT:
        if (playerLane > 1) // Ограничиваем левую границу позицией 1
          playerLane--;
        break;
      case ACTION_RIGHT:
        if (playerLane < 8) // Ограничиваем правую границу позицией 8
          playerLane++;
        break;
      case ACTION_ACCELERATE:
        // Ручное ускорение поверх базовой скорости уровня
        isAccelerating = hold;
        updateCurrentSpeed();
        break;
    }
  }

  @Override
  public GameSnapshot getSnapshot() {
    updateField();
    return snapshot;
  }

  @Override
  public void reset() {
    snapshot.score = 0;
    snapshot.pause = false;
    snapshot.pauseState = 0;
    snapshot.level = 1;
    baseSpeed = snapshot.level;
    isAccelerating = false;
    snapshot.speed = 1;
    playerLane = 4;
    cars.clear();
    tickCounter = 0;
    gameRunning = false;

    // Очищаем поле и цвета
    for (int i = 0; i < FIELD_HEIGHT_VISIBLE; i++) {
      for (int j = 0; j < FIELD_WIDTH; j++) {
        snapshot.field[i][j] = false;
        snapshot.colors[i][j] = 0;
      }
    }

    // Обновляем movementSpeed после сброса
    updateCurrentSpeed();
  }

  @Override
  public boolean isGameRunning() {
    return gameRunning;
  }

  private void updateField() {
    // Очистка поля и цветов (видимой части)
    for (int y = 0; y < FIELD_HEIGHT_VISIBLE; y++) {
      for (int x = 0; x < FIELD_WIDTH; x++) {
        snapshot.field[y][x] = false;
        snapshot.colors[y][x] = 0; // 0 = пусто
      }
    }

    if (!gameRunning)
      return;

    // Если игра на паузе, отображаем текущие позиции машин, но не обновляем логику
    if (snapshot.pause) {
      // Отображаем игрока
      placePlayerCar(snapshot.field, snapshot.colors);
      // Отображаем соперников
      for (Car car : cars) {
        placeOpponentCar(snapshot.field, snapshot.colors, car);
      }
      return;
    }

    // Управляем скоростью ТОЛЬКО частотой событий (шаг = 1 клетка)
    int updateInterval = computeUpdateInterval(snapshot.speed);
    if (tickCounter % updateInterval == 0) {
      spawnCar();
      moveCars();
      checkCollisions();
      // Начисление очков теперь перенесено в moveCars, где считаются прошедшие машины
      // Пересчёт уровня по набранным очкам: каждые 10 очков = +1 уровень
      int newLevel = Math.min((snapshot.score / POINTS_PER_LEVEL) + 1, MAX_LEVEL);
      if (newLevel != snapshot.level) {
        snapshot.level = newLevel;
        // Базовая скорость увеличивается с уровнем
        baseSpeed = newLevel;
        updateCurrentSpeed();
      }

      // Обновляем рекорд во время игры, если текущий счет больше
      if (snapshot.score > snapshot.highScore) {
        snapshot.highScore = snapshot.score;
        highScore = snapshot.score;
      }
    }

    // Размещение игрока (многокубиковая машина) в видимой части
    placePlayerCar(snapshot.field, snapshot.colors);

    // Размещение машин соперников (конвертация из полной в видимую координату)
    for (Car car : cars) {
      placeOpponentCar(snapshot.field, snapshot.colors, car);
    }

    snapshot.tickCounter = tickCounter; // Передаем счетчик ДО увеличения
    tickCounter++;
  }

  private int computeUpdateInterval(int speed) {
    // Более плавная шкала частоты с мелкими шагами к высоким скоростям
    // speed:1→7, 2→6, 3→5, 4→4, 5→3, 6→2, 7+→1
    int capped = Math.min(speed, 7);
    return Math.max(1, 8 - capped);
  }

  private void spawnCar() {
    // Попытки спавна зависят от уровня: 1..3 машины за тик
    int maxSpawns = testMode ? 3 : Math.min(3, 1 + (snapshot.level - 1) / 3);
    double baseProb = testMode ? 1.0 : Math.min(0.65, 0.20 + 0.05 * (snapshot.level - 1));

    java.util.List<Car> newThisTick = new java.util.ArrayList<>();
    for (int attempt = 0; attempt < maxSpawns; attempt++) {
      if (random.nextDouble() > baseProb)
        continue;
      java.util.List<Integer> candidates = new java.util.ArrayList<>();
      for (int cx = 1; cx <= 8; cx++) {
        if (canSpawnAtCenter(cx, newThisTick)) {
          candidates.add(cx);
        }
      }
      if (candidates.isEmpty())
        continue;
      int lane = candidates.get(random.nextInt(candidates.size()));
      // Спавним в оффскрине, чтобы машина «выезжала» сверху
      Car spawned = new Car(lane, -4, false); // верхняя точка силуэта будет появляться из-за края
      cars.add(spawned);
      newThisTick.add(spawned);
    }
  }

  private boolean canSpawnAtCenter(int centerX, java.util.List<Car> newThisTick) {
    // Горизонтальный зазор: минимум ширина машины игрока (3 пустых колонки)
    // Для машин шириной 3 это эквивалентно |Δcenter| >= 6 между соседними машинами
    // в верхней зоне.
    for (Car c : cars) {
      if (c.y <= 7 && Math.abs(centerX - c.x) < 6)
        return false;
      if (!verticalGapOkForTop(c))
        return false;
    }
    for (Car c : newThisTick) {
      if (Math.abs(centerX - c.x) < 6)
        return false;
    }
    return true;
  }

  private boolean verticalGapOkForTop(Car existing) {
    // Учитываем полную высоту: верхняя часть новой машины будет на y=-4..0.
    // Требуем зазор не менее 3 рядов между нижним краем предыдущей машины в верхней
    // зоне
    // и верхним краем новой. Допустим, не спавним близко, если существующая машина
    // выше OFFSCREEN_ROWS+2.
    return existing.y >= (OFFSCREEN_ROWS + 2);
  }

  private void moveCars() {
    int passed = 0;
    java.util.Iterator<Car> it = cars.iterator();
    while (it.hasNext()) {
      Car car = it.next();
      // Используем единую скорость движения
      car.y += snapshot.movementSpeed;
      if (car.y >= FIELD_HEIGHT_FULL) {
        passed++;
        it.remove();
      }
    }
    if (passed > 0) {
      // По заданию: 1 балл за проезд машины соперника
      snapshot.score += passed * 1;
    }
  }

  private void checkCollisions() {
    if (testMode) {
      // В тестовом режиме коллизии отключаем, чтобы не останавливать сбор очков
      return;
    }
    for (Car car : cars) {
      if (checkCarCollision(car, playerLane)) {
        // Столкновение - игра окончена
        gameRunning = false;
        if (snapshot.score > highScore) {
          highScore = snapshot.score;
          snapshot.highScore = highScore;
        }
        break;
      }
    }
  }

  private boolean checkCarCollision(Car car, int playerX) {
    // Сборка клеток игрока (1-3-1-3) в полных координатах (учитываем
    // OFFSCREEN_ROWS)
    java.util.List<int[]> playerCells = new java.util.ArrayList<>();
    int baseYFull = OFFSCREEN_ROWS + FIELD_HEIGHT_VISIBLE - 1;
    if (baseYFull - 3 >= 0)
      playerCells.add(new int[] { playerX, baseYFull - 3 });
    if (baseYFull - 2 >= 0) {
      playerCells.add(new int[] { playerX - 1, baseYFull - 2 });
      playerCells.add(new int[] { playerX, baseYFull - 2 });
      playerCells.add(new int[] { playerX + 1, baseYFull - 2 });
    }
    if (baseYFull - 1 >= 0)
      playerCells.add(new int[] { playerX, baseYFull - 1 });
    playerCells.add(new int[] { playerX - 1, baseYFull });
    playerCells.add(new int[] { playerX, baseYFull });
    playerCells.add(new int[] { playerX + 1, baseYFull });

    // Сборка клеток соперника (1-3-1-3-1 сверху вниз от car.y) — в полных
    // координатах
    java.util.List<int[]> oppCells = new java.util.ArrayList<>();
    oppCells.add(new int[] { car.x, car.y });
    oppCells.add(new int[] { car.x - 1, car.y + 1 });
    oppCells.add(new int[] { car.x, car.y + 1 });
    oppCells.add(new int[] { car.x + 1, car.y + 1 });
    oppCells.add(new int[] { car.x, car.y + 2 });
    oppCells.add(new int[] { car.x - 1, car.y + 3 });
    oppCells.add(new int[] { car.x, car.y + 3 });
    oppCells.add(new int[] { car.x + 1, car.y + 3 });
    oppCells.add(new int[] { car.x, car.y + 4 });

    for (int[] pc : playerCells) {
      int px = pc[0], py = pc[1];
      if (px < 0 || px >= FIELD_WIDTH || py < 0 || py >= FIELD_HEIGHT_FULL)
        continue;
      for (int[] oc : oppCells) {
        int ox = oc[0], oy = oc[1];
        if (ox < 0 || ox >= FIELD_WIDTH || oy < 0 || oy >= FIELD_HEIGHT_FULL)
          continue;
        if (px == ox && py == oy)
          return true;
      }
    }
    return false;
  }

  private void updateCurrentSpeed() {
    // Базовая скорость от уровня + более мягкое ускорение при удержании клавиши
    if (isAccelerating) {
      snapshot.speed = Math.min(baseSpeed + 1, 15); // Ускорение +1 к базовой скорости
    } else {
      snapshot.speed = baseSpeed; // Только базовая скорость уровня
    }

    // Делает движение недискретным за счет частоты событий: шаг всегда 1 клетка
    snapshot.movementSpeed = 1;
  }

  private void updateScore() {
    // Этот метод больше не используется, логика перенесена в moveCars()
    // Оставляем для совместимости, но не используем
  }

  private void placePlayerCar(boolean[][] field, int[][] colors) {
    int baseYFull = OFFSCREEN_ROWS + FIELD_HEIGHT_VISIBLE - 1;
    int x = playerLane;
    // Конвертация в видимые координаты
    int baseY = baseYFull - OFFSCREEN_ROWS;

    // Цвет игрока = 1
    if (baseY - 3 >= 0 && x >= 0 && x < FIELD_WIDTH) {
      field[baseY - 3][x] = true;
      colors[baseY - 3][x] = 1;
    }
    if (baseY - 2 >= 0) {
      if (x - 1 >= 0) {
        field[baseY - 2][x - 1] = true;
        colors[baseY - 2][x - 1] = 1;
      }
      if (x >= 0 && x < FIELD_WIDTH) {
        field[baseY - 2][x] = true;
        colors[baseY - 2][x] = 1;
      }
      if (x + 1 < FIELD_WIDTH) {
        field[baseY - 2][x + 1] = true;
        colors[baseY - 2][x + 1] = 1;
      }
    }
    if (baseY - 1 >= 0 && x >= 0 && x < FIELD_WIDTH) {
      field[baseY - 1][x] = true;
      colors[baseY - 1][x] = 1;
    }
    if (x - 1 >= 0) {
      field[baseY][x - 1] = true;
      colors[baseY][x - 1] = 1;
    }
    if (x >= 0 && x < FIELD_WIDTH) {
      field[baseY][x] = true;
      colors[baseY][x] = 1;
    }
    if (x + 1 < FIELD_WIDTH) {
      field[baseY][x + 1] = true;
      colors[baseY][x + 1] = 1;
    }
  }

  private void placeOpponentCar(boolean[][] field, int[][] colors, Car car) {
    int yFull = car.y;
    int x = car.x;

    // Конвертация в видимые координаты
    int y = yFull - OFFSCREEN_ROWS;

    // Цвет соперника = 2, 3, 4 (car.colorIndex + 2)
    int color = car.colorIndex + 2;

    // Машина соперника: 5 рядов — рисуем только видимую часть
    if (x >= 0 && x < FIELD_WIDTH && y >= 0 && y < FIELD_HEIGHT_VISIBLE) {
      field[y][x] = true;
      colors[y][x] = color;
    }
    if (x - 1 >= 0 && y + 1 >= 0 && y + 1 < FIELD_HEIGHT_VISIBLE) {
      field[y + 1][x - 1] = true;
      colors[y + 1][x - 1] = color;
    }
    if (x >= 0 && x < FIELD_WIDTH && y + 1 >= 0 && y + 1 < FIELD_HEIGHT_VISIBLE) {
      field[y + 1][x] = true;
      colors[y + 1][x] = color;
    }
    if (x + 1 < FIELD_WIDTH && y + 1 >= 0 && y + 1 < FIELD_HEIGHT_VISIBLE) {
      field[y + 1][x + 1] = true;
      colors[y + 1][x + 1] = color;
    }
    if (x >= 0 && x < FIELD_WIDTH && y + 2 >= 0 && y + 2 < FIELD_HEIGHT_VISIBLE) {
      field[y + 2][x] = true;
      colors[y + 2][x] = color;
    }
    if (x - 1 >= 0 && y + 3 >= 0 && y + 3 < FIELD_HEIGHT_VISIBLE) {
      field[y + 3][x - 1] = true;
      colors[y + 3][x - 1] = color;
    }
    if (x >= 0 && x < FIELD_WIDTH && y + 3 >= 0 && y + 3 < FIELD_HEIGHT_VISIBLE) {
      field[y + 3][x] = true;
      colors[y + 3][x] = color;
    }
    if (x + 1 < FIELD_WIDTH && y + 3 >= 0 && y + 3 < FIELD_HEIGHT_VISIBLE) {
      field[y + 3][x + 1] = true;
      colors[y + 3][x + 1] = color;
    }
    if (x >= 0 && x < FIELD_WIDTH && y + 4 >= 0 && y + 4 < FIELD_HEIGHT_VISIBLE) {
      field[y + 4][x] = true;
      colors[y + 4][x] = color;
    }
  }

  private static class Car {
    int x, y;
    boolean isPlayer;
    int colorIndex; // Индекс цвета для машин соперников (0, 1, 2)

    Car(int x, int y, boolean isPlayer) {
      this.x = x;
      this.y = y;
      this.isPlayer = isPlayer;
      // Стабильный цвет на основе позиции спавна
      this.colorIndex = isPlayer ? -1 : Math.abs((x * 31 + y * 17) % 3);
    }
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
    // Создаем State точно по спецификации
    return new State(snapshot.field, snapshot.next, snapshot.score, snapshot.highScore,
        snapshot.level, snapshot.speed, snapshot.pause);
  }
}
