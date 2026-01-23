package example.datasource.model;

import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

import java.math.BigDecimal;
import java.time.LocalDate;
import java.util.UUID;

@Getter
@Setter
@NoArgsConstructor
@AllArgsConstructor
public class ProductDAO {
    private UUID id;
    private String name;
    private UUID categoryId;    // ID категории (для БД)
    private String categoryCode;// Код категории (для удобства)
    private BigDecimal price;
    private Integer availableStock;
    private LocalDate lastUpdateDate;
    private UUID supplierId;
    private UUID imageId;
    }
