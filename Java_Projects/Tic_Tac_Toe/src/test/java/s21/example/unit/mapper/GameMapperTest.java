package s21.example.unit.mapper;

import org.junit.jupiter.api.Test;
import s21.example.datasource.model.GameEntity;
import s21.example.domain.model.Game;
import s21.example.domain.model.GameState;
import s21.example.datasource.mapper.GameMapper;

import java.util.UUID;

import static org.junit.jupiter.api.Assertions.*;

class GameMapperTest {

    @Test
    void toEntity_ConvertsGameCorrectly() {
        Game game = new Game();
        game.setPlayer1Id(UUID.randomUUID());
        game.setState(GameState.PLAYER_1_TURN);

        GameEntity entity = GameMapper.toEntity(game);
        assertEquals(game.getPlayer1Id(), entity.getPlayer1Id());
        assertEquals(GameState.PLAYER_1_TURN, entity.getState());
    }
}