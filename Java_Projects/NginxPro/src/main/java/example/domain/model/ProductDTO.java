package example.domain.model;

import com.fasterxml.jackson.annotation.JsonProperty;
import example.datasource.model.CategoryDAO;
import lombok.Getter;
import lombok.Setter;

import java.math.BigDecimal;
import java.time.LocalDate;
import java.util.UUID;

@Getter
@Setter
public class ProductDTO {
    private UUID id;
    private String name;

    // Для JSON API: "category" (как в задании)
    // Для внутреннего использования: храним код категории
    @JsonProperty("category")
    private String categoryCode;

    private BigDecimal price;
    private Integer availableStock;
    private LocalDate lastUpdateDate;
    private UUID supplierId;
    private UUID imageId;
}