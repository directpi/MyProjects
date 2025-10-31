package org.example.model.entity;

import lombok.Getter;
import org.example.controller.ConsoleMessageHandler;
import org.example.model.entity.item.Treasure;

import java.util.ArrayList;
import java.util.HashMap;

public class Backpack {
    ConsoleMessageHandler handler = new ConsoleMessageHandler();

    @Getter
    HashMap<String, ArrayList<Item>> items;
    int maxItems;

    {
        maxItems = 9;
        items = new HashMap<>();
    }

    public boolean addItem(Item item) {
        boolean result = false;
        if (item instanceof Treasure) {
            return item.useItem();
        }
        if (!items.containsKey(item.getClass().getSimpleName())) {
            items.put(item.getClass().getSimpleName(), new ArrayList<>());
        }
        if (items.get(item.getClass().getSimpleName()).size() <= maxItems) {
            items.get(item.getClass().getSimpleName()).add(item);
            result = true;
        } else {
            handler.sendMessage("The item does not fit in the backpack");
        }
        return result;
    }
}
