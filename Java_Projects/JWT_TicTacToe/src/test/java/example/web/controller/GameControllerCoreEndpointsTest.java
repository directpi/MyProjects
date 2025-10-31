package example.web.controller;

import example.domain.model.Game;
import example.domain.model.GameBoard;
import example.domain.model.GameState;
import example.domain.service.GameService;
import example.utils.AuthUtils;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.Mock;
import org.mockito.junit.jupiter.MockitoExtension;
import org.springframework.http.MediaType;
import org.springframework.test.web.servlet.MockMvc;
import org.springframework.test.web.servlet.setup.MockMvcBuilders;

import java.util.Arrays;
import java.util.Collections;
import java.util.UUID;

import static org.mockito.Mockito.*;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.*;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.*;

/**
 * Минимальные тесты для основных endpoint'ов GameController
 * Каждый тест независим и мокает только то, что нужно
 */
@ExtendWith(MockitoExtension.class)
class GameControllerCoreEndpointsTest {

        private MockMvc mockMvc;

        @Mock
        private GameService gameService;

        @Mock
        private AuthUtils authUtils;

        @BeforeEach
        void setUp() {
                // Standalone MockMvc без Spring Context
                GameController controller = new GameController(gameService, authUtils);
                mockMvc = MockMvcBuilders.standaloneSetup(controller).build();
        }

        // =============== CREATE GAME TESTS ===============

        @Test
        void createGame_WithVsComputerTrue_ShouldCreateGame() throws Exception {
                // Given
                UUID playerId = UUID.randomUUID();
                UUID gameId = UUID.randomUUID();

                GameBoard board = mock(GameBoard.class);
                when(board.getBoard()).thenReturn(new int[3][3]);

                Game game = mock(Game.class);
                when(game.getId()).thenReturn(gameId);
                when(game.getPlayer1Id()).thenReturn(playerId);
                when(game.getPlayer2Id()).thenReturn(null);
                when(game.getState()).thenReturn(GameState.WAITING_FOR_PLAYERS);
                when(game.isVsComputer()).thenReturn(true);
                when(game.getVersion()).thenReturn(1);
                when(game.getBoard()).thenReturn(board);

                when(authUtils.getCurrentPlayerId()).thenReturn(playerId);
                when(gameService.createGame(playerId, true)).thenReturn(game);

                // When & Then
                mockMvc.perform(post("/game/create")
                                .param("vsComputer", "true"))
                                .andExpect(status().isOk())
                                .andExpect(content().contentType(MediaType.APPLICATION_JSON))
                                .andExpect(jsonPath("$.id").value(gameId.toString()))
                                .andExpect(jsonPath("$.vsComputer").value(true));

                verify(authUtils).getCurrentPlayerId();
                verify(gameService).createGame(playerId, true);
        }

        @Test
        void createGame_WithVsComputerFalse_ShouldCreateMultiplayerGame() throws Exception {
                // Given
                UUID playerId = UUID.randomUUID();
                UUID gameId = UUID.randomUUID();

                GameBoard board = mock(GameBoard.class);
                when(board.getBoard()).thenReturn(new int[3][3]);

                Game game = mock(Game.class);
                when(game.getId()).thenReturn(gameId);
                when(game.getPlayer1Id()).thenReturn(playerId);
                when(game.getPlayer2Id()).thenReturn(null);
                when(game.getState()).thenReturn(GameState.WAITING_FOR_PLAYERS);
                when(game.isVsComputer()).thenReturn(false);
                when(game.getVersion()).thenReturn(1);
                when(game.getBoard()).thenReturn(board);

                when(authUtils.getCurrentPlayerId()).thenReturn(playerId);
                when(gameService.createGame(playerId, false)).thenReturn(game);

                // When & Then
                mockMvc.perform(post("/game/create")
                                .param("vsComputer", "false"))
                                .andExpect(status().isOk())
                                .andExpect(jsonPath("$.id").value(gameId.toString()))
                                .andExpect(jsonPath("$.vsComputer").value(false));

                verify(authUtils).getCurrentPlayerId();
                verify(gameService).createGame(playerId, false);
        }

        // =============== GET GAME TESTS ===============

