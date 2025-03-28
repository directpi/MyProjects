package org.example.model.entity.enemy.strategy;

import org.example.controller.MessageHandler;
import org.example.model.dungeon.Room;
import org.example.model.entity.Enemy;
import org.example.model.util.Coordinate;

public class DiagonalMoveStrategy implements EnemyMovementStrategy {
    private boolean moveRight = true; // Направление по горизонтали (вправо/влево)
    private boolean moveDown = true;  // Направление по вертикали (вниз/вверх)
    private final EnemyMovementStrategy moveToPlayerStrategy = new MoveToPlayerStrategy();
    private MessageHandler handler;

    @Override
    public boolean move(Enemy enemy, Coordinate playerCoord, Room room) {
        if (enemy.getState()) {
            diagonalMove(enemy, room);
        } else {
            moveToPlayerStrategy.move(enemy, playerCoord, room);
        }
        return false;
    }

    private void diagonalMove(Enemy enemy, Room room) {
        Coordinate current = enemy.getCoordinate();
        int newX = moveRight ? current.x + 1 : current.x - 1; // Движение по горизонтали
        int newY = moveDown ? current.y + 1 : current.y - 1;  // Движение по вертикали

        boolean reachedRight = newX > room.getBottomRight().x;
        boolean reachedLeft = newX < room.getTopLeft().x;
        boolean reachedBottom = newY > room.getBottomRight().y;
        boolean reachedTop = newY < room.getTopLeft().y;

        // Если упёрлись в обе стены (например, в углу), корректируем сразу оба направления
        if (reachedRight && reachedBottom) {
            moveRight = false;
            moveDown = false;  // Двигаемся влево-вверх
        } else if (reachedRight && reachedTop) {
            moveRight = false;
            moveDown = true;  // Двигаемся влево-вниз
        } else if (reachedLeft && reachedBottom) {
            moveRight = true;
            moveDown = false;  // Двигаемся вправо-вверх
        } else if (reachedLeft && reachedTop) {
            moveRight = true;
            moveDown = true;  // Двигаемся вправо-вниз
        } else {
            if (reachedRight || reachedLeft) {
                moveRight = !moveRight;
            }
            if (reachedBottom || reachedTop) {
                moveDown = !moveDown;
            }
        }

        newX = moveRight ? current.x + 1 : current.x - 1;
        newY = moveDown ? current.y + 1 : current.y - 1;

        Coordinate newCoord = new Coordinate(newX, newY);

        if (room.contains(newCoord) && enemy.isWalkable(newCoord) && !room.isOccupied(newCoord)) {
            enemy.setCoordinate(newCoord);
        } else {

            // Если клетка непроходима, пробуем изменить направление и переместиться снова
            boolean moved = false;

            // Пробуем изменить только горизонтальное направление
            moveRight = !moveRight;
            newX = moveRight ? current.x + 1 : current.x - 1;
            newCoord = new Coordinate(newX, newY);

            if (room.contains(newCoord) && enemy.isWalkable(newCoord) && !room.isOccupied(newCoord)) {
                enemy.setCoordinate(newCoord);
                moved = true;
            } else {
                moveRight = !moveRight; // Возвращаем обратно
                moveDown = !moveDown;
                newX = moveRight ? current.x + 1 : current.x - 1;
                newY = moveDown ? current.y + 1 : current.y - 1;
                newCoord = new Coordinate(newX, newY);

                if (room.contains(newCoord) && enemy.isWalkable(newCoord) && !room.isOccupied(newCoord)) {
                    enemy.setCoordinate(newCoord);
                    moved = true;
                } else {
                    // меняем оба направления
                    moveRight = !moveRight;
                    moveDown = !moveDown;
                    newX = moveRight ? current.x + 1 : current.x - 1;
                    newY = moveDown ? current.y + 1 : current.y - 1;
                    newCoord = new Coordinate(newX, newY);

                    if (room.contains(newCoord) && enemy.isWalkable(newCoord) && !room.isOccupied(newCoord)) {
                        enemy.setCoordinate(newCoord);
                        moved = true;
                    }
                }
            }
        }
    }
}