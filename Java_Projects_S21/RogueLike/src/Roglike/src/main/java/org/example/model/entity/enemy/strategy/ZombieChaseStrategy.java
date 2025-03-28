package org.example.model.entity.enemy.strategy;

import org.example.model.dungeon.Room;
import org.example.model.entity.Enemy;
import org.example.model.util.Coordinate;
import org.example.controller.MessageHandler;


import java.util.Random;

public class ZombieChaseStrategy implements EnemyMovementStrategy {
    private static final Random rand = new Random();
    private static final Coordinate[] DIRECTIONS = {
            new Coordinate(1, 0), new Coordinate(-1, 0),
            new Coordinate(0, 1), new Coordinate(0, -1)
    };
    private final EnemyMovementStrategy moveToPlayerStrategy = new MoveToPlayerStrategy();
    private MessageHandler handler;

    @Override
    public boolean move(Enemy enemy, Coordinate playerCoord, Room room) {
        if (enemy.getState()) {
            return randomWander(enemy, room);
        } else {
            return moveToPlayerStrategy.move(enemy, playerCoord, room);
        }
    }

    private boolean randomWander(Enemy enemy, Room room) {
        Coordinate current = enemy.getCoordinate();
        Coordinate direction = DIRECTIONS[rand.nextInt(DIRECTIONS.length)];

        Coordinate newCoord = new Coordinate(current.x + direction.x, current.y + direction.y);

        if (!room.contains(newCoord) && !room.isOccupied(newCoord)) {
            return false; // Не удалось сделать шаг
        }

        if (enemy.isWalkable(newCoord) && !room.isOccupied(newCoord)) {
            enemy.setCoordinate(newCoord);
            return true; // Шаг выполнен успешно
        } else {
            return false; // Не удалось сделать шаг
        }
    }
}
