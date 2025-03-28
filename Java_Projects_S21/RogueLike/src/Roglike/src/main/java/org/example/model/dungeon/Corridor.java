package org.example.model.dungeon;

import org.example.model.util.Coordinate;
import org.example.model.util.Direction;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class Corridor  implements LabyrinthPart {
    public enum corridorType {LeftTurn, RightTurn, TopToBottom, LeftToRight}
    public static char SKIN = '#';

    private corridorType type;
    private ArrayList<Coordinate> points = new ArrayList<>(4);
    Room connectionRoom1;
    Room connectionRoom2;
    Door door1;
    Door door2;

    public Corridor(corridorType type) {
        this.type = type;
    }

    public void setRoomsCon(Room connectionRoom1, Room connectionRoom2) {
        this.connectionRoom1 = connectionRoom1;
        this.connectionRoom2 = connectionRoom2;
        Direction direction1;
        Direction direction2;
        Direction room1DoorDir = null;
        Direction room2DoorDir = null;

        if (type == corridorType.LeftToRight || type == corridorType.TopToBottom) {
            if (type == corridorType.TopToBottom) {
                direction1 = Direction.BOTTOM;
                direction2 = Direction.TOP;
            } else {
                direction1 = Direction.RIGHT;
                direction2 = Direction.LEFT;
            }
            room1DoorDir = (connectionRoom1.getSector() < connectionRoom2.getSector()) ? direction1 : direction2;
            room2DoorDir = (room1DoorDir == direction1) ? direction2 : direction1;

        } else if (type == corridorType.RightTurn || type == corridorType.LeftTurn) {
            direction1 = Direction.BOTTOM;
            if (type == corridorType.RightTurn) {
                direction2 = Direction.LEFT;
            } else {
                direction2 = Direction.RIGHT;
            }
            room1DoorDir = (connectionRoom1.getSector() < connectionRoom2.getSector()) ? direction1 : direction2;
            room2DoorDir = (room1DoorDir == direction1) ? direction2 : direction1;
        }
        this.door1 = (room1DoorDir!=null) ? connectionRoom1.getDoor(room1DoorDir) :null;
        this.door2 = (room1DoorDir!=null) ? connectionRoom2.getDoor(room2DoorDir) :null;
        if (this.door1 == null || this.door2 == null) {
            throw new IllegalStateException("Error. One or both doors is null");
        }
        switch (type) {
            case TopToBottom: generateTopToBottomCorridor(); break;
            case LeftToRight: generateLeftToRightCorridor(); break;
            case RightTurn: generateRightTurnCorridor(); break;
            case LeftTurn: generateLeftTurnCorridor(); break;
        }
    }


    public Coordinate getCoordinate(int i) {
        if (i < 0  || i + 1 > points.size()) {
            throw new IndexOutOfBoundsException("Incorrect index: " + i);
        }
        return new Coordinate(points.get(i));
    }

    public corridorType getCorridorType() {
        return type;
    }

    public ArrayList<Coordinate> getPoints() {
        return new ArrayList<>(points);
    }
    public Room getOutputRoom(Room inputRoom) {
        return (connectionRoom1 == inputRoom) ? connectionRoom2 : connectionRoom1;
    }
    public Door getOutputDoor(Room inputRoom) {
        return  (connectionRoom1 == inputRoom) ? door2 : door1;
    }
    public List<Door> getDoors(){
        return List.of(door1, door2);
    }

    private void generateTopToBottomCorridor() {
        var topRoom = (connectionRoom1.getSector() < connectionRoom2.getSector()) ? connectionRoom1 : connectionRoom2;
        var bottomRoom = (topRoom == connectionRoom1) ? connectionRoom2 : connectionRoom1;
        points.add(new Coordinate(topRoom.getDoor(Direction.BOTTOM).getCoordinate()));

        int yTop = topRoom.getDoor(Direction.BOTTOM).getCoordinate().y;
        int yBottom = bottomRoom.getDoor(Direction.TOP).getCoordinate().y;
        var random = new Random();
        int check_range = yBottom- yTop + 1;
        int randomCenterY = (check_range>0) ? random.nextInt(yBottom - yTop - 1) + yTop + 1 : yTop;

        points.add(new Coordinate(topRoom.getDoor(Direction.BOTTOM).getCoordinate().x, randomCenterY));
        points.add(new Coordinate(bottomRoom.getDoor(Direction.TOP).getCoordinate().x, randomCenterY));
        points.add(bottomRoom.getDoor(Direction.TOP).getCoordinate());
    }
    private void generateLeftToRightCorridor() {
        var leftRoom = (connectionRoom1.getSector() < connectionRoom2.getSector()) ? connectionRoom1 : connectionRoom2;
        var rightRoom = (leftRoom == connectionRoom1) ? connectionRoom2 : connectionRoom1;
        points.add(new Coordinate(leftRoom.getDoor(Direction.RIGHT).getCoordinate()));

        int xLeft = leftRoom.getDoor(Direction.RIGHT).getCoordinate().x;
        int xRight = rightRoom.getDoor(Direction.LEFT).getCoordinate().x;
        var random = new Random();
        int randomCenterX = random.nextInt(xRight - xLeft - 1) + xLeft + 1;

        points.add(new Coordinate(randomCenterX, leftRoom.getDoor(Direction.RIGHT).getCoordinate().y));
        points.add(new Coordinate(randomCenterX, rightRoom.getDoor(Direction.LEFT).getCoordinate().y));
        points.add(rightRoom.getDoor(Direction.LEFT).getCoordinate());
    }
    private void generateLeftTurnCorridor() {
        var topRoom = (connectionRoom1.getSector() < connectionRoom2.getSector()) ? connectionRoom1 : connectionRoom2;
        var bottomRoom = (topRoom == connectionRoom1) ? connectionRoom2 : connectionRoom1;
        points.add(topRoom.getDoor(Direction.BOTTOM).getCoordinate());
        points.add(new Coordinate(topRoom.getDoor(Direction.BOTTOM).getCoordinate().x, bottomRoom.getDoor(Direction.RIGHT).getCoordinate().y));
        points.add(new Coordinate(bottomRoom.getDoor(Direction.RIGHT).getCoordinate()));
    }
    private void generateRightTurnCorridor() {
        var topRoom = (connectionRoom1.getSector() < connectionRoom2.getSector()) ? connectionRoom1 : connectionRoom2;
        var bottomRoom = (topRoom == connectionRoom1) ? connectionRoom2 : connectionRoom1;
        points.add(new Coordinate(topRoom.getDoor(Direction.BOTTOM).getCoordinate()));
        points.add(new Coordinate(topRoom.getDoor(Direction.BOTTOM).getCoordinate().x, bottomRoom.getDoor(Direction.LEFT).getCoordinate().y));
        points.add(new Coordinate(bottomRoom.getDoor(Direction.LEFT).getCoordinate()));
    }
}
