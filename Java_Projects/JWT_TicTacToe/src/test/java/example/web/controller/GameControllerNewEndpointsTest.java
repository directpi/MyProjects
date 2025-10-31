package example.web.controller;

import example.domain.model.Game;
import example.domain.model.GameBoard;
import example.domain.model.GameState;
import example.domain.model.PlayerStats;
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
import java.util.List;
import java.util.UUID;

import static org.mockito.Mockito.*;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.get;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.*;

/**
 * Минимальные тесты для новых endpoint'ов GameController (history, leaderboard)
 * Каждый тест независим и мокает только то, что нужно
 */
@ExtendWith(MockitoExtension.class)
class GameControllerNewEndpointsTest {

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

        // =============== GAME HISTORY TESTS ===============

        @Test
        void getGameHistory_WithCompletedGames_ShouldReturnGameHistory() throws Exception {
                // Given
                UUID playerId = UUID.randomUUID();
                UUID game1Id = UUID.randomUUID();
                UUID game2Id = UUID.randomUUID();

                GameBoard board1 = mock(GameBoard.class);
                when(board1.getBoard()).thenReturn(new int[3][3]);

                GameBoard board2 = mock(GameBoard.class);
                when(board2.getBoard()).thenReturn(new int[3][3]);

                Game game1 = mock(Game.class);
                when(game1.getId()).thenReturn(game1Id);
                when(game1.getPlayer1Id()).thenReturn(playerId);
                when(game1.getPlayer2Id()).thenReturn(UUID.randomUUID());
                when(game1.getState()).thenReturn(GameState.PLAYER_1_WIN);
                when(game1.isVsComputer()).thenReturn(false);
                when(game1.getVersion()).thenReturn(1);
                when(game1.getBoard()).thenReturn(board1);

                Game game2 = mock(Game.class);
                when(game2.getId()).thenReturn(game2Id);
                when(game2.getPlayer1Id()).thenReturn(UUID.randomUUID());
                when(game2.getPlayer2Id()).thenReturn(playerId);
                when(game2.getState()).thenReturn(GameState.DRAW);
                when(game2.isVsComputer()).thenReturn(false);
                when(game2.getVersion()).thenReturn(1);
                when(game2.getBoard()).thenReturn(board2);

                List<Game> completedGames = Arrays.asList(game1, game2);

                when(authUtils.getCurrentPlayerId()).thenReturn(playerId);
                when(gameService.getCompletedGamesByPlayerId(playerId)).thenReturn(completedGames);

                // When & Then
                mockMvc.perform(get("/game/history"))
                                .andExpect(status().isOk())
                                .andExpect(content().contentType(MediaType.APPLICATION_JSON))
                                .andExpect(jsonPath("$").isArray())
                                .andExpect(jsonPath("$.length()").value(2));

                verify(authUtils).getCurrentPlayerId();
                verify(gameService).getCompletedGamesByPlayerId(playerId);
        }

        @Test
        void getGameHistory_WithNoCompletedGames_ShouldReturnEmptyArray() throws Exception {
                // Given
                UUID playerId = UUID.randomUUID();

                when(authUtils.getCurrentPlayerId()).thenReturn(playerId);
                when(gameService.getCompletedGamesByPlayerId(playerId)).thenReturn(Collections.emptyList());

                // When & Then
                mockMvc.perform(get("/game/history"))
                                .andExpect(status().isOk())
                                .andExpect(jsonPath("$").isArray())
                                .andExpect(jsonPath("$").isEmpty());

                verify(authUtils).getCurrentPlayerId();
                verify(gameService).getCompletedGamesByPlayerId(playerId);
        }

        // =============== LEADERBOARD TESTS ===============

