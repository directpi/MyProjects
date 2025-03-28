package org.example.model;

import com.google.gson.JsonObject;
import lombok.Getter;
import org.example.controller.Controller;
import org.example.datalayer.File;
import org.example.datalayer.GameJson;
import org.example.datalayer.GameStatistic;
import org.example.model.dungeon.*;
import org.example.model.entity.UserHero;

public class GameModel {
    @Getter
    private int dungeonLevel;
    private Screen screen;
    @Getter
    private Dungeon dungeon;
    Field field;

    {
        dungeonLevel = 1;
    }

    public GameModel() {
        dungeon = Dungeon.StaticDungeonGenerator.generatDungeon(dungeonLevel);
        firstUpdate();
        screen.playerName = UserHero.getHero().getHeroName();
    }

    public GameModel(int dungeonLevel) {
        this.dungeonLevel = dungeonLevel;
        dungeon = Dungeon.StaticDungeonGenerator.generatDungeon(dungeonLevel);
        firstUpdate();
        screen.playerName = UserHero.getHero().getHeroName();
    }

    public Screen getScreen() {
        screen.setField(field.getField());
        screen.setShadowField(field.getShadowField());
        return screen;
    }

    public Controller.GameState nextLevel() {
        GameStatistic.save();
        dungeonLevel++;
        if (dungeonLevel > 21)
            return Controller.GameState.WIN;
        GameJson.save(dungeonLevel);
        dungeon = Dungeon.StaticDungeonGenerator.generatDungeon(dungeonLevel);
        GameStatistic.resetStatistics();
        GameStatistic.addLevel(dungeonLevel - 1);
        firstUpdate();
        return Controller.GameState.PROCESSING;
    }


    private void firstUpdate() {
        screen = new Screen();
        screen.dungeonLevel = dungeonLevel;
        field = new Field(dungeon);
    }

}
