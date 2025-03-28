package org.example.model.entity.enemy.strategy;

import org.example.model.dungeon.Room;
import org.example.model.entity.Enemy;
import org.example.model.util.Coordinate;

public class DiagonalChaseStrategy implements EnemyMovementStrategy {

    private boolean moveHorizontal = true; // Флаг для чередования направления (горизонталь/вертикаль)

    @Override
    public boolean move(Enemy enemy, Coordinate playerCoord, Room room) {
        int playerX = playerCoord.x;
        int playerY = playerCoord.y;
        int enemyX = enemy.getCoordinate().x;
        int enemyY = enemy.getCoordinate().y;

        // Определяем направление движения к игроку
        int dx = Integer.compare(playerX, enemyX);
        int dy = Integer.compare(playerY, enemyY);

        // Чередуем движение: сначала по горизонтали, затем по вертикали
        if (moveHorizontal) {
            // Движение по горизонтали (влево или вправо)
            Coordinate newCoord = new Coordinate(enemyX + dx, enemyY);
            if (enemy.isWalkable(newCoord) && !room.isOccupied(newCoord)) {
                enemy.setCoordinate(newCoord);
                moveHorizontal = !moveHorizontal; // Меняем направление на вертикальное
                return true;
            }
        } else {
            // Движение по вертикали (вверх или вниз)
            Coordinate newCoord = new Coordinate(enemyX, enemyY + dy);
            if (enemy.isWalkable(newCoord) && !room.isOccupied(newCoord)) {
                enemy.setCoordinate(newCoord);
                moveHorizontal = !moveHorizontal; // Меняем направление на горизонтальное
                return true;
            }
        }

        // Если движение в выбранном направлении невозможно, пробуем альтернативное направление
        if (moveHorizontal) {
            // Пробуем движение по вертикали, если горизонтальное недоступно
            Coordinate newCoord = new Coordinate(enemyX, enemyY + dy);
            if (enemy.isWalkable(newCoord) && !room.isOccupied(newCoord)) {
                enemy.setCoordinate(newCoord);
                moveHorizontal = !moveHorizontal; // Меняем направление на горизонтальное
                return true;
            }
        } else {
            // Пробуем движение по горизонтали, если вертикальное недоступно
            Coordinate newCoord = new Coordinate(enemyX + dx, enemyY);
            if (enemy.isWalkable(newCoord) && !room.isOccupied(newCoord)) {
                enemy.setCoordinate(newCoord);
                moveHorizontal = !moveHorizontal; // Меняем направление на вертикальное
                return true;
            }
        }

        // Если ни одно из направлений недоступно, змей-маг не двигается
        return false;
    }
}