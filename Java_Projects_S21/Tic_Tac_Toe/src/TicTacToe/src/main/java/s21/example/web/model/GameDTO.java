package s21.example.web.model;

import lombok.Getter;
import lombok.Setter;

import java.util.UUID;

@Setter
@Getter
public class GameDTO {
    private UUID id;
    private int[][] board;
}
