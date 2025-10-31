package org.example.model.entity.item;

import org.example.model.entity.Item;
import org.example.model.util.Coordinate;

import java.util.Random;

public class StaticItemFactory {
    public enum ItemType {
        Treasure, Food, Elixir, Scroll, Weapon;

        private static final Random random = new Random();

        public static StaticItemFactory.ItemType randomItem() {
            StaticItemFactory.ItemType[] types = values();
            return types[random.nextInt(types.length)];
        }
    }
//    public enum ItemState {He}
    public static Item createRandomItem(Coordinate coordinate) {
        return createItem(StaticItemFactory.ItemType.randomItem(), coordinate);
    }
    public static Item createItem(StaticItemFactory.ItemType type, Coordinate coordinate) {
        return switch (type){
            case Treasure -> new Treasure(coordinate);
            case Food -> new Food(coordinate);
            case Elixir -> new Elixir(coordinate);
            case Scroll -> new Scroll(coordinate);
            case Weapon -> new Weapon(coordinate);
        };
    }
}

