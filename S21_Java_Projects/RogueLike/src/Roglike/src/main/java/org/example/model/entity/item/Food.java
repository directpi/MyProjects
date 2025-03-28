package org.example.model.entity.item;

import lombok.NoArgsConstructor;
import org.example.datalayer.GameStatistic;
import org.example.model.entity.Item;
import org.example.model.entity.UserHero;
import org.example.model.util.Coordinate;

@NoArgsConstructor
public class Food extends Item {
    public Food(Coordinate coordinate) {
        super(coordinate);
        health.setCurrentHealth(rand.nextInt(9) + 1);
    }

    @Override
    public boolean useItem() {
        boolean result = false;
        var userHealth = UserHero.getHero().getHealth();
        if (userHealth.getCurrentHealth() != userHealth.getMaxHealth()) {
            userHealth.addCurrentHealth(this.health.getCurrentHealth());

            GameStatistic.addEatingFood();

            result = true;
        }
        return result;
    }
}
