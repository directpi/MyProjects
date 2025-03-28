package org.example.model;

import lombok.Getter;
import org.example.model.entity.UserHero;
import org.example.model.entity.item.Elixir;

public class ActiveElixir {

    private final Elixir.TypeElixir type;
    @Getter
    private int duration;
    private int strength;
    private int dexterity;
    private int health;

    {
        duration = 0;
        strength = 0;
        dexterity = 0;
        health = 0;
    }

    public ActiveElixir(Elixir.TypeElixir type, int characteristic, int duration) {
        this.duration = duration;
        this.type = type;
        switch (type) {
            case STRENGTH -> strength = characteristic;
            case DEXTERITY -> dexterity = characteristic;
            case HEALTH -> health = characteristic;
        }
    }

    public void decreaseDuration() {
        if (duration <= 0) {
            return;
        }
        duration--;
        if (duration == 0) {
            removeEffect();
        }
    }

    public void removeEffect() {
        var hero = UserHero.getHero();
        switch (type) {
            case STRENGTH -> hero.decreaseStrength(strength);
            case DEXTERITY -> hero.decreaseDexterity(dexterity);
            case HEALTH -> hero.decreaseHealth(health);
        }
    }

    public Elixir.TypeElixir getTypeElixir() {
        return type;
    }

    public int getCharacteristic() {
        switch (type) {
            case STRENGTH -> {
                return strength;
            }
            case DEXTERITY -> {
                return dexterity;
            }
            case HEALTH -> {
                return health;
            }
            default -> {
                return 0;
            }
        }
    }
}
