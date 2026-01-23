package example.datasource.mapper;

import example.datasource.model.SupplierDAO;

import org.springframework.jdbc.core.RowMapper;
import org.springframework.stereotype.Component;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.UUID;

@Component
public class SupplierRowMapper implements RowMapper<SupplierDAO>{
    @Override
    public SupplierDAO mapRow(ResultSet rs, int rowNum) throws SQLException {
        SupplierDAO supplier = new SupplierDAO();

        // Маппим каждое поле из ResultSet в SupplierDAO
        supplier.setId(rs.getObject("id", UUID.class));
        supplier.setName(rs.getString("name"));
        supplier.setAddressId(rs.getObject("address_id", UUID.class));
        supplier.setPhoneNumber(rs.getString("phone_number"));
        return supplier;
    }
}
