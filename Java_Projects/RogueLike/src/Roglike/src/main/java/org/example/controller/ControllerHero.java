package org.example.controller;


import lombok.Getter;
import org.example.datalayer.GameStatistic;
import org.example.model.ActiveElixir;
import org.example.model.GameModel;
import org.example.model.Screen;
import org.example.model.entity.*;
import org.example.model.dungeon.*;
import org.example.model.entity.item.*;
import org.example.model.util.*;

import java.util.Iterator;
import java.util.List;

public class ControllerHero {
    private enum CellType {
        FREE, WALL, ENEMY, ITEM, DOOR, EXIT
    }

    ConsoleMessageHandler handler = new ConsoleMessageHandler();

    public enum MoveResult {OK, NEXT_LEVEL}

    @Getter
    private CellType oldCellType;
    private final GameModel model;


    public ControllerHero(GameModel model) {
        this.model = model;
        oldCellType = CellType.FREE;
    }

    public MoveResult movePlayer(Direction direction) {

        var hero = UserHero.getHero();
        var resultMove = MoveResult.OK;
        var newCoord = new Coordinate(hero.getCoordinate().x, hero.getCoordinate().y);
        switch (direction) {
            case TOP -> newCoord.y--;
            case BOTTOM -> newCoord.y++;
            case RIGHT -> newCoord.x++;
            case LEFT -> newCoord.x--;
        }
        var typeCell = checkingNextCell(newCoord);
        switch (typeCell) {
            case FREE -> freeCell(newCoord);
            case WALL -> {
            }
            case ITEM -> itemCell(newCoord);
            case DOOR -> doorCell(newCoord);
            case EXIT -> resultMove = MoveResult.NEXT_LEVEL;
            case ENEMY -> hero.attack(findEnemy(newCoord));
        }
        activeElixirTic();
        if (!(hero.getLocation() instanceof Room)) {
            Screen.addMessage(""); // Добавляем пустое сообщение для "ленты сообщений"
        }
        return resultMove;
    }

    public static void useItem(Item item) {
        var hero = UserHero.getHero();
        if (!(item instanceof Weapon) && item.useItem()) {
            hero.getBackpack().getItems().get(item.getClass().getSimpleName()).remove(item);
        } else if ((item instanceof Weapon)) {
            if (hero.getEquippedWeapon() != null) {
                if (!hero.dropItem(hero.getEquippedWeapon())) {
                    return;
                }
            }
            if (item.useItem()) {
                hero.getBackpack().getItems().get(item.getClass().getSimpleName()).remove(item);
            }
        }
    }

    private void activeElixirTic() {
        var activeElixirs = UserHero.getHero().getActiveElixir();
        Iterator<ActiveElixir> iterator = activeElixirs.iterator();
        while (iterator.hasNext()) {
            var activeElixir = iterator.next();
            activeElixir.decreaseDuration();
            if (activeElixir.getDuration() <= 0) {
                iterator.remove();
            }
        }
    }

    private CellType checkingNextCell(Coordinate coordinate) {
        var result = CellType.FREE;
        if (isDoor(coordinate)) {
            result = CellType.DOOR;
        } else if (isWall(coordinate)) {
            result = CellType.WALL;
        } else if (isEnemy(coordinate)) {
            result = CellType.ENEMY;
        } else if (isItem(coordinate)) {
            result = CellType.ITEM;
        } else if (isExit(coordinate)) {
            result = CellType.EXIT;
        }
        return result;
    }

    private boolean isWall(Coordinate coordinate) {
        var location = UserHero.getHero().getLocation();
        boolean result = false;
        if (location instanceof Room) {
            result = inRoomCoord(((Room) location), coordinate);
            if (!result) {
                result = outRoomCoord(((Room) location), coordinate);
            }
        } else if (location instanceof Corridor) {
            result = inCorridorCoord(((Corridor) location), coordinate);
        }
        return !result;
    }

    private boolean inRoomCoord(Room room, Coordinate coordinate) {
        var topLeft = room.getTopLeft();
        var botRight = room.getBottomRight();
        return topLeft.y < coordinate.y && botRight.y > coordinate.y && topLeft.x < coordinate.x && botRight.x > coordinate.x;
    }

