package org.example.model.dungeon;

import lombok.Getter;
import org.example.model.entity.UserHero;
import org.example.model.util.Coordinate;
import org.example.model.util.Direction;

public class Field {
    private char[][] field;
    @Getter
    private char[][] shadowField;
    private Dungeon dungeon;

    {
        field = new char[Dungeon.StaticDungeonGenerator.MAP_HEIGHT][Dungeon.StaticDungeonGenerator.MAP_WIDTH];
        shadowField = new char[Dungeon.StaticDungeonGenerator.MAP_HEIGHT][Dungeon.StaticDungeonGenerator.MAP_WIDTH];
        for (int i = 0; i < Dungeon.StaticDungeonGenerator.MAP_HEIGHT; i++) {
            for (int j = 0; j < Dungeon.StaticDungeonGenerator.MAP_WIDTH; j++) {
                shadowField[i][j] = ' ';
                field[i][j] = ' ';
            }
        }
    }

    public Field(Dungeon dungeon) {
        this.dungeon = dungeon;
        dungeonToField();
        initField();
    }

    public char[][] getField() {
        var tmp = new char[Dungeon.StaticDungeonGenerator.MAP_HEIGHT][Dungeon.StaticDungeonGenerator.MAP_WIDTH];
        if(UserHero.getHero().getLocation() instanceof Room)
            removeFogRoom((Room)UserHero.getHero().getLocation());
        removeFogCorridor();
        for (int i = 0; i < Dungeon.StaticDungeonGenerator.MAP_HEIGHT; i++) {
            System.arraycopy(field[i], 0, tmp[i], 0, field[i].length);
        }
        entityToMap(tmp);
        tmp[UserHero.getHero().getCoordinate().y][UserHero.getHero().getCoordinate().x] = '@';

        return tmp;
    }
    public void removeFogRoom(Room room) {
        roomToField(room,field);
    }
    public void removeFogCorridor(){
        var hero = UserHero.getHero();
        for (int i = -1; i < 2; i++) {
            for (int j = -1; j < 2; j++) {
                if(i == 0 && j == 0) continue;
                if(shadowField[hero.getCoordinate().y + i][hero.getCoordinate().x + j] != '#'){
                    field[hero.getCoordinate().y + i][hero.getCoordinate().x + j] = shadowField[hero.getCoordinate().y + i][hero.getCoordinate().x + j];
                }
            }
        }
    }

    private void dungeonToField(){
        corridorsToField();
        roomsToField();
    }
    private void roomsToField() {
        var _field = shadowField;
        for (int i = 0; i < dungeon.getRoomCnt(); i++) {
            var room = dungeon.getRoom(i);
            roomToField(room, _field);
        }
    }
    private void roomToField(Room room, char[][] field) {
        Coordinate bottomRight = room.getBottomRight();
        Coordinate topLeft = room.getTopLeft();
        rectangleToField(topLeft, bottomRight, field);
        fillRectangle(topLeft, bottomRight, field);
        paintingRoom(topLeft, bottomRight, field);
        doorToField(room, field);
    }
    private void fillRectangle(Coordinate top, Coordinate bot, char[][] field) {
        for (int i = top.y + 1; i < bot.y; i++)
            for (int j = top.x + 1; j < bot.x; j++)
                field[i][j] = ' ';
    }
    private void rectangleToField(Coordinate top, Coordinate bot, char[][] field) {

        for (int i = top.x; i <= bot.x; i++) {
            field[top.y][i] = '#';
            field[bot.y][i] = '#';
        }

        for (int i = top.y; i <= bot.y; i++) {
            field[i][top.x] = '#';
            field[i][bot.x] = '#';
        }
    }
    private void paintingRoom(Coordinate top, Coordinate bot, char[][] field){
        for (int i = top.x + 1; i < bot.x; i++) {
            for (int j = top.y + 1; j < bot.y; j++) {
                field[j][i] = '.';
            }
        }
    }
    private void corridorsToField() {
        var _field = shadowField;
        for (int i = 0; i < dungeon.getCorridorCnt(); i++) {
            corridorToField(dungeon.getCorridor(i), _field);
        }
    }
    private void corridorToField(Corridor corridor, char[][] field){
        var corridorType = corridor.getCorridorType();
        if (corridorType == Corridor.corridorType.LeftTurn || corridorType == Corridor.corridorType.RightTurn) {
            verticalLine(corridor.getCoordinate(0), corridor.getCoordinate(1), field);
            horizontalLine(corridor.getCoordinate(1), corridor.getCoordinate(2), field);
        } else if (corridorType == Corridor.corridorType.LeftToRight) {
            horizontalLine(corridor.getCoordinate(0), corridor.getCoordinate(1), field);
            verticalLine(corridor.getCoordinate(1), corridor.getCoordinate(2), field);
            horizontalLine(corridor.getCoordinate(2), corridor.getCoordinate(3), field);
        } else if (corridorType == Corridor.corridorType.TopToBottom) {
            verticalLine(corridor.getCoordinate(0), corridor.getCoordinate(1), field);
            horizontalLine(corridor.getCoordinate(1), corridor.getCoordinate(2), field);
            verticalLine(corridor.getCoordinate(2), corridor.getCoordinate(3), field);
        }
    }
    private void verticalLine(Coordinate first, Coordinate second, char[][] field) {
        for (int y = Math.min(first.y, second.y); y <= Math.max(first.y, second.y); y++) {
            field[y][first.x] = '+';
        }
    }
    private void horizontalLine(Coordinate first, Coordinate second, char[][] field) {
        for (int x = Math.min(first.x, second.x); x <= Math.max(first.x, second.x); x++) {
            field[first.y][x] = '+';
        }
    }
    private void doorToField(Room room, char[][] field) {
        var door = room.getDoor(Direction.TOP);
        if (door != null) {
            field[door.getCoordinate().y][door.getCoordinate().x] = (door.isOpen()) ?'|' : '-';
        }
        door = room.getDoor(Direction.RIGHT);
        if (door != null) {
            field[door.getCoordinate().y][door.getCoordinate().x] = (door.isOpen()) ?'-' : '|';
        }
        door = room.getDoor(Direction.BOTTOM);
        if (door != null) {
            field[door.getCoordinate().y][door.getCoordinate().x] = (door.isOpen()) ?'|' : '-';
        }
        door = room.getDoor(Direction.LEFT);
        if (door != null) {
            field[door.getCoordinate().y][door.getCoordinate().x] = (door.isOpen()) ?'-' : '|';
        }
    }
    private void initField(){
        roomToField((Room) UserHero.getHero().getLocation(), field);
    }

    private void entityToMap(char[][] _field) {
        var hero = UserHero.getHero();
        if (hero.getLocation() instanceof Room) {
            var room = (Room) hero.getLocation();
            for (var enemie : room.getEnemies()) {
                _field[enemie.getCoordinate().y][enemie.getCoordinate().x] = enemie.getSkin();
            }
            for (var item : room.getItems()) {
                _field[item.getCoordinate().y][item.getCoordinate().x] = item.getSkin();
            }
            if(room.getExit() != null) {
                _field[room.getExit().getCoordinate().y][room.getExit().getCoordinate().x] = room.getExit().getSkin();
            }
        }
    }
    private  void statusToField() {

    }
}
