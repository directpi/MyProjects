package example.domain.service;

import example.datasource.repository.GameRepository;
import example.domain.model.PlayerStats;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import org.mockito.junit.jupiter.MockitoExtension;

import java.math.BigInteger;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.UUID;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.ArgumentMatchers.anyInt;
// import static org.mockito.ArgumentMatchers.eq;
import static org.mockito.Mockito.*;

@ExtendWith(MockitoExtension.class)
class GameServiceImplLeaderboardTest {

    @Mock
    private GameRepository gameRepository;

    @InjectMocks
    private GameServiceImpl gameService;

    private UUID player1Id;
    private UUID player2Id;
    private UUID player3Id;

    @BeforeEach
    void setUp() {
        player1Id = UUID.randomUUID();
        player2Id = UUID.randomUUID();
        player3Id = UUID.randomUUID();
    }

    @Test
    void getTopPlayersByWinRatio_WithValidLimit_ShouldReturnPlayerStats() {
        // Given
        int limit = 5;
        List<Object[]> rawResults = Arrays.asList(
                new Object[] { player1Id, "player1", 10L, 2L, 1L, 0.769 }, // 10 побед из 13 игр
                new Object[] { player2Id, "player2", 5L, 5L, 0L, 0.500 }, // 5 побед из 10 игр
                new Object[] { player3Id, "player3", 1L, 3L, 1L, 0.200 } // 1 победа из 5 игр
        );

        when(gameRepository.findTopPlayersByWinRatio(limit)).thenReturn(rawResults);

        // When
        List<PlayerStats> result = gameService.getTopPlayersByWinRatio(limit);

        // Then
        assertNotNull(result);
        assertEquals(3, result.size());

        // Проверяем первого игрока (лучший)
        PlayerStats topPlayer = result.get(0);
        assertEquals(player1Id, topPlayer.getPlayerId());
        assertEquals("player1", topPlayer.getLogin());
        assertEquals(10L, topPlayer.getWins());
        assertEquals(2L, topPlayer.getLosses());
        assertEquals(1L, topPlayer.getDraws());
        assertEquals(0.769, topPlayer.getWinRatio(), 0.001);

        // Проверяем второго игрока
        PlayerStats secondPlayer = result.get(1);
        assertEquals(player2Id, secondPlayer.getPlayerId());
        assertEquals("player2", secondPlayer.getLogin());
        assertEquals(0.500, secondPlayer.getWinRatio(), 0.001);

        verify(gameRepository).findTopPlayersByWinRatio(limit);
    }

    @Test
    void getTopPlayersByWinRatio_WithZeroLimit_ShouldThrowException() {
        // When & Then
        IllegalArgumentException exception = assertThrows(IllegalArgumentException.class,
                () -> gameService.getTopPlayersByWinRatio(0));
        assertEquals("Limit должен быть больше 0", exception.getMessage());

        verify(gameRepository, never()).findTopPlayersByWinRatio(anyInt());
    }

    @Test
    void getTopPlayersByWinRatio_WithNegativeLimit_ShouldThrowException() {
        // When & Then
        IllegalArgumentException exception = assertThrows(IllegalArgumentException.class,
                () -> gameService.getTopPlayersByWinRatio(-5));
        assertEquals("Limit должен быть больше 0", exception.getMessage());

        verify(gameRepository, never()).findTopPlayersByWinRatio(anyInt());
    }

    @Test
    void getTopPlayersByWinRatio_WithEmptyResult_ShouldReturnEmptyList() {
        // Given
        int limit = 10;
        when(gameRepository.findTopPlayersByWinRatio(limit)).thenReturn(Collections.emptyList());

        // When
        List<PlayerStats> result = gameService.getTopPlayersByWinRatio(limit);

        // Then
        assertNotNull(result);
        assertTrue(result.isEmpty());
        verify(gameRepository).findTopPlayersByWinRatio(limit);
    }

