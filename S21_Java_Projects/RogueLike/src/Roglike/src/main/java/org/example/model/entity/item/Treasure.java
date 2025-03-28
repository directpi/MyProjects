package org.example.model.entity.item;

import org.example.datalayer.GameStatistic;
import org.example.model.util.Coordinate;
import org.example.model.entity.*;

public class Treasure extends Item {
    public Treasure(Coordinate coordinate) {
        super(coordinate);
        price = rand.nextInt(150) + 1;
    }

    @Override
    public boolean useItem() {
        UserHero.getHero().addTreasure(this.price);
        GameStatistic.addJewel(this.price);
        return true;
    }
}
