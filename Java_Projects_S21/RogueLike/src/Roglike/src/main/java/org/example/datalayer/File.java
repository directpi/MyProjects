package org.example.datalayer;

import com.google.gson.Gson;
import com.google.gson.JsonArray;
import com.google.gson.JsonObject;
import org.example.model.ActiveElixir;
import org.example.model.entity.Item;
import org.example.model.entity.UserHero;
import org.example.model.entity.item.Elixir;
import org.example.model.entity.item.Weapon;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;

public class File {

    static public void saveHero(JsonObject jsonOut) {
        saveJson(jsonOut, UserHero.getHero().getHeroName());
    }

    static public void saveStatistic(JsonObject jsonOut) {
        saveJson(jsonOut, "rogue_stat");
    }

    static void saveJson(JsonObject jsonOut, String jsonName) {
        try {
            FileWriter outFile = new FileWriter(jsonName + ".json");
            Gson json = new Gson();
            json.toJson(jsonOut, outFile);
            outFile.close();
        } catch (IOException e) {
            System.out.println("Ошибка при открытии файла: " + e.toString());
        }
    }

    static public JsonObject readHero(String heroName) {
        return readJson(heroName + ".json");
    }

    static public JsonObject readStatistic() {
        return readJson("rogue_stat.json");
    }

    static JsonObject readJson(String fileName) {
        Gson json = new Gson();
        JsonObject jsonReadAll = null;
        try {
            BufferedReader br = new BufferedReader(new FileReader(fileName));
            jsonReadAll = json.fromJson(br, JsonObject.class);
        } catch (Exception e) {
            //System.out.println("Ошибка чтения файла или неверный JSON-формат " + e.toString());
        }
        return jsonReadAll;
    }
}

