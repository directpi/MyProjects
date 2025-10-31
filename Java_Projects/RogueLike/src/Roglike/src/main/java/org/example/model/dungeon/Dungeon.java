package org.example.model.dungeon;


import lombok.Getter;
import org.example.model.entity.enemy.StaticEnemyFactory;
import org.example.model.entity.item.StaticItemFactory;
import org.example.model.util.Coordinate;
import org.example.model.util.Direction;
import org.example.model.entity.*;
import org.example.model.entity.Character;

import java.util.*;

public class Dungeon {
    private Room[][] rooms;
    private final int level;
    private ArrayList<Room> sequence;
    private ArrayList<Corridor> corridors;
    private ArrayList<Enemy> enemies;
    //        return Collections.unmodifiableList(items);
    @Getter
    private ArrayList<Item> items;
    public UserHero hero;
    public Exit exit;

    public Dungeon(int level) {
        this.level = level;
    }

    public boolean isRoomExist(int x, int y) {
        return rooms[x][y] != null;
    }

    protected void addRoom(Room room, int x, int y) {
        rooms[x][y] = room;
        sequence.add(room);
    }

    public int getRoomCount() {
        return sequence.size();
    }

    public void sortRooms() {
        sequence.sort(Comparator.comparingInt(Room::getSector));
    }

    private boolean checkFreeCoordinate(int x, int y) {
        boolean isFree = true;
        for (int i = 0; i < enemies.size() && isFree; i++) {
            var entityCoord = enemies.get(i).getCoordinate();
            if (entityCoord.x == x && entityCoord.y == y) {
                isFree = false;
            }
        }
        return isFree;
    }

    public int getRoomCnt() {
        return sequence.size();
    }

    public Room getRoom(int i) {
        Room tmp = null;
        if (i + 1 <= sequence.size()) {
            tmp = sequence.get(i);
        }
        if (tmp == null) {
            throw new RuntimeException("index is incorrect");
        }
        return tmp;
    }

    public int getCorridorCnt() {
        return corridors.size();
    }

    public Corridor getCorridor(int i) {
        Corridor tmp = null;
        if (i + 1 <= corridors.size()) {
            tmp = corridors.get(i);
        }
        if (tmp == null) {
            throw new RuntimeException("index is incorrect");
        }
        return tmp;
    }

    public int getEnemiesCount() {
        return enemies.size();
    }

    public int getItemsCount() {
        return items.size();
    }

    public static class StaticDungeonGenerator {
        public static final int MAP_HEIGHT = 30;
        public static final int MAP_WIDTH = 90;
        public static final int ROOMS_PER_SIDE = 3;
        private static final int SECTOR_HEIGHT = MAP_HEIGHT / ROOMS_PER_SIDE;
        private static final int SECTOR_WIDTH = MAP_WIDTH / ROOMS_PER_SIDE;
        private static final double ROOM_CHANCE = 0.5;
        private static int MAX_ITEMS_PER_ROOM = 5;
        private static int MAX_ENEMIES_PER_ROOM = 5;
        public static final int MAX_ITEMS_TOTAL = MAX_ITEMS_PER_ROOM * ROOMS_PER_SIDE * ROOMS_PER_SIDE;
        public static final int MAX_ENEMIES_TOTAL = MAX_ENEMIES_PER_ROOM * ROOMS_PER_SIDE * ROOMS_PER_SIDE;

        private static Dungeon dungeon;

        public static Dungeon generatDungeon(int level) {
            dungeon = new Dungeon(level);
            MAX_ITEMS_PER_ROOM -= level / 4;
            MAX_ENEMIES_PER_ROOM = 1 + level;
            dungeon.rooms = new Room[ROOMS_PER_SIDE][ROOMS_PER_SIDE];
            dungeon.sequence = new ArrayList<>(ROOMS_PER_SIDE * ROOMS_PER_SIDE);
            dungeon.corridors = new ArrayList<>(12);
            dungeon.enemies = new ArrayList<>(MAX_ENEMIES_TOTAL + MAX_ITEMS_TOTAL + 2);
            dungeon.items = new ArrayList<>(MAX_ITEMS_TOTAL + MAX_ITEMS_TOTAL + 2);
            generateSectors();
            generateConnections();
            checkDoors();
            generateStartRoom();
            generateExit();
            generateEnemyAndItem();

            return dungeon;
        }

