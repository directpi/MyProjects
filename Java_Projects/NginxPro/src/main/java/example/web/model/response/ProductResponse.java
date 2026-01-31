package example.web.model.response;

import com.fasterxml.jackson.annotation.JsonFormat;
import com.fasterxml.jackson.annotation.JsonInclude;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Getter;
import lombok.Setter;

import java.math.BigDecimal;
import java.time.LocalDate;
import java.util.UUID;

@Getter
@Setter
@JsonInclude(JsonInclude.Include.NON_NULL)
public class ProductResponse {
    @Schema(description = "ID товара", example = "dddddddd-dddd-dddd-dddd-ddddddddddd1")
    private UUID id;
    @Schema(description = "Название товара", example = "Холодильник Samsung RB33J")
    private String name;
    @Schema(description = "Код категории", example = "REFRIGERATORS")
    private String category;
    @Schema(description = "Цена", example = "45999.99")
    private BigDecimal price;
    @Schema(description = "Количество на складе", example = "15")
    private Integer availableStock;

    @JsonFormat(pattern = "yyyy-MM-dd")
    @Schema(description = "Дата последнего обновления", example = "2024-01-15")
    private LocalDate lastUpdateDate;

    @Schema(description = "ID поставщика", example = "cccccccc-cccc-cccc-cccc-ccccccccccc1")
    private UUID supplierId;
    @Schema(description = "ID изображения", example = "eeeeeeee-eeee-eeee-eeee-eeeeeeeeeee1")
    private UUID imageId;
}