        @Test
        void getGame_WithExistingGameId_ShouldReturnGame() throws Exception {
                // Given
                UUID gameId = UUID.randomUUID();

                GameBoard board = mock(GameBoard.class);
                when(board.getBoard()).thenReturn(new int[3][3]);

                Game game = mock(Game.class);
                when(game.getId()).thenReturn(gameId);
                when(game.getPlayer1Id()).thenReturn(UUID.randomUUID());
                when(game.getPlayer2Id()).thenReturn(null);
                when(game.getState()).thenReturn(GameState.WAITING_FOR_PLAYERS);
                when(game.isVsComputer()).thenReturn(false);
                when(game.getVersion()).thenReturn(1);
                when(game.getBoard()).thenReturn(board);

                when(gameService.getGameById(gameId)).thenReturn(game);

                // When & Then
                mockMvc.perform(get("/game/{gameId}", gameId))
                                .andExpect(status().isOk())
                                .andExpect(jsonPath("$.id").value(gameId.toString()));

                verify(gameService).getGameById(gameId);
        }

        // =============== JOIN GAME TESTS ===============

        @Test
        void joinGame_WithValidGameId_ShouldJoinGame() throws Exception {
                // Given
                UUID playerId = UUID.randomUUID();
                UUID gameId = UUID.randomUUID();

                GameBoard board = mock(GameBoard.class);
                when(board.getBoard()).thenReturn(new int[3][3]);

                Game joinedGame = mock(Game.class);
                when(joinedGame.getId()).thenReturn(gameId);
                when(joinedGame.getPlayer1Id()).thenReturn(UUID.randomUUID());
                when(joinedGame.getPlayer2Id()).thenReturn(playerId);
                when(joinedGame.getState()).thenReturn(GameState.PLAYER_1_TURN);
                when(joinedGame.isVsComputer()).thenReturn(false);
                when(joinedGame.getVersion()).thenReturn(1);
                when(joinedGame.getBoard()).thenReturn(board);

                when(authUtils.getCurrentPlayerId()).thenReturn(playerId);
                when(gameService.joinGame(gameId, playerId)).thenReturn(joinedGame);

                // When & Then
                mockMvc.perform(post("/game/join/{gameId}", gameId))
                                .andExpect(status().isOk())
                                .andExpect(jsonPath("$.id").value(gameId.toString()))
                                .andExpect(jsonPath("$.state").value("PLAYER_1_TURN"));

                verify(authUtils).getCurrentPlayerId();
                verify(gameService).joinGame(gameId, playerId);
        }

        // =============== GET AVAILABLE GAMES TESTS ===============

        @Test
        void getAvailableGames_WithAvailableGames_ShouldReturnList() throws Exception {
                // Given
                UUID gameId = UUID.randomUUID();

                GameBoard board = mock(GameBoard.class);
                when(board.getBoard()).thenReturn(new int[3][3]);

                Game game = mock(Game.class);
                when(game.getId()).thenReturn(gameId);
                when(game.getPlayer1Id()).thenReturn(UUID.randomUUID());
                when(game.getPlayer2Id()).thenReturn(null);
                when(game.getState()).thenReturn(GameState.WAITING_FOR_PLAYERS);
                when(game.isVsComputer()).thenReturn(false);
                when(game.getVersion()).thenReturn(1);
                when(game.getBoard()).thenReturn(board);

                when(gameService.getAvailableGames()).thenReturn(Arrays.asList(game));

                // When & Then
                mockMvc.perform(get("/game/available"))
                                .andExpect(status().isOk())
                                .andExpect(jsonPath("$[0].id").value(gameId.toString()));

                verify(gameService).getAvailableGames();
        }

        @Test
        void getAvailableGames_WithNoAvailableGames_ShouldReturnEmptyList() throws Exception {
                // Given
                when(gameService.getAvailableGames()).thenReturn(Collections.emptyList());

                // When & Then
                mockMvc.perform(get("/game/available"))
                                .andExpect(status().isOk())
                                .andExpect(jsonPath("$").isArray())
                                .andExpect(jsonPath("$").isEmpty());

                verify(gameService).getAvailableGames();
        }

        // =============== DELETE GAME TESTS ===============

        @Test
        void deleteGame_WithValidGameId_ShouldDeleteGame() throws Exception {
                // Given
                UUID playerId = UUID.randomUUID();
                UUID gameId = UUID.randomUUID();

                when(authUtils.getCurrentPlayerId()).thenReturn(playerId);
                doNothing().when(gameService).deleteGame(gameId, playerId);

                // When & Then
                mockMvc.perform(delete("/game/{gameId}", gameId))
                                .andExpect(status().isOk())
                                .andExpect(jsonPath("$.message").value("Игра успешно удалена"))
                                .andExpect(jsonPath("$.gameId").value(gameId.toString()));

                verify(authUtils).getCurrentPlayerId();
                verify(gameService).deleteGame(gameId, playerId);
        }
}