        private static void checkDoors() {
            for (int i = 0; i < dungeon.getRoomCount(); i++) {
                var room = dungeon.getRoom(i);
                for (int j = 0; j < room.getDoorCount(); j++) {
                    var doors = room.getAllDoors();
                    if (doors.get(j).isOpen() && doors.get(j).getConnectionCorridor() == null) {
                        doors.get(j).isOpen = false;
                    }
                }
            }
        }

        private static void generateSectors() {
            while (dungeon.getRoomCount() < 9) {
                var random = new Random();
                for (int i = 0, sector = 0; i < ROOMS_PER_SIDE; i++) {
                    for (int j = 0; j < ROOMS_PER_SIDE; j++, sector++) {
                        if (random.nextDouble(1) < ROOM_CHANCE && !dungeon.isRoomExist(i, j)) {
                            dungeon.addRoom(Room.StaticRoomGenerator.createRoom(sector, SECTOR_HEIGHT, SECTOR_WIDTH), i, j);
                        }
                    }
                }
            }
            dungeon.sortRooms();
        }

        private static void generateEnemyAndItem() {
            var random = new Random();
            int levelCoefficient = dungeon.level;
            for (int j = 0; j < dungeon.getRoomCount(); j++) {
                if (!dungeon.getRoom(j).isStartRoom()) {
                    var enemyInRoom = random.nextInt(MAX_ENEMIES_PER_ROOM) + 1;
                    for (int i = 0; i < enemyInRoom; i++) {
                        addEntity(StaticEnemyFactory.createRandomEnemy(generateEntityCoords(j), levelCoefficient), dungeon.getRoom(j));
                    }
                    var itemInRoom = random.nextInt(MAX_ITEMS_PER_ROOM);
                    for (var room : dungeon.rooms) {
                        for (int i = 0; i < itemInRoom; i++) {
                            addEntity(StaticItemFactory.createRandomItem(generateEntityCoords(j)), dungeon.getRoom(j));
                        }
                    }
                }
            }
        }

        private static void addEntity(Entity entity, Room room) {
            room.addEntity(entity);
            if (entity instanceof Item) {
                dungeon.items.add((Item) entity);
            } else if (entity instanceof Character) {
                dungeon.enemies.add((Enemy) entity);
                ((Enemy) entity).setLocation(room);
            }
        }

        private static Coordinate generateEntityCoords(int roomIndex) {
            var random = new Random();
            var room = dungeon.getRoom(roomIndex);
            int x, y;
            do {
                x = random.nextInt(room.getBottomRight().x - room.getTopLeft().x - 1) + 1 + room.getTopLeft().x;
                y = random.nextInt(room.getBottomRight().y - room.getTopLeft().y - 1) + 1 + room.getTopLeft().y;
            } while (!checkFreeCoordinate(x, y));
            return new Coordinate(x, y);
        }

        private static boolean checkFreeCoordinate(int x, int y) {
            boolean isFree = true;
            for (int i = 0; i < dungeon.getEnemiesCount() && isFree; i++) {
                var enemyCoord = dungeon.enemies.get(i).getCoordinate();
                if (enemyCoord.x == x && enemyCoord.y == y) {
                    isFree = false;
                }
            }
            for (int i = 0; i < dungeon.getItemsCount() && isFree; i++) {
                var itemCoord = dungeon.items.get(i).getCoordinate();
                if (itemCoord.x == x && itemCoord.y == y) {
                    isFree = false;
                }
            }
            return isFree;
        }

        private static void generateStartRoom() {
            var random = new Random();
            var roomIndex = random.nextInt(dungeon.getRoomCount() - 1);
            dungeon.sequence.get(roomIndex).setStartRoom();
            dungeon.hero = UserHero.getHero();
            dungeon.hero.setLocation(dungeon.sequence.get(roomIndex));
            dungeon.hero.setCoordinate(generateEntityCoords(roomIndex));
        }