    private boolean outRoomCoord(Room room, Coordinate coordinate) {
        var topLeft = room.getTopLeft();
        var botRight = room.getBottomRight();
        return (topLeft.y > coordinate.y) || (topLeft.x > coordinate.x) || (botRight.y < coordinate.y) || (botRight.x < coordinate.x);
    }

    private boolean inCorridorCoord(Corridor corridor, Coordinate coordinate) {
        var points = corridor.getPoints();
        var typeCorridor = corridor.getCorridorType();
        boolean result = false;
        if (typeCorridor == Corridor.corridorType.RightTurn) {
            if (coordinate.x == points.getFirst().x) {
                result = coordinate.y >= points.getFirst().y && coordinate.y <= points.get(1).y;
            } else if (coordinate.x > points.getFirst().x && coordinate.x <= points.getLast().y) {
                result = coordinate.y == points.getLast().y;
            }
        } else if (typeCorridor == Corridor.corridorType.LeftTurn) {
            if (coordinate.x == points.getFirst().x) {
                result = coordinate.y >= points.getFirst().y && coordinate.y <= points.get(1).y;
            } else if (coordinate.x < points.getFirst().x && coordinate.x >= points.getLast().x) {
                result = coordinate.y == points.getLast().y;
            }
        } else if (typeCorridor == Corridor.corridorType.LeftToRight) {
            if (points.getFirst().y == points.getLast().y) {
                result = coordinate.y == points.getFirst().y && coordinate.x >= points.getFirst().x && coordinate.x <= points.getLast().x;
            } else if (coordinate.x >= points.getFirst().x && coordinate.x < points.get(1).x) {
                result = coordinate.y == points.getFirst().y;
            } else if (coordinate.x == points.get(1).x) {
                if (points.get(1).y > points.get(2).y) {
                    result = coordinate.y <= points.get(1).y && coordinate.y >= points.get(2).y;
                } else {
                    result = coordinate.y >= points.get(1).y && coordinate.y <= points.get(2).y;
                }
            } else if (coordinate.x > points.get(2).x && coordinate.x <= points.getLast().x) {
                result = coordinate.y == points.getLast().y;
            }
        } else if (typeCorridor == Corridor.corridorType.TopToBottom) {
            if (points.getFirst().x == points.getLast().x) {
                result = coordinate.x == points.getFirst().x && coordinate.y >= points.getFirst().y && coordinate.y <= points.getLast().y;
            } else if (coordinate.y >= points.getFirst().y && coordinate.y < points.get(1).y) {
                result = coordinate.x == points.getFirst().x;
            } else if (coordinate.y == points.get(1).y) {
                if (points.get(1).x > points.get(2).x) {
                    result = coordinate.x <= points.get(1).x && coordinate.x >= points.get(2).x;
                } else {
                    result = coordinate.x >= points.get(1).x && coordinate.x <= points.get(2).x;
                }
            } else if (coordinate.y > points.get(2).y && coordinate.y <= points.getLast().y) {
                result = coordinate.x == points.getLast().x;
            }
        }
        return result;
    }

    private boolean isDoor(Coordinate coordinate) {
        return findDoor(coordinate) != null;
    }

    private Door findDoor(Coordinate coordinate) {
        var location = UserHero.getHero().getLocation();
        Door result = null;
        if (location instanceof Room) {
            result = findDoorInList(((Room) location).getAllDoors(), coordinate);
        } else if (location instanceof Corridor) {
            result = findDoorInList(((Corridor) location).getDoors(), coordinate);
        }
        return result;
    }

    private Door findDoorInList(List<Door> doors, Coordinate coordinate) {
        Door result = null;
        boolean doorFound = false;
        for (int i = 0; i < doors.size() && !doorFound; i++) {
            var coord = doors.get(i).getCoordinate();
            if (coord.x == coordinate.x && coord.y == coordinate.y) {
                result = doors.get(i);
                doorFound = true;
            }
        }
        return result;
    }

    private boolean isItem(Coordinate coordinate) {
        return findItem(coordinate) != null;
    }

    private Item findItem(Coordinate coordinate) {
        var location = UserHero.getHero().getLocation();
        Item result = null;
        if (location instanceof Room) {
            var items = ((Room) location).getItems();
            boolean itemFound = false;
            for (int i = 0; i < items.size() && !itemFound; i++) {
                var coord = items.get(i).getCoordinate();
                if (coord.x == coordinate.x && coord.y == coordinate.y) {
                    result = items.get(i);
                    itemFound = true;
                }
            }
        }
        return result;
    }

