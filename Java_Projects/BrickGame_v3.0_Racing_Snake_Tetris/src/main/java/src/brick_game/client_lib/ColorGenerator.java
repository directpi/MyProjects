/**
 * @file: ColorGenerator.java
 * @description: Генератор цветов для машин на клиенте (совместимость со спецификацией)
 * @dependencies: none
 * @created: 2025-10-05
 */
package src.brick_game.client_lib;

import java.util.HashMap;
import java.util.Map;

/**
 * Генерирует стабильные цвета для машин на основе их позиции Заменяет передачу colors[][] через API
 */
public class ColorGenerator {

  // Цвета: 0=пусто, 1=игрок, 2-4=соперники
  public static final int COLOR_EMPTY = 0;
  public static final int COLOR_PLAYER = 1;
  public static final int COLOR_OPPONENT_1 = 2;
  public static final int COLOR_OPPONENT_2 = 3;
  public static final int COLOR_OPPONENT_3 = 4;

  // Кэш цветов для стабильности
  private final Map<String, Integer> carColors = new HashMap<>();

  /**
   * Получить цвет для клетки на основе позиции и содержимого
   * 
   * @param field - игровое поле
   * @param x - координата X
   * @param y - координата Y
   * @return цвет клетки (0-4)
   */
  public int getColorForCell(boolean[][] field, int x, int y) {
    if (!field[y][x]) {
      return COLOR_EMPTY;
    }

    // Определяем, является ли это машиной игрока (в нижних рядах)
    if (isPlayerCar(field, x, y)) {
      return COLOR_PLAYER;
    }

    // Для машин соперников генерируем стабильный цвет
    return getOpponentCarColor(field, x, y);
  }

  /**
   * Проверяет, является ли клетка частью машины игрока
   */
  private boolean isPlayerCar(boolean[][] field, int x, int y) {
    // Машина игрока находится в нижних рядах (y >= 16)
    // и имеет характерный паттерн
    if (y < 16)
      return false;

    // Проверяем паттерн машины игрока (3x5 клеток)
    int baseY = field.length - 1;
    if (y > baseY)
      return false;

    // Простая проверка: если в нижних 5 рядах и есть соседние клетки
    return y >= baseY - 4 && hasNeighbors(field, x, y);
  }

  /**
   * Проверяет наличие соседних клеток (признак машины)
   */
  private boolean hasNeighbors(boolean[][] field, int x, int y) {
    int count = 0;
    for (int dy = -1; dy <= 1; dy++) {
      for (int dx = -1; dx <= 1; dx++) {
        int nx = x + dx;
        int ny = y + dy;
        if (nx >= 0 && nx < field[0].length && ny >= 0 && ny < field.length) {
          if (field[ny][nx])
            count++;
        }
      }
    }
    return count >= 3; // Машина имеет минимум 3 соседние клетки
  }

  /**
   * Генерирует стабильный цвет для машины соперника
   */
  private int getOpponentCarColor(boolean[][] field, int x, int y) {
    // Находим центр машины для стабильного ключа
    String carKey = findCarCenter(field, x, y);

    if (!carColors.containsKey(carKey)) {
      // Генерируем случайный, но стабильный цвет
      int color = COLOR_OPPONENT_1 + (Math.abs(carKey.hashCode()) % 3);
      carColors.put(carKey, color);
    }

    return carColors.get(carKey);
  }

  /**
   * Находит центр машины для создания стабильного ключа
   */
  private String findCarCenter(boolean[][] field, int x, int y) {
    // Ищем ближайший центр машины (простая эвристика)
    int centerX = x;
    int centerY = y;

    // Ищем самую левую и самую правую клетки машины
    int leftX = x;
    int rightX = x;
    while (leftX > 0 && field[y][leftX - 1])
      leftX--;
    while (rightX < field[0].length - 1 && field[y][rightX + 1])
      rightX++;

    centerX = (leftX + rightX) / 2;

    // Ищем самую верхнюю и самую нижнюю клетки машины
    int topY = y;
    int bottomY = y;
    while (topY > 0 && field[topY - 1][centerX])
      topY--;
    while (bottomY < field.length - 1 && field[bottomY + 1][centerX])
      bottomY++;

    centerY = (topY + bottomY) / 2;

    return centerX + "," + centerY;
  }

  /**
   * Очищает кэш цветов (вызывать при смене игры)
   */
  public void clearCache() {
    carColors.clear();
  }
}
