package example.web.model;

import jakarta.validation.constraints.NotNull;
import lombok.Getter;
import lombok.Setter;

@Getter
@Setter
public class MoveRequestDTO {
    @NotNull(message = "Board cannot be null")
    private int[][] board;

    @NotNull(message = "Version cannot be null")
    private Integer version;
}