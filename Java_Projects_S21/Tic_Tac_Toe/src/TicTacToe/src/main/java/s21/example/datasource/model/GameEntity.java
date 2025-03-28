package s21.example.datasource.model;

import lombok.Getter;
import lombok.Setter;

import java.util.UUID;

@Getter
@Setter
public class GameEntity {
    private UUID id;
    private int[][] board;

    public GameEntity() {
    }
}
