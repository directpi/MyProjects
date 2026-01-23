package example.web.model.request;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.*;
import lombok.Getter;
import lombok.Setter;

import java.math.BigDecimal;
import java.time.LocalDate;

@Getter
@Setter
public class CreateProductRequest {
    @Schema(description = "Название товара", example = "Холодильник Samsung RB33J")
    @NotBlank(message = "Название товара обязательно")
    @Size(min = 2, max = 100, message = "Название должно быть от 2 до 100 символов")
    private String name;

    @Schema(description = "Код категории", example = "REFRIGERATORS")
    @NotBlank(message = "Категория товара обязательна")
    private String category;

    @Schema(description = "Цена товара", example = "45999.99")
    @NotNull(message = "Цена товара обязательна")
    @DecimalMin(value = "0.0", inclusive = false, message = "Цена должна быть больше 0")
    @DecimalMax(value = "9999999.99", message = "Цена не может превышать 9999999.99")
    private BigDecimal price;

    @Schema(description = "Количество на складе", example = "15")
    @NotNull(message = "Количество товара обязательно")
    @Min(value = 0, message = "Количество не может быть отрицательным")
    private Integer availableStock;

    @Schema(description = "Дата последнего обновления", example = "2024-01-15")
    private LocalDate lastUpdateDate;
    @Schema(description = "ID поставщика", example = "cccccccc-cccc-cccc-cccc-ccccccccccc1")
    private java.util.UUID supplierId;
    @Schema(description = "ID изображения", example = "eeeeeeee-eeee-eeee-eeee-eeeeeeeeeee1")
    private java.util.UUID imageId;
}
