package s21.example.datasource.repository;

import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.CrudRepository;
import org.springframework.data.repository.query.Param;
import s21.example.datasource.model.GameEntity;
import s21.example.domain.model.GameState;

import java.util.List;
import java.util.UUID;

public interface GameRepository extends CrudRepository<GameEntity, UUID> {
    List<GameEntity> findByState(GameState state);

    boolean existsByPlayer1IdAndStateNotIn(UUID playerId, List<GameState> states);
    boolean existsByPlayer2IdAndStateNotIn(UUID playerId, List<GameState> states);

    default boolean existsActiveGameByPlayer(UUID playerId) {
        List<GameState> finishedStates = List.of(GameState.DRAW, GameState.PLAYER_1_WIN, GameState.PLAYER_2_WIN);
        return existsByPlayer1IdAndStateNotIn(playerId, finishedStates)
                || existsByPlayer2IdAndStateNotIn(playerId, finishedStates);
    }

    @Query("SELECT g FROM GameEntity g WHERE g.player1Id = :playerId AND g.state NOT IN :states")
    List<GameEntity> findByPlayer1IdAndStateNotIn(@Param("playerId") UUID playerId, @Param("states") List<GameState> states);

    @Query("SELECT g FROM GameEntity g WHERE g.player2Id = :playerId AND g.state NOT IN :states")
    List<GameEntity> findByPlayer2IdAndStateNotIn(@Param("playerId") UUID playerId, @Param("states") List<GameState> states);

    List<GameEntity> findByPlayer1Id(UUID playerId);
    List<GameEntity> findByPlayer2Id(UUID playerId);


}
