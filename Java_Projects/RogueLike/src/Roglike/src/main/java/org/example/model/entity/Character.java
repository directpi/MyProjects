package org.example.model.entity;

import com.fasterxml.jackson.annotation.JsonIgnore;
import lombok.Getter;
import lombok.Setter;
import org.example.model.util.Coordinate;
import org.example.model.util.Direction;
import org.example.model.dungeon.LabyrinthPart;

@Setter
@Getter
public abstract class Character extends Entity {
    @JsonIgnore
    protected LabyrinthPart location;

    // Константы для логики боя
    private static final double CRITICAL_HIT_CHANCE = 0.1; // 10% шанс критического удара
    private static final int BASE_HIT_CHANCE = 50; // Базовый шанс попадания
    private static final int MIN_HIT_CHANCE = 10;
    private static final int MAX_HIT_CHANCE = 90;

    public Character(Health health, int dexterity, int speed, int strength, char skin) {
        super(health, dexterity, speed, strength, skin);
    }

    public void move(Direction direction) {

        switch (direction) {
            case TOP -> coordinate.y--;
            case BOTTOM -> coordinate.y++;
            case LEFT -> coordinate.x--;
            case RIGHT -> coordinate.x++;
        }
    }

    public abstract void attack(Character target);


    public boolean isAdjacent(Coordinate coord1, Coordinate coord2) {
        int dx = Math.abs(coord1.x - coord2.x);
        int dy = Math.abs(coord1.y - coord2.y);
        return dx <= 1 && dy <= 1; // Персонажи находятся на соседних клетках
    }

    protected int calculateDamage(int baseDamage, int extraValue) {
        double criticalChance = 0.1; // 10% шанс на критический удар
        if (Math.random() < CRITICAL_HIT_CHANCE) {
            return (baseDamage + extraValue) * 2;
        }
        return baseDamage + extraValue;
    }

    protected boolean canHit(int attackPower, int targetDexterity) {
        double hitChance = BASE_HIT_CHANCE + (attackPower - targetDexterity);
        hitChance = Math.max(MIN_HIT_CHANCE, Math.min(MAX_HIT_CHANCE, hitChance));// Ограничиваем шанс попадания
        return Math.random() * 100 < hitChance;
    }
}
