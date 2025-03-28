package org.example;

import jcurses.system.Toolkit;
import org.example.controller.Controller;
import org.example.datalayer.HighScoreTable;
import org.example.model.Screen;
import org.example.view.Menu;
import sun.misc.Signal;

import org.example.datalayer.GameStatistic;
import org.example.model.entity.UserHero;

public class App {
    Menu menu = new Menu();
    HighScoreTable highScoreTable = HighScoreTable.getInstance();

    public void run() {
        Toolkit.init();
        Signal.handle(new Signal("INT"), signal -> {
            Toolkit.shutdown();
            System.out.println("Interrupted with ctrl+c");
            System.exit(2);
        });
        Menu.MenuChoose menuChoose;
        Controller controller = null;
        do {
            menuChoose = menu.runMenu();
            if (menuChoose == Menu.MenuChoose.NEW_GAME || menuChoose == Menu.MenuChoose.LOAD_GAME) {
                var state = Controller.GameState.PROCESSING;
                GameStatistic.resetStatistics();
                GameStatistic.read();
                controller = (menuChoose == Menu.MenuChoose.LOAD_GAME) ? new Controller(Menu.getLoadLvl()) : new Controller();
                do {
                    controller.print();
                    state = controller.handleUserInput(controller.getInp());
                } while (state == Controller.GameState.PROCESSING);
                if (Controller.GameState.END_GAME.equals(state)) {
                    new java.io.File("save.json").delete();
                }
                GameStatistic.save();
                highScoreTable.addEntry(
                        UserHero.getHero().getHeroName(),
                        controller.getGameModel().getDungeonLevel(),
                        UserHero.getHero().getTreasure()
                );
                highScoreTable.save();
                menu.endGame();
                UserHero.resetHero();
                Screen.clearMessages();
            }
        } while (menuChoose != Menu.MenuChoose.END_GAME);
        Toolkit.shutdown();
    }
}
