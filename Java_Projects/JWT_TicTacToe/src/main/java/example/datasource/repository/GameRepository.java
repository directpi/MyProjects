package example.datasource.repository;

import example.datasource.model.GameEntity;
import example.domain.model.GameState;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.CrudRepository;
import org.springframework.data.repository.query.Param;
import org.springframework.stereotype.Repository;

import java.util.List;
import java.util.UUID;

@Repository
public interface GameRepository extends CrudRepository<GameEntity, UUID> {
        List<GameEntity> findByState(GameState state);

        boolean existsByPlayer1IdAndStateNotIn(UUID playerId, List<GameState> states);

        boolean existsByPlayer2IdAndStateNotIn(UUID playerId, List<GameState> states);

        default boolean existsActiveGameByPlayer(UUID playerId) {
                List<GameState> finishedStates = List.of(GameState.DRAW, GameState.PLAYER_1_WIN,
                                GameState.PLAYER_2_WIN);
                return existsByPlayer1IdAndStateNotIn(playerId, finishedStates)
                                || existsByPlayer2IdAndStateNotIn(playerId, finishedStates);
        }

        // Находит АКТИВНЫЕ игры, где пользователь является player1.
        // Исключает игры с завершенными состояниями (победа/ничья)
        @Query("SELECT g FROM GameEntity g WHERE g.player1Id = :playerId AND g.state NOT IN :states")
        List<GameEntity> findByPlayer1IdAndStateNotIn(@Param("playerId") UUID playerId,
                        @Param("states") List<GameState> states);

        // Находит АКТИВНЫЕ игры, где пользователь является player2.
        // Исключает игры с завершенными состояниями (победа/ничья)
        @Query("SELECT g FROM GameEntity g WHERE g.player2Id = :playerId AND g.state NOT IN :states")
        List<GameEntity> findByPlayer2IdAndStateNotIn(@Param("playerId") UUID playerId,
                        @Param("states") List<GameState> states);

        List<GameEntity> findByPlayer1Id(UUID playerId);

        List<GameEntity> findByPlayer2Id(UUID playerId);

        // Запрос для получения завершенных игр пользователя
        @Query("SELECT g FROM GameEntity g WHERE " +
                        "(g.player1Id = :playerId OR g.player2Id = :playerId) AND " +
                        "(g.state = example.domain.model.GameState.PLAYER_1_WIN OR " +
                        " g.state = example.domain.model.GameState.PLAYER_2_WIN OR " +
                        " g.state = example.domain.model.GameState.DRAW) " +
                        "ORDER BY g.createdAt DESC")
        List<GameEntity> findCompletedGamesByPlayerId(@Param("playerId") UUID playerId);

        // Запрос для получения статистики игроков (таблица лидеров)
        // Сортировка: 1) по win_ratio (DESC), 2) по количеству игр (DESC), 3) по wins
        // (DESC)
        // Это гарантирует, что игроки БЕЗ игр будут ниже игроков С играми при
        // одинаковом win_ratio
        @Query(value = """
                        SELECT
                            u.id,
                            u.login,
                            COUNT(CASE WHEN (g.player1_id = u.id AND g.state = 'PLAYER_1_WIN')
                                        OR (g.player2_id = u.id AND g.state = 'PLAYER_2_WIN')
                                   THEN 1 END) as wins,
                            COUNT(CASE WHEN (g.player1_id = u.id AND g.state = 'PLAYER_2_WIN')
                                        OR (g.player2_id = u.id AND g.state = 'PLAYER_1_WIN')
                                   THEN 1 END) as losses,
                            COUNT(CASE WHEN (g.player1_id = u.id OR g.player2_id = u.id)
                                        AND g.state = 'DRAW'
                                   THEN 1 END) as draws,
                            CASE
                                WHEN COUNT(g.id) = 0 THEN 0.0
                                ELSE CAST(COUNT(CASE WHEN (g.player1_id = u.id AND g.state = 'PLAYER_1_WIN')
                                                      OR (g.player2_id = u.id AND g.state = 'PLAYER_2_WIN')
                                                 THEN 1 END) AS DOUBLE PRECISION) / COUNT(g.id)
                            END as win_ratio
                        FROM users u
                        LEFT JOIN games g ON (g.player1_id = u.id OR g.player2_id = u.id)
                            AND g.state IN ('PLAYER_1_WIN', 'PLAYER_2_WIN', 'DRAW')
                        GROUP BY u.id, u.login
                        ORDER BY win_ratio DESC, COUNT(g.id) DESC, wins DESC
                        LIMIT :limit
                        """, nativeQuery = true)
        List<Object[]> findTopPlayersByWinRatio(@Param("limit") int limit);
}
