package org.example.view;

import jcurses.system.CharColor;
import jcurses.system.Toolkit;
import org.example.model.Screen;
import org.example.model.entity.Item;
import org.example.model.entity.UserHero;

import java.util.ArrayList;

public class GameView {
    static CharColor color = new CharColor(CharColor.BLACK, CharColor.WHITE);
    static CharColor green = new CharColor(CharColor.BLACK, CharColor.GREEN);
    static CharColor red = new CharColor(CharColor.BLACK, CharColor.RED);
    static CharColor yellow = new CharColor(CharColor.BLACK, CharColor.YELLOW);

    public static  void printMap(Screen screen) {
        CharColor colors;
        for (int i = 0; i < screen.getField().length; i++) {
            for (int j = 0; j < screen.getField()[i].length; j++) {
                switch (screen.getField()[i][j]) {
                    case 'z' -> colors = green;
                    case 'v' -> colors = red;
                    case 'O' -> colors = yellow;
                    default -> colors = color;
                }
                Toolkit.printString(String.valueOf(screen.getField()[i][j]), j, i, colors);
            }
        }
        // Выводим строку состояния и сообщения игры
        printStatusBar(screen, UserHero.getHero());
        printMessages();
    }

    private static  void printStatusBar(Screen screen, UserHero hero) {
        if (hero == null) {
            System.out.println("Error: the hero is not initialized.");
            return;
        }
        String statusBar = String.format(
                "Level:   %d %s",
                screen.dungeonLevel, // Уровень подземелья
                hero // Показатели героя
        );
        clearString(37, 120);
        printString(statusBar, 37);
    }

    public static void printMessages() {
        var messages = Screen.getMessages();
        if (!messages.isEmpty()) {
            int cnt = 0;
            for (String message : messages) {
                clearString(30 + cnt, 120);
                printString(message, 30 + cnt);
                cnt++;
            }
        }
    }

    public static void printItems(ArrayList<Item> items) {
        Clear();
        int cnt = 1;
        for (var item : items) {
            String str = cnt + ") " + item.toString();
            printString(str, cnt - 1);
            cnt++;
        }
    }

    public static void noItems() {
        Clear();
        printString("No item", 1);
    }

    public static void printString(String string, int rowNum) {
        printString(string, rowNum, 0);
    }

    public static void printString(String string, int rowNum, int colNum) {
        for (int i = colNum; i < string.length(); i++) {
            Toolkit.printString(String.valueOf(string.charAt(i)), i, rowNum, color);
        }
    }

    public static void printString(String string, int rowNum, int colNum, CharColor color) {
        for (int i = 0; i < string.length(); i++) {
            Toolkit.printString(String.valueOf(string.charAt(i)), i + colNum, rowNum, color);
            if (String.valueOf(string.charAt(i)).equals(" ")) {
                CharColor col;
                if (color.getBackground() == CharColor.GREEN) {
                    col = new CharColor(CharColor.GREEN, CharColor.GREEN);
                } else {
                    col = new CharColor(CharColor.BLACK, CharColor.BLACK);
                }
                Toolkit.printString(".", i + colNum, rowNum, col);
            }
        }
    }

    public static void clearString(int rowNum, int colNum) {
        for (int i = 0; i < colNum; i++) {
            Toolkit.printString(".", i, rowNum, new CharColor(CharColor.BLACK, CharColor.BLACK));
        }
    }


    public static void Clear() {
        Toolkit.clearScreen(new CharColor(CharColor.BLACK, CharColor.BLACK));
    }
    public static void Clear(int col, int row, int size, CharColor color) {
        for(int i = col; i < col + size; i++ ) {
            Toolkit.printString(".", i, row, color);
        }
    }
}
