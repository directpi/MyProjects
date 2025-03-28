package org.example.view;

import jcurses.system.CharColor;
import lombok.Getter;
import org.example.datalayer.GameJson;
import org.example.datalayer.GameStatistic;
import org.example.datalayer.HighScoreTable;
import org.example.model.entity.UserHero;

import java.io.File;
import java.util.ArrayList;

import static jcurses.system.Toolkit.clearScreen;

public class Menu {

    public enum MenuChoose {NEW_GAME, END_GAME, LOAD_GAME}

    @Getter
    private static MenuChoose menuChoose = null;
    @Getter
    private static int loadLvl = 0;

    CharColor v1 = new CharColor(CharColor.BLACK, CharColor.WHITE);
    CharColor v2 = new CharColor(CharColor.GREEN, CharColor.BLACK);
    CharColor v3 = new CharColor(CharColor.BLACK, CharColor.BLACK);


    public MenuChoose runMenu() {
        menuChoose = null;
        startMenu();
        return menuChoose;
    }

    private void startMenu() {
        GameView.Clear();
        String head = "Welcome to Rouge game";
        String p1 = "Start game";
        String p2 = "Load game";
        String p3 = "Show statistic";
        String p4 = "Show record";
        String p5 = "Exit";
        final int MIN = 1;
        final int MAX = 5;
        int numMenu = 1;
        boolean flag = true;
        boolean fileExist = saveExist();
        do {
            clearScreen(v3);
            GameView.printString(head, 17, 64, v1);
            GameView.printString(p1, 18, 66, (numMenu == 1) ? v2 : v1);
            GameView.printString(p2, 19, 66, (numMenu == 2) ? v2 : v1);
            GameView.printString(p3, 20, 66, (numMenu == 3) ? v2 : v1);
            GameView.printString(p4, 21, 66, (numMenu == 4) ? v2 : v1);
            GameView.printString(p5, 22, 66, (numMenu == 5) ? v2 : v1);
            var input = Input.getCh2();

            switch (input.getCharacter()) {
                case 's' -> numMenu += (numMenu < MAX) ? (!fileExist && numMenu == 1) ? 2 : 1 : 0;
                case 'w' -> numMenu -= (numMenu > MIN) ? (!fileExist && numMenu == 3) ? 2 : 1 : 0;
                default -> {
                    if (input.getCode() == 10) {
                        switch (numMenu) {
                            case 1 -> {
                                menuChoose = MenuChoose.NEW_GAME;
                                createHero();
                                flag = false;
                            }
                            case 2 -> {
                                loadLvl = loadHeroProgress();
                                flag = false;
                            }
                            case 3 -> statisticMenu();
                            case 4 -> recordMenu();
                            case 5 -> {
                                menuChoose = MenuChoose.END_GAME;
                                flag = false;
                            }
                        }
                    }
                }
            }
        } while (flag);
        clearScreen(v3);
        GameView.Clear(66, 22, 10, new CharColor(CharColor.BLACK, CharColor.BLACK));
    }

    private int loadHeroProgress() {
        menuChoose = MenuChoose.LOAD_GAME;
        return GameJson.loadCreateHeroGetLevel();
    }

    private boolean saveExist() {
        java.io.File file = new File("save.json");
        return file.exists();
    }

    private void statisticMenu() {
        clearScreen(v3);
        GameView.Clear(66, 22, 10, new CharColor(CharColor.BLACK, CharColor.BLACK));
        GameStatistic.read();
        ArrayList<String> statistics = GameStatistic.getStatistics();
        int pageSize = 40;
        int currentPage = 0;
        int totalPages = (int) Math.ceil((double) statistics.size() / pageSize);
        while (true) {
            int start = currentPage * pageSize;
            int end = Math.min(start + pageSize, statistics.size());
            int y = 0;
            String navigationHint = "Page " + (currentPage + 1) + " of " + totalPages +
                    ". Press N (next), P (prev), Q (quit).";
            GameView.printString(navigationHint, 0, 50, v1);

            for (int i = start; i < end; i++) {
                GameView.printString(statistics.get(i), y + 1, 0, v1);
                y++;
            }
            char input = Input.getCh();
            if (input == 'n' || input == 'N') {
                if (currentPage < totalPages - 1) {
                    clearScreen(v3);
                    currentPage++;
                }
            } else if (input == 'p' || input == 'P') {
                if (currentPage > 0) {
                    clearScreen(v3);
                    currentPage--;
                }
            } else if (input == 'q' || input == 'Q') {
                break;
            }
        }
    }

    private void recordMenu() {
        clearScreen(v3);
        GameView.Clear(66, 22, 10, new CharColor(CharColor.BLACK, CharColor.BLACK));
        String head = "Press Q to quit";
        HighScoreTable table = HighScoreTable.getInstance();
        table.load();
        var str = table.toString().split("\n");
        do {
            int cnt = 1;
            GameView.printString(head, 0, 0, v1);
            for (var stat : str) {
                GameView.printString(stat, cnt, 0, v1);
                cnt++;
            }
        } while (Input.getCh() != 'q');
    }

    public void endGame() {
        clearScreen(v3);
        GameStatistic.read();
        statisticMenu();
    }


    public void createHero() {
        clearScreen(v3);
        GameView.Clear(66, 22, 10, new CharColor(CharColor.BLACK, CharColor.BLACK));
        String head = "Enter your name:";
        GameView.printString(head, 0, 0, v1);
        StringBuilder name = new StringBuilder();
        int maxLength = 20;

        while (true) {
            char ch = Input.getCh();  // Читаем один символ

            if (ch == '\n' || ch == '\r') { // Enter завершает ввод
                break;
            } else if ((ch == '\b' || ch == 127) && !name.isEmpty()) { // Удаляем последний символ
                name.deleteCharAt(name.length() - 1);
            } else if (Character.isLetterOrDigit(ch) || ch == '_') { // Разрешенные символы
                if (name.length() < maxLength) { // Ограничение длины
                    name.append(ch);
                }
            }

            GameView.printString(" ".repeat(20), 1, 0, v1); // Очистка строки ввода
            GameView.printString(name.toString(), 1, 0, v1);
        }
        UserHero.getHero().setHeroName(name.toString());
    }
}
