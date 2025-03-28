package org.example.model.entity;


import com.fasterxml.jackson.annotation.JsonIgnore;
import lombok.Getter;
import lombok.Setter;
import org.example.model.util.Coordinate;

@Getter
public abstract class Entity {
    protected Health health;
    @Setter
    protected int speed;
    protected int dexterity;
    protected int strength;
    @JsonIgnore
    protected Coordinate coordinate;
    @Setter
    protected char skin;


    public Entity(Health health, int dexterity, int speed, int strength, char skin) {
        this.health = health;
        this.dexterity = dexterity;
        this.speed = speed;
        this.strength = strength;
        this.skin = skin;
        coordinate = new Coordinate();
    }

    public Entity() {
        this.health = new Health();
        this.dexterity = 0;
        this.speed = 0;
        this.strength = 0;
        coordinate = new Coordinate();
    }

//    public Coordinate getCoordinate() {
//        return new Coordinate(coordinate);
//    }

    public void setCoordinate(Coordinate coordinate) {
        setCoorY(coordinate.y);
        setCoorX(coordinate.x);
    }

    public void setCoorX(int x) {
        this.coordinate.x = x;
    }

    public void setCoorY(int y) {
        this.coordinate.y = y;
    }

}
