package s21.example.domain.model;

import lombok.Getter;
import lombok.Setter;

import java.util.UUID;

@Setter
@Getter
public class Game {
    private UUID id;
    private GameBoard board;
    private int[][] previousBoard;

    public Game() {
        this.id = UUID.randomUUID();
        this.board = new GameBoard();
        this.previousBoard = new int[3][3];
    }

    public void setPreviousBoard(int[][] previousBoard) {
        if (previousBoard == null) {
            throw new IllegalArgumentException("previousBoard не может быть null");
        }
        this.previousBoard = previousBoard;
    }
}
