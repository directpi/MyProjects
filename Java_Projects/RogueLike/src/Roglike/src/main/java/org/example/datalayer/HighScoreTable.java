package org.example.datalayer;

import com.google.gson.JsonArray;
import com.google.gson.JsonObject;

import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;

public class HighScoreTable {
    private static HighScoreTable instance;

    private HighScoreTable() {
        load();
    }

    public static HighScoreTable getInstance() {
        if (instance == null) {
            instance = new HighScoreTable();
        }
        return instance;
    }

    private record HighScoreEntry(String playerName, int dungeonLevel, int treasure) {

        @Override
        public String toString() {
            return String.format("Player: %s, Level: %d, Gold: %d", playerName, dungeonLevel, treasure);
        }
    }

    private List<HighScoreEntry> entries = new ArrayList<>();

    public void addEntry(String playerName, int dungeonLevel, int treasure) {
        HighScoreEntry newEntry = new HighScoreEntry(playerName, dungeonLevel, treasure);
        if (!entries.contains(newEntry)) {
            entries.add(new HighScoreEntry(playerName, dungeonLevel, treasure));
            sortEntries();
        }
    }

    private void sortEntries() {
        entries.sort(Comparator.comparingInt(HighScoreEntry::treasure).reversed());
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder("Records table:\n");
        for (int i = 0; i < entries.size(); i++) {
            sb.append(i + 1).append(". ").append(entries.get(i)).append("\n");
        }
        return sb.toString();
    }


    public void save() { // Сохраняет таблицу рекордов в JSON
        JsonObject jsonOut = new JsonObject();

        JsonArray jsonEntries = new JsonArray();
        for (HighScoreEntry entry : entries) {
            JsonObject jsonEntry = new JsonObject();
            jsonEntry.addProperty("playerName", entry.playerName());
            jsonEntry.addProperty("dungeonLevel", entry.dungeonLevel());
            jsonEntry.addProperty("treasure", entry.treasure());
            jsonEntries.add(jsonEntry);
        }

        jsonOut.add("highScores", jsonEntries);
        File.saveJson(jsonOut, "high_scores");
    }

    public void load() {
        entries.clear();
        JsonObject jsonIn = File.readJson("high_scores.json");
        if (jsonIn != null && jsonIn.has("highScores")) {
            JsonArray jsonEntries = jsonIn.getAsJsonArray("highScores");

            for (var entry : jsonEntries) {
                JsonObject jsonEntry = entry.getAsJsonObject();
                String playerName = jsonEntry.get("playerName").getAsString();
                int dungeonLevel = jsonEntry.get("dungeonLevel").getAsInt();
                int treasure = jsonEntry.get("treasure").getAsInt();
                entries.add(new HighScoreEntry(playerName, dungeonLevel, treasure));
            }
            sortEntries();
        }
    }
}