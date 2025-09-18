package s21.example.unit.service;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import org.mockito.junit.jupiter.MockitoExtension;
import s21.example.datasource.model.GameEntity;
import s21.example.datasource.repository.GameRepository;

import s21.example.datasource.repository.UserRepository;
import s21.example.domain.model.Game;
import s21.example.domain.model.GameState;
import s21.example.domain.service.GameServiceImpl;

import java.util.Optional;
import java.util.UUID;

import static org.mockito.ArgumentMatchers.any;
import static org.mockito.Mockito.when;
import static org.junit.jupiter.api.Assertions.*;

@ExtendWith(MockitoExtension.class)
class GameServiceTest {

    @Mock
    private GameRepository gameRepository;

    @Mock
    private UserRepository userRepository;



    @InjectMocks
    private GameServiceImpl gameService;

    @Test
    void makeMove_ValidMove_UpdatesGame() {
        UUID gameId = UUID.randomUUID();
        UUID player1 = UUID.randomUUID();

        // Состояние до хода: пустое поле, ход игрока 1
        int[][] board = new int[3][3];

        GameEntity entity = new GameEntity();
        entity.setId(gameId);
        entity.setPlayer1Id(player1);
        entity.setPlayer2Id(null);
        entity.setState(GameState.PLAYER_1_TURN);
        entity.setBoard(new int[3][3]);
        entity.setVsComputer(true);
        entity.setVersion(0);

        when(gameRepository.findById(any())).thenReturn(Optional.of(entity));
        when(gameRepository.save(any())).thenAnswer(invocation -> {
            GameEntity saved = invocation.getArgument(0);
            if (saved.getId() == null) saved.setId(gameId);
            if (saved.getBoard() == null) saved.setBoard(board);
            return saved;
        });

        // Делаем ход: ставим '1' в [0][0]
        int[][] requestedBoard = new int[3][3];
        requestedBoard[0][0] = 1;

        Game updatedGame = gameService.makeMove(gameId, requestedBoard, 0, player1);
        assertNotNull(updatedGame);
        assertEquals(1, updatedGame.getBoard().getBoard()[0][0]);
    }
}