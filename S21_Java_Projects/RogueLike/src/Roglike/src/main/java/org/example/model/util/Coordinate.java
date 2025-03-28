package org.example.model.util;

public class Coordinate {
    public int x;
    public int y;

    public Coordinate() {
        x = 0;
        y = 0;
    }

    public Coordinate(int x, int y) {
        this.x = x;
        this.y = y;
    }

    public Coordinate(Coordinate cor) {
        this.y = cor.y;
        this.x = cor.x;
    }

    public void setCoordinate(Coordinate cor) {
        this.x = cor.x;
        this.y = cor.y;
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) {
            return true;
        }
        if (obj == null || this.getClass() != obj.getClass()) {
            return false;
        }
        var tmp = (Coordinate) obj;
        return tmp.x == this.x && tmp.y == this.y;
    }

    @Override
    public int hashCode() {
        return this.getClass().hashCode() + this.x + this.y;
    }

    public int distanceTo(Coordinate other) {
        return Math.abs(this.x - other.x) + Math.abs(this.y - other.y);
    }
}