    @Test
    void getTopPlayersByWinRatio_WithBigIntegerValues_ShouldConvertCorrectly() {
        // Given - PostgreSQL может возвращать BigInteger вместо Long
        int limit = 3;
        List<Object[]> rawResults = Arrays.<Object[]>asList(
                new Object[] { player1Id, "player1", BigInteger.valueOf(15), BigInteger.valueOf(3),
                        BigInteger.valueOf(2), 0.750 });

        when(gameRepository.findTopPlayersByWinRatio(limit)).thenReturn(rawResults);

        // When
        List<PlayerStats> result = gameService.getTopPlayersByWinRatio(limit);

        // Then
        assertEquals(1, result.size());
        PlayerStats player = result.get(0);
        assertEquals(15L, player.getWins());
        assertEquals(3L, player.getLosses());
        assertEquals(2L, player.getDraws());
        verify(gameRepository).findTopPlayersByWinRatio(limit);
    }

    @Test
    void getTopPlayersByWinRatio_WithIntegerValues_ShouldConvertCorrectly() {
        // Given - могут прийти Integer значения
        int limit = 3;
        List<Object[]> rawResults = Arrays.<Object[]>asList(
                new Object[] { player1Id, "player1", Integer.valueOf(8), Integer.valueOf(1), Integer.valueOf(1),
                        0.800 });

        when(gameRepository.findTopPlayersByWinRatio(limit)).thenReturn(rawResults);

        // When
        List<PlayerStats> result = gameService.getTopPlayersByWinRatio(limit);

        // Then
        assertEquals(1, result.size());
        PlayerStats player = result.get(0);
        assertEquals(8L, player.getWins());
        assertEquals(1L, player.getLosses());
        assertEquals(1L, player.getDraws());
        verify(gameRepository).findTopPlayersByWinRatio(limit);
    }

    @Test
    void getTopPlayersByWinRatio_WithNullValues_ShouldHandleGracefully() {
        // Given
        int limit = 3;
        List<Object[]> rawResults = Arrays.<Object[]>asList(
                new Object[] { player1Id, "player1", null, null, null, 0.0 });

        when(gameRepository.findTopPlayersByWinRatio(limit)).thenReturn(rawResults);

        // When
        List<PlayerStats> result = gameService.getTopPlayersByWinRatio(limit);

        // Then
        assertEquals(1, result.size());
        PlayerStats player = result.get(0);
        assertEquals(0L, player.getWins());
        assertEquals(0L, player.getLosses());
        assertEquals(0L, player.getDraws());
        verify(gameRepository).findTopPlayersByWinRatio(limit);
    }

    @Test
    void getTopPlayersByWinRatio_WithRepositoryException_ShouldPropagateException() {
        // Given
        int limit = 5;
        when(gameRepository.findTopPlayersByWinRatio(limit))
                .thenThrow(new RuntimeException("Database connection error"));

        // When & Then
        RuntimeException exception = assertThrows(RuntimeException.class,
                () -> gameService.getTopPlayersByWinRatio(limit));
        assertEquals("Database connection error", exception.getMessage());

        verify(gameRepository).findTopPlayersByWinRatio(limit);
    }

    @Test
    void getTopPlayersByWinRatio_WithMappingException_ShouldThrowRuntimeException() {
        // Given - создаем некорректные данные, которые вызовут ошибку при
        // преобразовании
        int limit = 3;
        List<Object[]> rawResults = Arrays.<Object[]>asList(
                new Object[] { "invalid-uuid", "player1", 10L, 2L, 1L, 0.769 } // Некорректный UUID
        );

        when(gameRepository.findTopPlayersByWinRatio(limit)).thenReturn(rawResults);

        // When & Then
        RuntimeException exception = assertThrows(RuntimeException.class,
                () -> gameService.getTopPlayersByWinRatio(limit));
        assertEquals("Ошибка при обработке статистики игрока", exception.getMessage());

        verify(gameRepository).findTopPlayersByWinRatio(limit);
    }

    @Test
    void getTopPlayersByWinRatio_WithIncompleteData_ShouldThrowException() {
        // Given - недостаточно элементов в массиве
        int limit = 3;
        List<Object[]> rawResults = Arrays.<Object[]>asList(
                new Object[] { player1Id, "player1" } // Отсутствуют остальные поля
        );

        when(gameRepository.findTopPlayersByWinRatio(limit)).thenReturn(rawResults);

        // When & Then
        RuntimeException exception = assertThrows(RuntimeException.class,
                () -> gameService.getTopPlayersByWinRatio(limit));
        assertEquals("Ошибка при обработке статистики игрока", exception.getMessage());

        verify(gameRepository).findTopPlayersByWinRatio(limit);
    }

