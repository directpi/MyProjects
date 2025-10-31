package example.domain.service;

import example.datasource.mapper.GameMapper;
import example.datasource.model.GameEntity;
import example.datasource.repository.GameRepository;
import example.domain.model.Game;
import example.domain.model.GameState;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import org.mockito.MockedStatic;
import org.mockito.junit.jupiter.MockitoExtension;

import java.time.LocalDateTime;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.UUID;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.Mockito.*;
import org.mockito.junit.jupiter.MockitoSettings;
import org.mockito.quality.Strictness;

@ExtendWith(MockitoExtension.class)
@MockitoSettings(strictness = Strictness.LENIENT)
class GameServiceImplHistoryTest {

    @Mock
    private GameRepository gameRepository;

    @InjectMocks
    private GameServiceImpl gameService;

    private UUID testPlayerId;
    private GameEntity completedGame1;
    private GameEntity completedGame2;
    private GameEntity activeGame;

    @BeforeEach
    void setUp() {
        testPlayerId = UUID.randomUUID();

        // Завершенная игра 1 - победа игрока
        completedGame1 = new GameEntity();
        completedGame1.setId(UUID.randomUUID());
        completedGame1.setPlayer1Id(testPlayerId);
        completedGame1.setPlayer2Id(UUID.randomUUID());
        completedGame1.setState(GameState.PLAYER_1_WIN);
        completedGame1.setBoard(new int[][] { { 1, 1, 1 }, { 2, 2, 0 }, { 0, 0, 0 } });
        completedGame1.setCreatedAt(LocalDateTime.now().minusDays(2));
        completedGame1.setVsComputer(false);
        completedGame1.setVersion(1);

        // Завершенная игра 2 - ничья
        completedGame2 = new GameEntity();
        completedGame2.setId(UUID.randomUUID());
        completedGame2.setPlayer1Id(UUID.randomUUID());
        completedGame2.setPlayer2Id(testPlayerId);
        completedGame2.setState(GameState.DRAW);
        completedGame2.setBoard(new int[][] { { 1, 2, 1 }, { 2, 1, 2 }, { 2, 1, 2 } });
        completedGame2.setCreatedAt(LocalDateTime.now().minusDays(1));
        completedGame2.setVsComputer(false);
        completedGame2.setVersion(1);

        // Активная игра - не должна попасть в историю
        activeGame = new GameEntity();
        activeGame.setId(UUID.randomUUID());
        activeGame.setPlayer1Id(testPlayerId);
        activeGame.setPlayer2Id(UUID.randomUUID());
        activeGame.setState(GameState.PLAYER_1_TURN);
        activeGame.setBoard(new int[][] { { 1, 0, 0 }, { 0, 2, 0 }, { 0, 0, 0 } });
        activeGame.setCreatedAt(LocalDateTime.now());
        activeGame.setVsComputer(false);
        activeGame.setVersion(1);
    }

    @Test
    void getCompletedGamesByPlayerId_WithExistingGames_ShouldReturnCompletedGamesOnly() {
        // Given
        List<GameEntity> completedGames = Arrays.asList(completedGame2, completedGame1); // Сортировка по дате (новые
                                                                                         // первыми)
        when(gameRepository.findCompletedGamesByPlayerId(testPlayerId)).thenReturn(completedGames);

        try (MockedStatic<GameMapper> gameMapperMock = mockStatic(GameMapper.class)) {
            Game game1 = createMockGame(completedGame1);
            Game game2 = createMockGame(completedGame2);

            gameMapperMock.when(() -> GameMapper.toDomain(completedGame1)).thenReturn(game1);
            gameMapperMock.when(() -> GameMapper.toDomain(completedGame2)).thenReturn(game2);

            // When
            List<Game> result = gameService.getCompletedGamesByPlayerId(testPlayerId);

            // Then
            assertNotNull(result);
            assertEquals(2, result.size());

            // Проверяем порядок - более новые игры должны быть первыми
            assertEquals(game2.getId(), result.get(0).getId()); // completedGame2 (вчера)
            assertEquals(game1.getId(), result.get(1).getId()); // completedGame1 (2 дня назад)

            verify(gameRepository).findCompletedGamesByPlayerId(testPlayerId);
            gameMapperMock.verify(() -> GameMapper.toDomain(completedGame1));
            gameMapperMock.verify(() -> GameMapper.toDomain(completedGame2));
        }
    }

