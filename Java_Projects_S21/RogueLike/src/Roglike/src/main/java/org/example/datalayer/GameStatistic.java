package org.example.datalayer;

import com.google.gson.Gson;
import com.google.gson.JsonArray;
import com.google.gson.JsonObject;
import org.example.model.entity.UserHero;

import java.util.*;

/*
 В игре собирается и отображается в отдельном представлении статистика всех прохождений, отсортированная по
количеству набранных сокровищ: количество сокровищ, достигнутый уровень, количество побежденных противников,
количество съеденной еды, количество выпитых эликсиров, количество прочитанных свитков, количество нанесенных
и пропущенных ударов, количество пройденных клеток.
*/
public class GameStatistic {

    static private Integer[] statistic;
    static private Map<Integer, List<Integer[]>> statusStats;

    static {
        statistic = new Integer[9];
        for (int i = 0; i < 9; ++i) statistic[i] = 0;
        statusStats = new HashMap<>();
    }

    static public void addJewel(int quantity) {
        statistic[0] += quantity;
    }

    static public void addLevel(int value) {
        statistic[1] += value;
    }

    static public void addKillingEnemy() {
        statistic[2]++;
    }

    static public void addEatingFood() {
        statistic[3]++;
    }

    static public void addDrinkingElixir() {
        statistic[4]++;
    }

    static public void addReadingScroll() {
        statistic[5]++;
    }

    static public void addMissingStrike() {
        statistic[6]++;
    }

    static public void addMyHit() {
        statistic[7]++;
    }

    static public void addCellRoute() {
        statistic[8]++;
    }

    /**
     * Сохраняет статистику в json-формате, путем добавления к существующим записям
     * ключ... уровень игры, т.е. для каждого уровня добавляется текущая статистика
     */
    static public void save() {
        String keyLevel = statistic[1].toString();

        JsonObject existStatistic = File.readStatistic();
        if (existStatistic == null) {
            existStatistic = new JsonObject();
        }
        JsonArray listCurrentLevelStat = new JsonArray();
        if (existStatistic.has(keyLevel))
            listCurrentLevelStat = existStatistic.get(keyLevel).getAsJsonArray();

        Gson json = new Gson();
        listCurrentLevelStat.add(json.toJson(statistic));

        existStatistic.add(keyLevel, listCurrentLevelStat);

        File.saveStatistic(existStatistic);
    }

    static public void read() {
        JsonObject jsonReadAll = File.readStatistic();
        if (jsonReadAll != null) {
            fillExistingStatistics(jsonReadAll);
        }
//        else
//            System.out.println("empty JsonObject Statistic");

    }

    private static void fillExistingStatistics(JsonObject jsonReadAll) {
        Gson gson = new Gson();
        for (String key : jsonReadAll.keySet()) {
            int level = Integer.parseInt(key);
            JsonArray levelStats = jsonReadAll.get(key).getAsJsonArray();
            List<Integer[]> levelData = new ArrayList<>(); // Список для хранения всех записей уровня
            for (var stat : levelStats) {
                String statString = stat.getAsString();
                Integer[] stats = gson.fromJson(statString, Integer[].class);
                levelData.add(stats); // Добавляем запись в список
            }
            statusStats.put(level, levelData); // Сохраняем список для уровня
        }
    }

    public static ArrayList<String> getStatistics() {
        List<Map.Entry<Integer, List<Integer[]>>> sortedStats = new ArrayList<>(statusStats.entrySet());

        // Сортируем уровни по максимальному количеству сокровищ среди всех записей для уровня по убыванию
        sortedStats.sort((entry1, entry2) -> {
            int maxTreasures1 = entry1.getValue().stream().mapToInt(stats -> stats[0]).max().orElse(0);
            int maxTreasures2 = entry2.getValue().stream().mapToInt(stats -> stats[0]).max().orElse(0);
            return Integer.compare(maxTreasures2, maxTreasures1);
        });

        ArrayList<String> statistic = new ArrayList<>();

        for (Map.Entry<Integer, List<Integer[]>> entry : sortedStats) {
            int level = entry.getKey();
            List<Integer[]> levelStats = entry.getValue();

            // Сортируем записи внутри уровня по количеству сокровищ (по убыванию)
            levelStats.sort((stats1, stats2) -> Integer.compare(stats2[0], stats1[0]));

            int recordNumber = 1;
            for (Integer[] stats : levelStats) {
                statistic.add("  Level: " + level + ", Record #" + recordNumber);
                statistic.add("    Treasures: " + stats[0]);
                statistic.add("    Defeated opponents: " + stats[2]);
                statistic.add("    Eaten food: " + stats[3]);
                statistic.add("    Drunk elixirs: " + stats[4]);
                statistic.add("    Scrolls read: " + stats[5]);
                statistic.add("    Missed shots: " + stats[6]);
                statistic.add("    Strikes inflicted: " + stats[7]);
                statistic.add("    Passed cells: " + stats[8]);
                statistic.add("    --------------------");
                recordNumber++;
            }
        }
        return statistic;
    }

    static public void resetStatistics() {
        Arrays.fill(statistic, 0);
        statistic[1] = 1;
        statusStats.clear();
    }
}


