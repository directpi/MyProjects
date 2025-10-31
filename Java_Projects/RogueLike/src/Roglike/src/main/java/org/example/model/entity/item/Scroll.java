package org.example.model.entity.item;

import lombok.NoArgsConstructor;
import org.example.datalayer.GameStatistic;
import org.example.model.entity.Item;
import org.example.model.entity.UserHero;
import org.example.model.util.Coordinate;

@NoArgsConstructor
public class Scroll extends Item {
    public Scroll(Coordinate coordinate) {
        super(coordinate);
        int value = rand.nextInt(4) + 1;
        switch (rand.nextInt(3)) {
            case 0 -> dexterity = value;
            case 1 -> strength = value;
            case 2 -> health.setMaxHealth(value);
        }
    }

    @Override
    public boolean useItem() {
        var hero = UserHero.getHero();

        GameStatistic.addReadingScroll();

        if (this.strength > 0) {
            hero.addStrength(strength);
        } else if (this.dexterity > 0) {
            hero.addDexterity(dexterity);
        } else {
            hero.addMaxHp(health.getMaxHealth());
        }
        return true;
    }
}