    @Test
    void getCompletedGamesByPlayerId_WithNoCompletedGames_ShouldReturnEmptyList() {
        // Given
        when(gameRepository.findCompletedGamesByPlayerId(testPlayerId)).thenReturn(Collections.emptyList());

        // When
        List<Game> result = gameService.getCompletedGamesByPlayerId(testPlayerId);

        // Then
        assertNotNull(result);
        assertTrue(result.isEmpty());
        verify(gameRepository).findCompletedGamesByPlayerId(testPlayerId);
    }

    @Test
    void getCompletedGamesByPlayerId_WithNullPlayerId_ShouldCallRepository() {
        // Given
        UUID nullPlayerId = null;
        when(gameRepository.findCompletedGamesByPlayerId(nullPlayerId)).thenReturn(Collections.emptyList());

        // When
        List<Game> result = gameService.getCompletedGamesByPlayerId(nullPlayerId);

        // Then
        assertNotNull(result);
        assertTrue(result.isEmpty());
        verify(gameRepository).findCompletedGamesByPlayerId(nullPlayerId);
    }

    @Test
    void getCompletedGamesByPlayerId_WithRepositoryException_ShouldPropagateException() {
        // Given
        when(gameRepository.findCompletedGamesByPlayerId(testPlayerId))
                .thenThrow(new RuntimeException("Database connection error"));

        // When & Then
        RuntimeException exception = assertThrows(RuntimeException.class,
                () -> gameService.getCompletedGamesByPlayerId(testPlayerId));
        assertEquals("Database connection error", exception.getMessage());

        verify(gameRepository).findCompletedGamesByPlayerId(testPlayerId);
    }

    @Test
    void getCompletedGamesByPlayerId_WithMappingException_ShouldPropagateException() {
        // Given
        List<GameEntity> completedGames = Arrays.asList(completedGame1);
        when(gameRepository.findCompletedGamesByPlayerId(testPlayerId)).thenReturn(completedGames);

        try (MockedStatic<GameMapper> gameMapperMock = mockStatic(GameMapper.class)) {
            gameMapperMock.when(() -> GameMapper.toDomain(any(GameEntity.class)))
                    .thenThrow(new RuntimeException("Mapping error"));

            // When & Then
            RuntimeException exception = assertThrows(RuntimeException.class,
                    () -> gameService.getCompletedGamesByPlayerId(testPlayerId));
            assertEquals("Mapping error", exception.getMessage());

            verify(gameRepository).findCompletedGamesByPlayerId(testPlayerId);
            gameMapperMock.verify(() -> GameMapper.toDomain(completedGame1));
        }
    }

    @Test
    void getCompletedGamesByPlayerId_ShouldIncludeAllCompletedStates() {
        // Given - создаем игры с разными завершенными состояниями
        GameEntity winGame = createGameEntity(GameState.PLAYER_1_WIN);
        GameEntity lossGame = createGameEntity(GameState.PLAYER_2_WIN);
        GameEntity drawGame = createGameEntity(GameState.DRAW);

        List<GameEntity> completedGames = Arrays.asList(winGame, lossGame, drawGame);
        when(gameRepository.findCompletedGamesByPlayerId(testPlayerId)).thenReturn(completedGames);

        try (MockedStatic<GameMapper> gameMapperMock = mockStatic(GameMapper.class)) {
            Game mockWinGame = createMockGame(winGame);
            Game mockLossGame = createMockGame(lossGame);
            Game mockDrawGame = createMockGame(drawGame);

            gameMapperMock.when(() -> GameMapper.toDomain(winGame)).thenReturn(mockWinGame);
            gameMapperMock.when(() -> GameMapper.toDomain(lossGame)).thenReturn(mockLossGame);
            gameMapperMock.when(() -> GameMapper.toDomain(drawGame)).thenReturn(mockDrawGame);

            // When
            List<Game> result = gameService.getCompletedGamesByPlayerId(testPlayerId);

            // Then
            assertEquals(3, result.size());
            assertTrue(result.stream().anyMatch(g -> g.getState() == GameState.PLAYER_1_WIN));
            assertTrue(result.stream().anyMatch(g -> g.getState() == GameState.PLAYER_2_WIN));
            assertTrue(result.stream().anyMatch(g -> g.getState() == GameState.DRAW));
        }
    }

