package org.example.model.entity.enemy.strategy;

import org.example.model.dungeon.Room;
import org.example.model.entity.Enemy;
import org.example.model.util.Coordinate;
import org.example.model.util.Direction;

public class MoveToPlayerStrategy implements EnemyMovementStrategy {

    @Override
    public boolean move(Enemy enemy, Coordinate playerCoord, Room room) {
        int playerX = playerCoord.x;
        int playerY = playerCoord.y;
        int enemyX = enemy.getCoordinate().x;
        int enemyY = enemy.getCoordinate().y;

        Coordinate topLeft = room.getTopLeft();
        Coordinate bottomRight = room.getBottomRight();

        // Проверяем, находится ли персонаж рядом с героем

        if (enemy.getCoordinate().distanceTo(playerCoord) == 1) {
            return false;
        }
        // Выбираем направление, которое ближе к игроку
        int dx = Integer.compare(playerX, enemyX);
        int dy = Integer.compare(playerY, enemyY);

        if (dx != 0) {
            Coordinate newCoord = new Coordinate(enemyX + dx, enemyY);
            if (enemy.isWalkable(newCoord) && !room.isOccupied(newCoord)) {
                enemy.move(dx > 0 ? Direction.RIGHT : Direction.LEFT);
                return false;
            }
        }

        if (dy != 0) {
            Coordinate newCoord = new Coordinate(enemyX, enemyY + dy);
            if (enemy.isWalkable(newCoord) && !room.isOccupied(newCoord)) {
                enemy.move(dy > 0 ? Direction.BOTTOM : Direction.TOP);
            }
        }
        return false;
    }
}
