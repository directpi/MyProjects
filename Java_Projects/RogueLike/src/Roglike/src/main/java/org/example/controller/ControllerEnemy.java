package org.example.controller;

import org.example.model.dungeon.LabyrinthPart;
import org.example.model.dungeon.Room;
import org.example.model.entity.Enemy;
import org.example.model.entity.Character;
import org.example.model.entity.UserHero;
import org.example.model.util.Coordinate;

import java.util.List;

public class ControllerEnemy {

    public void updateEnemies(Room room, Character player) {
        List<Enemy> enemies = room.getEnemies();

        for (Enemy enemy : enemies) {
            moveEnemy(enemy, room, player);
        }
    }

    public void moveEnemy(Enemy enemy, Room room, Character player) {
        Coordinate playerCoord = player.getCoordinate();
        enemy.move(playerCoord, room);
    }

    private boolean isInBounds(Room room, Coordinate coord) {
        return room.contains(coord);
    }

    private boolean isWall(Coordinate coord) {
        LabyrinthPart location = UserHero.getHero().getLocation();

        boolean result = false;

        if (location instanceof Room) {
            result = inRoomCoord(((Room) location), coord);
        }

        return !result;
    }

    private boolean inRoomCoord(Room room, Coordinate coord) {
        return room.isWall(coord);
    }

    private boolean isOccupied(Room room, Coordinate coord) {
        return room.isOccupied(coord) || UserHero.getHero().getCoordinate().equals(coord);
    }
}