        @Test
        void getLeaderboard_WithDefaultLimit_ShouldReturnTop10Players() throws Exception {
                // Given
                PlayerStats stats1 = new PlayerStats();
                stats1.setPlayerId(UUID.randomUUID());
                stats1.setLogin("topPlayer");
                stats1.setWins(15L);
                stats1.setLosses(3L);
                stats1.setDraws(2L);
                stats1.setWinRatio(0.750);

                PlayerStats stats2 = new PlayerStats();
                stats2.setPlayerId(UUID.randomUUID());
                stats2.setLogin("goodPlayer");
                stats2.setWins(8L);
                stats2.setLosses(5L);
                stats2.setDraws(1L);
                stats2.setWinRatio(0.571);

                List<PlayerStats> topPlayers = Arrays.asList(stats1, stats2);
                when(gameService.getTopPlayersByWinRatio(10)).thenReturn(topPlayers);

                // When & Then
                mockMvc.perform(get("/game/leaderboard"))
                                .andExpect(status().isOk())
                                .andExpect(content().contentType(MediaType.APPLICATION_JSON))
                                .andExpect(jsonPath("$").isArray())
                                .andExpect(jsonPath("$.length()").value(2))
                                .andExpect(jsonPath("$[0].login").value("topPlayer"))
                                .andExpect(jsonPath("$[0].wins").value(15))
                                .andExpect(jsonPath("$[0].winRatio").value(0.750));

                verify(gameService).getTopPlayersByWinRatio(10);
        }

        @Test
        void getLeaderboard_WithCustomLimit_ShouldReturnRequestedNumberOfPlayers() throws Exception {
                // Given
                PlayerStats stats1 = new PlayerStats();
                stats1.setPlayerId(UUID.randomUUID());
                stats1.setLogin("topPlayer");
                stats1.setWins(15L);
                stats1.setLosses(3L);
                stats1.setDraws(2L);
                stats1.setWinRatio(0.750);

                when(gameService.getTopPlayersByWinRatio(5)).thenReturn(Arrays.asList(stats1));

                // When & Then
                mockMvc.perform(get("/game/leaderboard")
                                .param("limit", "5"))
                                .andExpect(status().isOk())
                                .andExpect(jsonPath("$.length()").value(1));

                verify(gameService).getTopPlayersByWinRatio(5);
        }

        @Test
        void getLeaderboard_WithEmptyResult_ShouldReturnEmptyArray() throws Exception {
                // Given
                when(gameService.getTopPlayersByWinRatio(10)).thenReturn(Collections.emptyList());

                // When & Then
                mockMvc.perform(get("/game/leaderboard"))
                                .andExpect(status().isOk())
                                .andExpect(jsonPath("$").isEmpty());

                verify(gameService).getTopPlayersByWinRatio(10);
        }

        @Test
        void getLeaderboard_WithInvalidLimit_ShouldReturnBadRequest() throws Exception {
                // When & Then - отрицательный limit
                mockMvc.perform(get("/game/leaderboard")
                                .param("limit", "-1"))
                                .andExpect(status().isBadRequest());

                // When & Then - нулевой limit
                mockMvc.perform(get("/game/leaderboard")
                                .param("limit", "0"))
                                .andExpect(status().isBadRequest());

                // When & Then - слишком большой limit
                mockMvc.perform(get("/game/leaderboard")
                                .param("limit", "101"))
                                .andExpect(status().isBadRequest());

                // Сервис не должен вызываться
                verifyNoInteractions(gameService);
        }

        @Test
        void getLeaderboard_ShouldCalculateAdditionalFields() throws Exception {
                // Given
                PlayerStats stats1 = new PlayerStats();
                stats1.setPlayerId(UUID.randomUUID());
                stats1.setLogin("topPlayer");
                stats1.setWins(15L);
                stats1.setLosses(3L);
                stats1.setDraws(2L);
                stats1.setWinRatio(0.750);

                when(gameService.getTopPlayersByWinRatio(10)).thenReturn(Arrays.asList(stats1));

                // When & Then
                mockMvc.perform(get("/game/leaderboard"))
                                .andExpect(status().isOk())
                                .andExpect(jsonPath("$[0].totalGames").value(20)) // 15 + 3 + 2
                                .andExpect(jsonPath("$[0].winPercentage").value(75.0)); // 0.75 * 100

                verify(gameService).getTopPlayersByWinRatio(10);
        }
}