package org.example.model;

import lombok.Getter;
import lombok.Setter;
import org.example.model.dungeon.Dungeon;
import org.example.model.entity.Health;

import java.util.ArrayList;
import java.util.List;

@Getter
public class Screen {
    @Setter
    public char[][] field;
    @Setter
    public char[][] shadowField;
    @Getter
    private static final List<String> messages = new ArrayList<>();// Список для хранения сообщений
    private static final int MAX_MESSAGES = 6;
    public int dungeonLevel;
    public Health health;
    public int playerStr;
    public int gold;
    public String playerName;

    {
        playerName = "";
        field = new char[Dungeon.StaticDungeonGenerator.MAP_HEIGHT][Dungeon.StaticDungeonGenerator.MAP_WIDTH];
        shadowField = new char[Dungeon.StaticDungeonGenerator.MAP_HEIGHT][Dungeon.StaticDungeonGenerator.MAP_WIDTH];

    }

    public Screen() {
        for (int i = 0; i < Dungeon.StaticDungeonGenerator.MAP_HEIGHT; i++) {
            for (int j = 0; j < Dungeon.StaticDungeonGenerator.MAP_WIDTH; j++) {
                shadowField[i][j] = ' ';
                field[i][j] = ' ';
            }
        }
    }

    public static void addMessage(String message) {
        messages.add(message);
        if (messages.size() > MAX_MESSAGES) {
            messages.removeFirst();
        }
    }
    public static void clearMessages() {
        messages.clear();
    }
}
