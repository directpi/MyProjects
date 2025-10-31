package example.datasource.mapper;

import example.datasource.model.GameEntity;
import example.domain.model.Game;
import example.domain.model.GameBoard;
import example.domain.model.GameState;
import example.utils.Utils;

import java.util.UUID;

public class GameMapper {
    private static final Utils utils = new Utils();

    public static GameEntity toEntity(Game game) {
        if (game == null) {
            throw new IllegalArgumentException("Game не может быть null");
        }

        GameEntity entity = new GameEntity();
        entity.setVersion(game.getVersion() != null ? game.getVersion() : 1);
        entity.setId(game.getId() != null ? game.getId() : UUID.randomUUID());

        // Обязательные поля
        if (game.getPlayer1Id() == null) {
            throw new IllegalArgumentException("Player1Id cannot be null");
        }
        entity.setPlayer1Id(game.getPlayer1Id());

        // Опциональные поля
        entity.setPlayer2Id(game.getPlayer2Id());
        entity.setState(game.getState() != null ? game.getState() : GameState.WAITING_FOR_PLAYERS);
        entity.setVsComputer(game.isVsComputer());

        // Обработка доски
        int[][] board = (game.getBoard() != null && game.getBoard().getBoard() != null)
                ? game.getBoard().getBoard()
                : new int[3][3];
        entity.setBoard(board);

        // Обработка предыдущей доски
        int[][] previousBoard = (game.getPreviousBoard() != null)
                ? utils.copyBoard(game.getPreviousBoard())
                : new int[3][3];
        entity.setPreviousBoard(previousBoard);

        return entity;
    }

    public static Game toDomain(GameEntity entity) {
        if (entity == null || entity.getId() == null) {
            throw new IllegalArgumentException("GameEntity не может быть null или содержать null ID");
        }

        Game game = new Game();
        game.setId(entity.getId());
        game.setVersion(entity.getVersion() != null ? entity.getVersion() : 1);
        game.setPlayer1Id(entity.getPlayer1Id());
        game.setPlayer2Id(entity.getPlayer2Id());
        game.setVsComputer(entity.isVsComputer());
        game.setState(entity.getState() != null ? entity.getState() : GameState.WAITING_FOR_PLAYERS);

        // Инициализация доски
        GameBoard board = new GameBoard();
        board.setBoard(entity.getBoard() != null ? entity.getBoard() : new int[3][3]);
        game.setBoard(board);

        // Инициализация предыдущей доски
        game.setPreviousBoard(entity.getPreviousBoard() != null
                ? utils.copyBoard(entity.getPreviousBoard())
                : new int[3][3]);

        return game;
    }


}