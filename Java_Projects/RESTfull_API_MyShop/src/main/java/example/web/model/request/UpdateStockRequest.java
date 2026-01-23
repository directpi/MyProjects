package example.web.model.request;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.Min;
import jakarta.validation.constraints.NotNull;
import lombok.Getter;
import lombok.Setter;

@Getter
@Setter
public class UpdateStockRequest {

    @Schema(description = "Изменение количества", example = "2")
    @NotNull(message = "Количество обязательно")
    @Min(value = 1, message = "Количество должно быть положительным числом")
    private Integer amount;
}