    private boolean isEnemy(Coordinate coordinate) {
        return findEnemy(coordinate) != null;
    }

    private Enemy findEnemy(Coordinate coordinate) {
        var location = UserHero.getHero().getLocation();
        Enemy result = null;
        if (location instanceof Room) {
            var enemies = ((Room) location).getEnemies();
            boolean enemyFound = false;
            for (int i = 0; i < enemies.size() && !enemyFound; i++) {
                var coord = enemies.get(i).getCoordinate();
                if (coord.x == coordinate.x && coord.y == coordinate.y) {
                    result = enemies.get(i);
                    enemyFound = true;
                }
            }
        }
        return result;
    }

    private boolean isExit(Coordinate coordinate) {
        var location = UserHero.getHero().getLocation();
        boolean result = false;
        if (location instanceof Room) {
            var exit = ((Room) location).getExit();
            result = exit != null && exit.getCoordinate().equals(coordinate);
        }
        return result;
    }

    private void doorCell(Coordinate coordinate) {
        var door = findDoor(coordinate);
        if (door.isOpen()) {
            var hero = UserHero.getHero();
            hero.setCoordinate(coordinate);
            oldCellType = CellType.DOOR;
            if (hero.getLocation() instanceof Corridor) {
                hero.setLocation(findRoom(door));
            }
            GameStatistic.addCellRoute();
        }
    }

    private Room findRoom(Door _door) {
        boolean isFind = false;
        Room resRoom = null;
        for (int i = 0; i < model.getDungeon().getRoomCnt() && !isFind; i++) {
            var room = model.getDungeon().getRoom(i);
            for (int j = 0; j < room.getDoorCount() && !isFind; j++) {
                var doors = room.getAllDoors();
                if (doors.get(j) == _door) {
                    isFind = true;
                    resRoom = room;
                }
            }
        }
        return resRoom;
    }

    private void itemCell(Coordinate coordinate) {
        var hero = UserHero.getHero();
        if (hero.pickUpItem(findItem(coordinate))) {
            var item = findItem(coordinate);
            model.getDungeon().getItems().remove(item);
            ((Room) hero.getLocation()).getItems().remove(item);

            String text = switch (item) {
                case Elixir elixir -> "The elixir has been found";
                case Food food -> "Food found";
                case Scroll scroll -> "A scroll has been found";
                case Treasure treasure -> String.format("Gold found (cost: %d)", treasure.getPrice());
                case Weapon weapon -> "A weapon was found";
                default -> null;
            };

            if (text != null) {
                handler.sendMessage(text);
            }
        }
        freeCell(coordinate);
        GameStatistic.addCellRoute();
    }

    private void freeCell(Coordinate coordinate) {
        var hero = UserHero.getHero();
        if (oldCellType == CellType.DOOR) {
            var newLocation = newLocation(coordinate);
            if (newLocation != hero.getLocation()) {
                oldCellType = CellType.FREE;
                hero.setLocation(newLocation);
            }
        }
        hero.setCoordinate(coordinate);
        GameStatistic.addCellRoute();
    }

    private LabyrinthPart newLocation(Coordinate coordinate) {
        boolean isFind = false;
        LabyrinthPart newLocation = null;
        for (int i = 0; i < model.getDungeon().getRoomCnt() && !isFind; i++) {
            var room = model.getDungeon().getRoom(i);
            if (inRoomCoord(room, coordinate)) {
                isFind = true;
                newLocation = model.getDungeon().getRoom(i);
            }
        }
        for (int i = 0; i < model.getDungeon().getCorridorCnt() && !isFind; i++) {
            var corridor = model.getDungeon().getCorridor(i);
            if (inCorridorCoord(corridor, coordinate)) {
                isFind = true;
                newLocation = model.getDungeon().getCorridor(i);
            }
        }
        return newLocation;
    }

    public static Backpack getBackpack() {
        return UserHero.getHero().getBackpack();
    }

    public boolean isHeroDead() {
        return UserHero.getHero().getHealth().isDead();
    }
}