    @Test
    void getTopPlayersByWinRatio_ShouldPreserveOrderFromRepository() {
        // Given - репозиторий должен возвращать данные уже отсортированными
        int limit = 3;
        List<Object[]> rawResults = Arrays.asList(
                new Object[] { player1Id, "bestPlayer", 20L, 1L, 0L, 0.952 }, // Лучший игрок
                new Object[] { player2Id, "goodPlayer", 10L, 5L, 2L, 0.588 }, // Средний игрок
                new Object[] { player3Id, "newPlayer", 1L, 4L, 1L, 0.167 } // Начинающий игрок
        );

        when(gameRepository.findTopPlayersByWinRatio(limit)).thenReturn(rawResults);

        // When
        List<PlayerStats> result = gameService.getTopPlayersByWinRatio(limit);

        // Then
        assertEquals(3, result.size());

        // Проверяем, что порядок сохранен
        assertEquals("bestPlayer", result.get(0).getLogin());
        assertEquals("goodPlayer", result.get(1).getLogin());
        assertEquals("newPlayer", result.get(2).getLogin());

        // Проверяем, что соотношения побед убывают
        assertTrue(result.get(0).getWinRatio() >= result.get(1).getWinRatio());
        assertTrue(result.get(1).getWinRatio() >= result.get(2).getWinRatio());

        verify(gameRepository).findTopPlayersByWinRatio(limit);
    }

    @Test
    void getTopPlayersByWinRatio_WithLargeLimit_ShouldWork() {
        // Given
        int largeLimit = 100;
        List<Object[]> rawResults = Arrays.<Object[]>asList(
                new Object[] { player1Id, "player1", 5L, 1L, 0L, 0.833 });

        when(gameRepository.findTopPlayersByWinRatio(largeLimit)).thenReturn(rawResults);

        // When
        List<PlayerStats> result = gameService.getTopPlayersByWinRatio(largeLimit);

        // Then
        assertEquals(1, result.size());
        verify(gameRepository).findTopPlayersByWinRatio(largeLimit);
    }

    @Test
    void getTopPlayersByWinRatio_WithStringNumbers_ShouldConvertCorrectly() {
        // Given - некоторые БД могут возвращать числа как строки
        int limit = 1;
        List<Object[]> rawResults = Arrays.<Object[]>asList(
                new Object[] { player1Id, "player1", "7", "2", "1", 0.700 });

        when(gameRepository.findTopPlayersByWinRatio(limit)).thenReturn(rawResults);

        // When
        List<PlayerStats> result = gameService.getTopPlayersByWinRatio(limit);

        // Then
        assertEquals(1, result.size());
        PlayerStats player = result.get(0);
        assertEquals(7L, player.getWins());
        assertEquals(2L, player.getLosses());
        assertEquals(1L, player.getDraws());
        verify(gameRepository).findTopPlayersByWinRatio(limit);
    }

    @Test
    void getTopPlayersByWinRatio_ShouldSetWinRatioFromDatabase() {
        // Given
        int limit = 2;
        List<Object[]> rawResults = Arrays.<Object[]>asList(
                new Object[] { player1Id, "player1", 6L, 2L, 2L, 0.600 }, // 6 побед из 10 игр
                new Object[] { player2Id, "player2", 3L, 3L, 4L, 0.300 } // 3 победы из 10 игр
        );

        when(gameRepository.findTopPlayersByWinRatio(limit)).thenReturn(rawResults);

        // When
        List<PlayerStats> result = gameService.getTopPlayersByWinRatio(limit);

        // Then
        assertEquals(2, result.size());
        assertEquals(0.600, result.get(0).getWinRatio(), 0.001);
        assertEquals(0.300, result.get(1).getWinRatio(), 0.001);
        verify(gameRepository).findTopPlayersByWinRatio(limit);
    }
}
