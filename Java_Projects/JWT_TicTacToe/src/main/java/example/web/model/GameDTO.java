package example.web.model;

import example.domain.model.GameState;
import lombok.Getter;
import lombok.Setter;

import java.util.UUID;

@Setter
@Getter
public class GameDTO {
    private UUID id;
    private int[][] board;
    private UUID player1Id;
    private UUID player2Id;
    private GameState state;
    private String errorMessage;
    private Integer version;
    private boolean vsComputer;
    private int[][] previousBoard;
}
