package org.example.model.entity.enemy.strategy;

import org.example.controller.MessageHandler;
import org.example.model.dungeon.Room;
import org.example.model.entity.Enemy;
import org.example.model.util.Coordinate;

import java.util.Optional;
import java.util.Random;
import java.util.stream.Stream;

public class GhostTeleportStrategy implements EnemyMovementStrategy {
    private static final Random rand = new Random();
    private final EnemyMovementStrategy moveToPlayerStrategy = new MoveToPlayerStrategy();
    private MessageHandler handler;

    public GhostTeleportStrategy(MessageHandler handler) {
        this.handler = handler;
    }

    @Override
    public boolean move(Enemy enemy, Coordinate playerCoord, Room room) {
        Coordinate topLeft = room.getTopLeft();
        Coordinate bottomRight = room.getBottomRight();

        if (enemy.getState()) {
            teleport(enemy, topLeft, bottomRight, room);
        } else {
            moveToPlayerStrategy.move(enemy, playerCoord, room);
        }
        return false;
    }

    private void teleport(Enemy enemy, Coordinate topLeft, Coordinate bottomRight, Room room) {
        Optional<Coordinate> validCoord = Stream.generate(() -> new Coordinate(
                        rand.nextInt(bottomRight.x - topLeft.x + 1) + topLeft.x,
                        rand.nextInt(bottomRight.y - topLeft.y + 1) + topLeft.y
                ))
                .limit(10)
                .filter(enemy::isWalkable)
                .filter(coord -> !room.isOccupied(coord))
                .findFirst();

        validCoord.ifPresent(enemy::setCoordinate);
    }
}
