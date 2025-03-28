package s21.example.domain.service;

import s21.example.domain.model.Game;

public interface GameService {
    Game getNextMove(Game game);
    boolean validateBoard(Game game);
    boolean isGameOver(Game game);
}
