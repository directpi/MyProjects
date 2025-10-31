package org.example.datalayer;

import com.fasterxml.jackson.annotation.JsonProperty;
import com.fasterxml.jackson.databind.ObjectMapper;
import lombok.SneakyThrows;
import org.example.model.entity.UserHero;

public class GameJson {

    private static class SaveInfo {
        @JsonProperty
        UserHero userHero = UserHero.getHero();
        @JsonProperty
        int dungenLvl = 2;

        public SaveInfo() {}
        public SaveInfo(int currentLevel){
            dungenLvl = currentLevel;
        }
    }

    /**
     * Сохраняет текущий статус героя в формате JSON, для последующей возможной загрузки.
     * Сохраняет все попытки всех уровней прохождения игры.
     * Сохраняет в файл с именем формата: <имягероя>.json
     * @param currentLevel текущий уровень игры
     */
    @SneakyThrows
    static public void save(int currentLevel) {
        ObjectMapper objectMapper = new ObjectMapper();
        java.io.File jsonFile = new java.io.File("save.json");
        objectMapper.writeValue(jsonFile, new SaveInfo(currentLevel));
    }

    /**
     * Загружает героя с заданным/введеным именем.
     * В случае отсутствия файла с именем nameHero.json создает героя
     * по умолчанию с указаным именем.
     * В случае ошибки формата или отсутствия файла возвращает начальный уровень 1.
     */
    @SneakyThrows
    static public int loadCreateHeroGetLevel()  {
        ObjectMapper objectMapper = new ObjectMapper();
        java.io.File jsonFile = new java.io.File("save.json");
        SaveInfo deserializedHero = objectMapper.readValue(jsonFile, SaveInfo.class);
        UserHero.getHero().setHero(deserializedHero.userHero);
        return deserializedHero.dungenLvl;
    }
}