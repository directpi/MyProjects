package s21.example.web.mapper;

import s21.example.domain.model.Game;
import s21.example.web.model.GameDTO;

//import java.util.UUID;

public class GameMapper {
    public static GameDTO toDTO(Game game) {
        GameDTO dto = new GameDTO();
        dto.setId(game.getId());
        dto.setBoard(game.getBoard().getBoard());
        return dto;
    }

    public static Game toDomain(GameDTO dto) {
        if (dto == null || dto.getId() == null || dto.getBoard() == null) {
            throw new IllegalArgumentException("GameDTO не может быть null или содержать null поля");
        }
        Game game = new Game();
        game.setId(dto.getId());
        game.getBoard().setBoard(dto.getBoard());
        return game;
    }
}
