package example.datasource.mapper;

import example.datasource.model.ProductDAO;
import org.springframework.jdbc.core.RowMapper;
import org.springframework.stereotype.Component;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.UUID;

@Component
public class ProductRowMapper implements RowMapper<ProductDAO> {

    @Override
    public ProductDAO mapRow(ResultSet rs, int rowNum) throws SQLException{
        ProductDAO product = new ProductDAO();

        product.setId(rs.getObject("id", UUID.class));
        product.setName(rs.getString("name"));

        // Category ID
        UUID categoryId = rs.getObject("category_id", UUID.class);
        product.setCategoryId(categoryId);

        // Category Code (из JOIN)
        String categoryCode = rs.getString("category_code");
        product.setCategoryCode(categoryCode);


        product.setPrice(rs.getBigDecimal("price"));
        product.setAvailableStock(rs.getInt("available_stock"));

        // Проверяем NULL для даты
        java.sql.Date lastUpdateDate = rs.getDate("last_update_date");
        product.setLastUpdateDate(lastUpdateDate != null ? lastUpdateDate.toLocalDate() : null);
        // Поля, которые могут быть NULL
        UUID supplierId = rs.getObject("supplier_id", UUID.class);
        product.setSupplierId(rs.wasNull() ? null : supplierId);

        UUID imageId = rs.getObject("image_id", UUID.class);
        product.setImageId(rs.wasNull() ? null : imageId);

        return product;
    }

}
