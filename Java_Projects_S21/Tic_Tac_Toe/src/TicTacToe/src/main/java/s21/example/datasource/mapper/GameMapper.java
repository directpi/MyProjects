package s21.example.datasource.mapper;

import s21.example.datasource.model.GameEntity;
import s21.example.domain.model.Game;

public class GameMapper {
    public static GameEntity toEntity(Game game) {
        GameEntity entity = new GameEntity();
        entity.setId(game.getId());
        entity.setBoard(game.getBoard().getBoard());
        return entity;
    }

    public static Game toDomain(GameEntity dto) {
        if (dto == null || dto.getId() == null || dto.getBoard() == null) {
            throw new IllegalArgumentException("GameDTO не может быть null или содержать null поля");
        }
        Game game = new Game();
        game.setId(dto.getId());
        game.getBoard().setBoard(dto.getBoard());
        game.setPreviousBoard(new int[3][3]);
        return game;
    }
}
