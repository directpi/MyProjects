package s21.example.domain.model;

import lombok.Getter;
import lombok.Setter;

@Getter
@Setter
public class GameBoard {
    private int[][] board;

    public GameBoard() {
        this.board = new int[3][3];
    }

}
