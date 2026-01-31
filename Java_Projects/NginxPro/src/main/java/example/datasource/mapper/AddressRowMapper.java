package example.datasource.mapper;

import example.datasource.model.AddressDAO;

import java.util.UUID;

import org.springframework.jdbc.core.RowMapper;
import org.springframework.stereotype.Component;

import java.sql.ResultSet;
import java.sql.SQLException;

@Component
public class AddressRowMapper implements RowMapper<AddressDAO> {
    // маппинг ResultSet → AddressDAO
    @Override
    public AddressDAO mapRow(ResultSet rs, int rowNum) throws SQLException {
        AddressDAO address = new AddressDAO();

        // Маппим поля адреса из ResultSet в AddressDAO
        address.setId(rs.getObject("id", UUID.class));
        address.setCountry(rs.getString("country"));
        address.setCity(rs.getString("city"));
        address.setStreet(rs.getString("street"));

        return address;
    }
}
