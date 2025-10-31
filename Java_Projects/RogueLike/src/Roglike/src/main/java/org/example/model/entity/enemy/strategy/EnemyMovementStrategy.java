package org.example.model.entity.enemy.strategy;

import org.example.model.dungeon.Room;
import org.example.model.entity.Enemy;
import org.example.model.util.Coordinate;

public interface EnemyMovementStrategy {
    boolean move(Enemy enemy, Coordinate playerCoord, Room room);
}