        private static void generateExit() {
            var random = new Random();
            int roomIndex = -1;
            do {
                roomIndex = random.nextInt(dungeon.sequence.size() - 1);
            } while (dungeon.sequence.get(roomIndex).isStartRoom());
            Exit exit = new Exit(generateEntityCoords(roomIndex));
            dungeon.exit = exit;
            dungeon.sequence.get(roomIndex).setExit(exit);
            var hero = UserHero.getHero();
            ((Room) hero.getLocation()).setExit(new Exit(hero.getCoordinate()));
        }

        private static void generateConnections() {
            generatePrimaryConnections();
            generateSecondaryConnections();
            removingUnnecessaryConnect();
        }

        private static void generatePrimaryConnections() {
            for (int i = 0; i < ROOMS_PER_SIDE; i++) {
                for (int j = 0; j < ROOMS_PER_SIDE; j++) {
                    if (dungeon.rooms[i][j] != null) {
                        if (i != 0 && dungeon.rooms[i - 1][j] != null) {
                            if (dungeon.rooms[i][j].getDoor(Direction.TOP).isConnectionExist()) continue;
                            Corridor tmp = new Corridor(Corridor.corridorType.TopToBottom);
                            dungeon.corridors.add(tmp);
                            dungeon.rooms[i][j].addConnections(Direction.TOP, tmp);
                            dungeon.rooms[i - 1][j].addConnections(Direction.BOTTOM, tmp);
                            tmp.setRoomsCon(dungeon.rooms[i][j], dungeon.rooms[i - 1][j]);

                        }
                        if (j + 1 < ROOMS_PER_SIDE && dungeon.rooms[i][j + 1] != null) {
                            if (dungeon.rooms[i][j].getDoor(Direction.RIGHT).isConnectionExist()) continue;
                            Corridor tmp = new Corridor(Corridor.corridorType.LeftToRight);
                            dungeon.corridors.add(tmp);
                            dungeon.rooms[i][j].addConnections(Direction.RIGHT, tmp);
                            dungeon.rooms[i][j + 1].addConnections(Direction.LEFT, tmp);
                            tmp.setRoomsCon(dungeon.rooms[i][j], dungeon.rooms[i][j + 1]);
                        }
                        if (i + 1 < ROOMS_PER_SIDE && dungeon.rooms[i + 1][j] != null) {
                            if (dungeon.rooms[i][j].getDoor(Direction.BOTTOM).isConnectionExist()) continue;
                            Corridor tmp = new Corridor(Corridor.corridorType.TopToBottom);
                            dungeon.corridors.add(tmp);
                            dungeon.rooms[i][j].addConnections(Direction.BOTTOM, tmp);
                            dungeon.rooms[i + 1][j].addConnections(Direction.TOP, tmp);
                            tmp.setRoomsCon(dungeon.rooms[i][j], dungeon.rooms[i + 1][j]);
                        }
                        if (j != 0 && dungeon.rooms[i][j - 1] != null) {
                            if (dungeon.rooms[i][j].getDoor(Direction.LEFT).isConnectionExist()) continue;
                            Corridor tmp = new Corridor(Corridor.corridorType.LeftToRight);
                            dungeon.corridors.add(tmp);
                            dungeon.rooms[i][j].addConnections(Direction.LEFT, tmp);
                            dungeon.rooms[i][j - 1].addConnections(Direction.RIGHT, tmp);
                            tmp.setRoomsCon(dungeon.rooms[i][j], dungeon.rooms[i][j - 1]);
                        }
                    }
                }
            }
        }