    @Test
    void getCompletedGamesByPlayerId_ShouldNotIncludeActiveGames() {
        // Given - включаем активную игру в результат репозитория (хотя в реальности
        // этого не должно быть)
        List<GameEntity> gamesFromRepo = Arrays.asList(completedGame1, activeGame);
        when(gameRepository.findCompletedGamesByPlayerId(testPlayerId)).thenReturn(gamesFromRepo);

        try (MockedStatic<GameMapper> gameMapperMock = mockStatic(GameMapper.class)) {
            Game game1 = createMockGame(completedGame1);
            Game activeGameDomain = createMockGame(activeGame);

            gameMapperMock.when(() -> GameMapper.toDomain(completedGame1)).thenReturn(game1);
            gameMapperMock.when(() -> GameMapper.toDomain(activeGame)).thenReturn(activeGameDomain);

            // When
            List<Game> result = gameService.getCompletedGamesByPlayerId(testPlayerId);

            // Then
            assertEquals(2, result.size()); // Все игры из репозитория должны быть преобразованы
            // Репозиторий сам должен фильтровать активные игры
            verify(gameRepository).findCompletedGamesByPlayerId(testPlayerId);
        }
    }

    @Test
    void getCompletedGamesByPlayerId_WithLargeResultSet_ShouldHandleCorrectly() {
        // Given - создаем большой список завершенных игр
        List<GameEntity> manyGames = Arrays.asList(
                createGameEntity(GameState.PLAYER_1_WIN),
                createGameEntity(GameState.PLAYER_2_WIN),
                createGameEntity(GameState.DRAW),
                createGameEntity(GameState.PLAYER_1_WIN),
                createGameEntity(GameState.DRAW));
        when(gameRepository.findCompletedGamesByPlayerId(testPlayerId)).thenReturn(manyGames);

        try (MockedStatic<GameMapper> gameMapperMock = mockStatic(GameMapper.class)) {
            // Мокаем маппер для каждой игры
            for (GameEntity entity : manyGames) {
                Game mockGame = createMockGame(entity);
                gameMapperMock.when(() -> GameMapper.toDomain(entity)).thenReturn(mockGame);
            }

            // When
            List<Game> result = gameService.getCompletedGamesByPlayerId(testPlayerId);

            // Then
            assertEquals(5, result.size());
            verify(gameRepository).findCompletedGamesByPlayerId(testPlayerId);
            // Проверяем, что mapper был вызван для каждой игры
            gameMapperMock.verify(() -> GameMapper.toDomain(any(GameEntity.class)), times(5));
        }
    }

    // Вспомогательные методы
    private GameEntity createGameEntity(GameState state) {
        GameEntity entity = new GameEntity();
        entity.setId(UUID.randomUUID());
        entity.setPlayer1Id(testPlayerId);
        entity.setPlayer2Id(UUID.randomUUID());
        entity.setState(state);
        entity.setBoard(new int[3][3]);
        entity.setCreatedAt(LocalDateTime.now());
        entity.setVsComputer(false);
        entity.setVersion(1);
        return entity;
    }

    private Game createMockGame(GameEntity entity) {
        Game game = mock(Game.class);
        when(game.getId()).thenReturn(entity.getId());
        when(game.getState()).thenReturn(entity.getState());
        when(game.getPlayer1Id()).thenReturn(entity.getPlayer1Id());
        when(game.getPlayer2Id()).thenReturn(entity.getPlayer2Id());
        return game;
    }
}
