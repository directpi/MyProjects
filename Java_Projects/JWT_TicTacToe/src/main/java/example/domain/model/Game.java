package example.domain.model;

import lombok.Getter;
import lombok.Setter;
import org.springframework.data.annotation.Transient;

import java.util.UUID;

@Setter
@Getter
public class Game {
    private UUID id;
    private GameBoard board;
    private int[][] previousBoard;
    private GameState state;
    private String player1Username;
    private UUID player1Id;
    private String player2Username;
    private UUID player2Id;
    private boolean vsComputer;
    private int player1Symbol = 1; // X
    private int player2Symbol = 2; // O
    private Integer version = 1; // Добавленное поле для оптимистичной блокировки

    @Transient
    private transient int currentPlayerSymbol;

    public Game() {
//        this.id = UUID.randomUUID();
        this.board = new GameBoard();
        this.previousBoard = new int[3][3];
        this.state = GameState.WAITING_FOR_PLAYERS;
    }


    public int getCurrentPlayerSymbol() {
        return switch(state) {
            case PLAYER_1_TURN -> player1Symbol;
            case PLAYER_2_TURN -> player2Symbol;
            default -> 0;
        };
    }
}
