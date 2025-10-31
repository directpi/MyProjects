package org.example.model.dungeon;

import lombok.Getter;
import lombok.Setter;
import org.example.model.entity.*;
import org.example.model.entity.Character;
import org.example.model.util.Coordinate;
import org.example.model.util.Direction;

import java.util.ArrayList;
import java.util.Random;

public class Room implements LabyrinthPart {

    @Getter
    private final int sector;
    private final Coordinate topLeft;
    private final Coordinate bottomRight;
    private final Door[] doors;
    @Getter
    private final ArrayList<Item> items;
    @Getter
    private final ArrayList<Enemy> enemies;
    private boolean isStartRoom;
    @Setter
    @Getter
    private Exit exit;

    {
        isStartRoom = false;
        topLeft = new Coordinate();
        bottomRight = new Coordinate();
        enemies = new ArrayList<>();
        items = new ArrayList<>();
        doors = new Door[4];
        exit = null;
    }

    public Room(int sector) {
        this.sector = sector;
    }

    public boolean contains(Coordinate coord) {
        return coord.x >= topLeft.x && coord.x <= bottomRight.x &&
                coord.y >= topLeft.y && coord.y <= bottomRight.y;
    }


    public boolean isWall(Coordinate coord) {
        return coord.x == topLeft.x || coord.x == bottomRight.x ||
                coord.y == topLeft.y || coord.y == bottomRight.y;
    }

    public boolean isOccupied(Coordinate coord) {
        UserHero hero = UserHero.getHero();
        if (hero != null && hero.getLocation() != null && hero.getLocation().equals(coord)) {
            return true;
        }
        for (Enemy enemy : enemies) {
            if (enemy.getCoordinate().equals(coord)) {
                return true;
            }
        }
        for (Item item : items) {
            if (item.getCoordinate().equals(coord)) {
                return true;
            }
        }
        return false;
    }

    public void addEntity(Entity entity) {
        if (entity instanceof Item) {
            items.add((Item) entity);
        } else if (entity instanceof Character) {
            enemies.add((Enemy) entity);
        }
    }

    public void setStartRoom() {
        isStartRoom = true;
    }

    public boolean isStartRoom() {
        return isStartRoom;
    }

    public int getRowNumber() {
        return sector / 3;
    }

    public int getColumnNumber() {
        return sector % 3;
    }

    public Coordinate getTopLeft() {
        return new Coordinate(topLeft);
    }

    public Coordinate getBottomRight() {
        return new Coordinate(bottomRight);
    }

    public Door getDoor(Direction direction) {
        return switch (direction) {
            case TOP -> doors[0];
            case RIGHT -> doors[1];
            case BOTTOM -> doors[2];
            case LEFT -> doors[3];
        };
    }

    public ArrayList<Door> getAllDoors() {
        ArrayList<Door> allDoors = new ArrayList<>(4);
        for (var door : doors) {
            if (door != null) {
                allDoors.add(door);
            }
        }
        return allDoors;
    }

    public int getDoorCount() {
        int cnt = 0;
        for (var door : doors) {
            if (door != null) {
                cnt++;
            }
        }
        return cnt;
    }

    public int getConnectCount() {
        int count = 0;
        for (Door door : doors) {
            if (door != null && door.isConnectionExist()) {
                count++;
            }
        }
        return count;
    }

    public void addConnections(Direction direction, Corridor corridor) {
        doors[direction.ordinal()].setConnectionCorridor(corridor);
    }

    public static class StaticRoomGenerator {
        private static Room room;
        private static int sector_height;
        private static int sector_width;

        public static Room createRoom(int sector, int sectorHeight, int sectorWidth) {
            room = new Room(sector);
            sector_height = sectorHeight;
            sector_width = sectorWidth;
            generateCorners();
            generateDoors();
            return room;
        }

        private static void generateCorners() {
            var random = new Random();

            room.topLeft.y = random.nextInt((sector_height - 6) / 2) + room.getRowNumber() * sector_height + 1;
            room.topLeft.x = random.nextInt((sector_width - 6) / 2) + room.getColumnNumber() * sector_width + 1;

            room.bottomRight.y = sector_height - 1 - random.nextInt((sector_height - 6) / 2) + room.getRowNumber() * sector_height - 1;
            room.bottomRight.x = sector_width - 1 - random.nextInt((sector_width - 6) / 2) + room.getColumnNumber() * sector_width - 1;
        }

        private static void generateDoors() {
            var random = new Random();
            Coordinate coord = new Coordinate();
            Coordinate topLeft = room.getTopLeft();
            Coordinate bottomRight = room.getBottomRight();
            if (room.getRowNumber() != 0) {
                coord.y = topLeft.y;
                coord.x = random.nextInt(bottomRight.x - topLeft.x - 1) + topLeft.x + 1;
                room.doors[Direction.TOP.ordinal()] = new Door(coord);
            }
            if (room.getColumnNumber() != 2) {
                coord.y = random.nextInt(bottomRight.y - topLeft.y - 1) + topLeft.y + 1;
                coord.x = bottomRight.x;
                room.doors[Direction.RIGHT.ordinal()] = new Door(coord);
            }
            if (room.getRowNumber() != 2) {
                coord.y = bottomRight.y;
                coord.x = random.nextInt(bottomRight.x - topLeft.x - 1) + topLeft.x + 1;
                room.doors[Direction.BOTTOM.ordinal()] = new Door(coord);
            }
            if (room.getColumnNumber() != 0) {
                coord.y = random.nextInt(bottomRight.y - topLeft.y - 1) + topLeft.y + 1;
                coord.x = topLeft.x;
                room.doors[Direction.LEFT.ordinal()] = new Door(coord);
            }
        }
    }
}
