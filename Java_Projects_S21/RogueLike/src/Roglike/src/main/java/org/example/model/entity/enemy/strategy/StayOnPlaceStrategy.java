package org.example.model.entity.enemy.strategy;

import org.example.model.dungeon.Room;
import org.example.model.entity.Enemy;
import org.example.model.util.Coordinate;
import org.example.controller.MessageHandler;

public class StayOnPlaceStrategy implements EnemyMovementStrategy {
    private final EnemyMovementStrategy moveToPlayerStrategy = new MoveToPlayerStrategy();
    private MessageHandler handler;

    @Override
    public boolean move(Enemy enemy, Coordinate playerCoord, Room room) {

        if (!enemy.getState()) {
            moveToPlayerStrategy.move(enemy, playerCoord, room);
        }
        return false;
    }
}
