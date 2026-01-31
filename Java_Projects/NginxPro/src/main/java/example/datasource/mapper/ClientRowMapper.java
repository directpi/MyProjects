package example.datasource.mapper;

import example.datasource.model.ClientDAO;

//import javax.swing.tree.RowMapper;
import org.springframework.jdbc.core.RowMapper;
import org.springframework.stereotype.Component;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.UUID;

@Component
public class ClientRowMapper implements RowMapper<ClientDAO> {

    @Override
    public ClientDAO mapRow(ResultSet rs, int rowNum) throws SQLException {
        ClientDAO client = new ClientDAO();

        // Маппим каждое поле из ResultSet в ClientDAO
        client.setId(rs.getObject("id", UUID.class));
        client.setClientName(rs.getString("client_name"));
        client.setClientSurname(rs.getString("client_surname"));
        client.setBirthday(rs.getDate("birthday").toLocalDate());
        client.setGender(rs.getString("gender"));
        client.setRegistrationDate(rs.getDate("registration_date").toLocalDate());
        client.setAddressId(rs.getObject("address_id", UUID.class));
        return client;
    }
}
