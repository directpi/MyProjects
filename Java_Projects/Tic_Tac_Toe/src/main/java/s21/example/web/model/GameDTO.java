package s21.example.web.model;

import lombok.Getter;
import lombok.Setter;
import s21.example.domain.model.GameState;

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
}
