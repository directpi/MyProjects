package org.example.model.dungeon;

import org.example.model.util.Coordinate;

public class Door {
    public static final char SKIN_1 = '-';
    public static final char SKIN_2 = '|';

    private Coordinate coordinate;
    private Corridor corridor;

    boolean isOpen;

    {
        isOpen = false;
    }

    public Door(Coordinate coordinate) {
        this.coordinate = new Coordinate(coordinate);
        corridor = null;
    }

    public boolean isConnectionExist(){
        return corridor != null;
    }
    public Coordinate getCoordinate() {
        return new Coordinate(coordinate);
    }
    public void setCoordinate(Coordinate coordinate) {
        this.coordinate.setCoordinate(coordinate);
    }
    public Corridor getConnectionCorridor() {
        return corridor;
    }
    public void setConnectionCorridor(Corridor corridor) {
        this.corridor = corridor;
        isOpen = true;
    }
    public boolean isOpen() {
        return isOpen;
    }
}
