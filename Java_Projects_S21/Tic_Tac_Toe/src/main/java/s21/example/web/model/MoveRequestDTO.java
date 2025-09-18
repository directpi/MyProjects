package s21.example.web.model;

import lombok.Getter;
import lombok.Setter;
import jakarta.validation.constraints.NotNull;

@Getter
@Setter
public class MoveRequestDTO {
    @NotNull(message = "Board cannot be null")
    private int[][] board;

    @NotNull(message = "Version cannot be null")
    private Integer version;
}