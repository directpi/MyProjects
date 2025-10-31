package org.example.model.entity.item;

import com.fasterxml.jackson.annotation.JsonProperty;
import lombok.NoArgsConstructor;
import org.example.datalayer.GameStatistic;
import org.example.model.entity.Item;
import org.example.model.entity.UserHero;
import org.example.model.util.Coordinate;

@NoArgsConstructor
public class Elixir extends Item {
    public enum TypeElixir {STRENGTH, DEXTERITY, HEALTH}
    @JsonProperty
    private TypeElixir type;
    @JsonProperty
    private int duration;

    public Elixir(Coordinate coordinate) {
        super(coordinate);
        int value = rand.nextInt(4) + 1;
        int dur = rand.nextInt(15) + 1;
        switch (rand.nextInt(3)) {
            case 0 -> {
                dexterity = value;
                duration = dur;
                type = TypeElixir.DEXTERITY;
            }
            case 1 -> {
                strength = value;
                duration = dur;
                type = TypeElixir.STRENGTH;
            }
            case 2 -> {
                health.setMaxHealth(value);
                duration = dur;
                type = TypeElixir.HEALTH;
            }
        }
    }

    @Override
    public boolean useItem() {
        var hero = UserHero.getHero();

        GameStatistic.addDrinkingElixir();

        switch (type) {
            case STRENGTH -> {
                hero.addActiveElixir(type, duration, this.strength);
                hero.addStrength(strength);
            }
            case DEXTERITY -> {
                hero.addActiveElixir(type, duration, this.dexterity);
                hero.addDexterity(dexterity);
            }
            case HEALTH -> {
                hero.addActiveElixir(type, duration, this.health.getMaxHealth());
                hero.addMaxHp(health.getMaxHealth());
            }
        }
        return true;
    }

    @Override
    public String toString() {
        return super.toString() + " for " +
                duration +
                " moves";
    }
}
