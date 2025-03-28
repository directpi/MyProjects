package org.example.controller;

import org.example.model.dungeon.LabyrinthPart;
import org.example.model.dungeon.Room;
import org.example.model.util.Direction;
import org.example.model.entity.UserHero;
import org.example.model.GameModel;
import org.example.view.GameView;
import org.example.view.Input;

public class Controller {
    enum PrintType {FIELD, BACKPACK_WEAPON, BACKPACK_HEALTH, BACKPACK_ELIXIR, BACKPACK_SCROLL}

    public enum GameState {PROCESSING, END_GAME, WIN, EXIT}

    private final GameModel model;
    private final ControllerHero heroController;
    private final ControllerEnemy enemyController;
    private PrintType printType;

    public Controller() {
        this.model = new GameModel();
        this.heroController = new ControllerHero(model);
        this.enemyController = new ControllerEnemy();
        printType = PrintType.FIELD;
    }
    public Controller(int startLvl){
        this.model = new GameModel(startLvl);
        this.heroController = new ControllerHero(model);
        this.enemyController = new ControllerEnemy();
        printType = PrintType.FIELD;
    }

    public char getInp() {
        return Input.getCh();
    }

    public GameState handleUserInput(char input) {
        ControllerHero.MoveResult result = null;
        if (printType == PrintType.FIELD) {
            switch (input) {
                case 'w' -> result = heroController.movePlayer(Direction.TOP);
                case 's' -> result = heroController.movePlayer(Direction.BOTTOM);
                case 'a' -> result = heroController.movePlayer(Direction.LEFT);
                case 'd' -> result = heroController.movePlayer(Direction.RIGHT);
                case 'q' -> {
                    return GameState.EXIT;
                }
                case 'h' -> printType = PrintType.BACKPACK_WEAPON;
                case 'j' -> printType = PrintType.BACKPACK_HEALTH;
                case 'k' -> printType = PrintType.BACKPACK_ELIXIR;
                case 'e' -> printType = PrintType.BACKPACK_SCROLL;
            }
            if (result == ControllerHero.MoveResult.NEXT_LEVEL) {
                if (model.nextLevel() == GameState.WIN) {
                    return GameState.WIN;
                } else {
                    GameView.Clear();
                }
            }
            // Обновляем всех персонажей после движения игрока
            LabyrinthPart location = UserHero.getHero().getLocation();
            if (printType == PrintType.FIELD && location instanceof Room currentRoom && !currentRoom.isStartRoom()) {
                enemyController.updateEnemies(currentRoom, UserHero.getHero());
            }

        } else {
            switch (input) {
                case 'h' ->
                        printType = (printType == PrintType.BACKPACK_WEAPON) ? PrintType.FIELD : PrintType.BACKPACK_WEAPON;
                case 'j' ->
                        printType = (printType == PrintType.BACKPACK_HEALTH) ? PrintType.FIELD : PrintType.BACKPACK_HEALTH;
                case 'k' ->
                        printType = (printType == PrintType.BACKPACK_ELIXIR) ? PrintType.FIELD : PrintType.BACKPACK_ELIXIR;
                case 'e' ->
                        printType = (printType == PrintType.BACKPACK_SCROLL) ? PrintType.FIELD : PrintType.BACKPACK_SCROLL;
                default -> {
                    if (input > '0' && input <= '9') {
                        var items = ControllerHero.getBackpack().getItems().get(getChooseBackpackItem());
                        if (input <= items.size() + '0') {
                            ControllerHero.useItem(items.get(input - '0' - 1));
                            printType = PrintType.FIELD;
                            GameView.Clear();
                        } else
                            System.out.println("There is no such item");
                    }
                }
            }
            if (printType == PrintType.FIELD) {
                GameView.Clear();
            }
        }
        return (heroController.isHeroDead()) ? GameState.END_GAME : GameState.PROCESSING;
    }

    private String getChooseBackpackItem() {
        String itemName = "Nan";
        switch (printType) {
            case BACKPACK_ELIXIR -> itemName = "Elixir";
            case BACKPACK_SCROLL -> itemName = "Scroll";
            case BACKPACK_HEALTH -> itemName = "Food";
            case BACKPACK_WEAPON -> itemName = "Weapon";
        }
        return itemName;
    }

    public GameModel getGameModel() {
        return model;
    }

    public void print() {
        if (printType == PrintType.FIELD) {
            GameView.printMap(model.getScreen());
        } else {
            String itemName = getChooseBackpackItem();
            if (ControllerHero.getBackpack().getItems().containsKey(itemName)) {
                var items = ControllerHero.getBackpack().getItems().get(itemName);
                GameView.printItems(items);
            } else {
                GameView.noItems();
            }
        }
    }

}
