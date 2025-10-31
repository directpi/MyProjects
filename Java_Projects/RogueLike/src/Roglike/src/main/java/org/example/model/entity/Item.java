package org.example.model.entity;

import com.fasterxml.jackson.annotation.JsonSubTypes;
import com.fasterxml.jackson.annotation.JsonTypeInfo;
import lombok.Getter;
import org.example.model.entity.item.*;
import org.example.model.util.Coordinate;

import java.util.Random;

@JsonTypeInfo(use = JsonTypeInfo.Id.NAME)
@JsonSubTypes({
        @JsonSubTypes.Type(value = Elixir.class),
        @JsonSubTypes.Type(value = Food.class),
        @JsonSubTypes.Type(value = Scroll.class),
        @JsonSubTypes.Type(value = Weapon.class),
        @JsonSubTypes.Type(value = Treasure.class)
})

public abstract class Item extends Entity {
    protected Random rand;
    @Getter
    protected int price;

    {
       this.rand = new Random();
       this.skin = 'I';
    }

    public Item() {}

    public Item(Coordinate coordinate) {
        this.coordinate = coordinate;
    }

    public abstract boolean useItem();

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append(this.getClass().getSimpleName());
        sb.append(" ");
        if (strength > 0) {
            sb.append("strength: +");
            sb.append(strength);
            sb.append(" ");
        }
        if (dexterity > 0) {
            sb.append("dexterity: +");
            sb.append(dexterity);
            sb.append(" ");
        }
        if (health.getCurrentHealth() > 0) {
            sb.append("health: +");
            sb.append(health.getCurrentHealth());
            sb.append(" ");
        }
        if (health.getMaxHealth() > 0) {
            sb.append("max health: +");
            sb.append(health.getMaxHealth());
            sb.append(" ");
        }
        return sb.toString();
    }
}
