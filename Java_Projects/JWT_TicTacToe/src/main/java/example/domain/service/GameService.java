package example.domain.service;

import example.domain.model.Game;
import example.domain.model.PlayerStats;

import java.util.List;
import java.util.Optional;
import java.util.UUID;

public interface GameService {
    Game getNextMove(Game game);
    boolean validateBoardWithState(Game game, UUID playerId);
    boolean isGameOver(Game game);
    Game startNewGame(UUID playerId);
    Game makeMove(UUID gameId, int [][] board, Integer version, UUID playerId); // Сигнатура метода
    Game createGame(UUID playerId, boolean vsComputer);
    Game joinGame(UUID gameId, UUID playerId);
    Game getGameById(UUID gameId);
    List<Game> getAvailableGames();
    Optional<Game> getActiveGameByPlayer(UUID playerId);
    void deleteGame(UUID gameId, UUID playerId);
    void deleteGameAdmin(UUID gameId); // Для админского удаления
    List<Game> getCompletedGamesByPlayerId(UUID playerId);
    List<PlayerStats> getTopPlayersByWinRatio(int limit);
}
