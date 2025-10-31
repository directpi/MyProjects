
package example.web.mapper;
import example.domain.model.Game;
import example.domain.model.GameState;
import example.web.model.GameDTO;

//import java.util.UUID;

public class GameMapper {
    public static GameDTO toDTO(Game game) {
        GameDTO dto = new GameDTO();
        dto.setId(game.getId());
        dto.setBoard(game.getBoard().getBoard());
        dto.setPlayer1Id(game.getPlayer1Id());
        dto.setPlayer2Id(game.getPlayer2Id());
        dto.setState(game.getState());
        dto.setVersion(game.getVersion());
        dto.setVsComputer(game.isVsComputer());
        dto.setPreviousBoard(game.getPreviousBoard());
        return dto;
    }

    public static Game toDomain(GameDTO dto) {
        if (dto == null || dto.getId() == null || dto.getBoard() == null) {
            throw new IllegalArgumentException("GameDTO не может быть null или содержать null поля");
        }
        Game game = new Game();
        game.setId(dto.getId());
        game.getBoard().setBoard(dto.getBoard());

        if (dto.getPlayer1Id() == null) {
            throw new IllegalArgumentException("Player1Id не может быть null");
        }
        game.setPlayer1Id(dto.getPlayer1Id());

        // Игрок 2 может быть null (если игра с компьютером или ожидает игрока)
        game.setPlayer2Id(dto.getPlayer2Id());

        // Состояние обязательно (по умолчанию WAITING_FOR_PLAYERS)
        game.setState(dto.getState() != null ? dto.getState() : GameState.WAITING_FOR_PLAYERS);
        game.setVersion(dto.getVersion() != null ? dto.getVersion() : 0);
        game.setVsComputer(dto.isVsComputer());
        game.setPreviousBoard(dto.getPreviousBoard());

        return game;
    }
}