        private static void generateSecondaryConnections() {
            for (int i = 0; i < dungeon.sequence.size() - 1; i++) {
                Room cur = dungeon.sequence.get(i);
                Room next = dungeon.sequence.get(i + 1);

                if (cur.getRowNumber() == next.getRowNumber() && !next.getDoor(Direction.LEFT).isConnectionExist()) {
                    Corridor tmp = new Corridor(Corridor.corridorType.LeftToRight);
                    dungeon.corridors.add(tmp);
                    cur.addConnections(Direction.RIGHT, tmp);
                    next.addConnections(Direction.LEFT, tmp);
                    tmp.setRoomsCon(cur, next);
                } else if (cur.getRowNumber() - next.getRowNumber() == -1 && !cur.getDoor(Direction.BOTTOM).isConnectionExist()) {
                    if (cur.getColumnNumber() < next.getColumnNumber() && !next.getDoor(Direction.LEFT).isConnectionExist()) {
                        Corridor tmp = new Corridor(Corridor.corridorType.RightTurn);
                        dungeon.corridors.add(tmp);
                        cur.addConnections(Direction.BOTTOM, tmp);
                        next.addConnections(Direction.LEFT, tmp);
                        tmp.setRoomsCon(cur, next);
                    } else if (cur.getColumnNumber() > next.getColumnNumber() && !next.getDoor(Direction.RIGHT).isConnectionExist()) {
                        Corridor tmp = new Corridor(Corridor.corridorType.LeftTurn);
                        dungeon.corridors.add(tmp);
                        cur.addConnections(Direction.BOTTOM, tmp);
                        next.addConnections(Direction.RIGHT, tmp);
                        tmp.setRoomsCon(cur, next);
                    } else if (cur.getColumnNumber() > next.getColumnNumber() && i < dungeon.rooms.length - 2) {
                        Corridor tmp = new Corridor(Corridor.corridorType.LeftTurn);
                        dungeon.corridors.add(tmp);
                        cur.addConnections(Direction.BOTTOM, tmp);
                        dungeon.sequence.get(i + 2).addConnections(Direction.RIGHT, tmp);
                        tmp.setRoomsCon(cur, dungeon.sequence.get(i + 2));
                    }
                } else if (cur.getRowNumber() - next.getRowNumber() == -2 && !next.getDoor(Direction.TOP).isConnectionExist()) {
                    Corridor tmp = new Corridor(Corridor.corridorType.TopToBottom);
                    dungeon.corridors.add(tmp);
                    cur.addConnections(Direction.BOTTOM, tmp);
                    next.addConnections(Direction.TOP, tmp);
                    tmp.setRoomsCon(cur, next);
                }
            }
        }

        private static void removingUnnecessaryConnect() {
            boolean tryAgane = false;
            do {
                var directions = Direction.values();
                tryAgane = false;
                for (int i = 0; i < dungeon.sequence.size(); i++) {
                    Room cur = dungeon.sequence.get(i);
                    if (cur.getConnectCount() <= 2) {
                        continue;
                    }
                    int index = Direction.randomDirection().ordinal();
                    boolean endSearch = false;
                    for (int j = 0; j < directions.length && !endSearch; j++) {
                        index = (index + j) % directions.length;
                        if (cur.getDoor(directions[index]) == null) {
                            continue;
                        }
                        var corridor = cur.getDoor(directions[index]).getConnectionCorridor();
                        if (corridor != null && isHaveMoreConnections(corridor.getOutputRoom(cur), corridor.getOutputDoor(cur), cur)) {
                            endSearch = true;
                            tryAgane = true;
                            corridor.getOutputDoor(cur).setConnectionCorridor(null);
                            cur.getDoor(directions[index]).setConnectionCorridor(null);
                            dungeon.corridors.remove(corridor);
                        }
                    }
                }
            } while (tryAgane);
        }

        private static boolean isHaveMoreConnections(Room cur, Door inputDoor, Room start) {
            var roomCnt = cur.getConnectCount();
            boolean isMoreCon = false;
            if (roomCnt == 2 && start != cur) {
                var doors = cur.getAllDoors();
                Door newDoor = null;
                for (Door door : doors) {
                    if (door.getConnectionCorridor() != null && door != inputDoor) {
                        newDoor = door;
                    }
                }
                assert newDoor != null;
                var tmp = newDoor.getConnectionCorridor();
                isMoreCon = isHaveMoreConnections(tmp.getOutputRoom(cur), tmp.getOutputDoor(cur), start);
            } else if (start == cur) {
                isMoreCon = true;
            } else {
                isMoreCon = roomCnt != 1;
            }

            return isMoreCon;
        }

    }
}

