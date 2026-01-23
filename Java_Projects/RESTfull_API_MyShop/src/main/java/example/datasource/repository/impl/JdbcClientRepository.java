package example.datasource.repository.impl;

import example.datasource.model.ClientDAO;
import example.datasource.repository.ClientRepository;
import example.datasource.mapper.ClientRowMapper;
import org.springframework.dao.EmptyResultDataAccessException;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Repository;

import java.time.LocalDate;
import java.util.List;
import java.util.Optional;
import java.util.UUID;


@Repository
public class JdbcClientRepository implements ClientRepository{
    private final JdbcTemplate jdbcTemplate;
    private final ClientRowMapper clientRowMapper;

    public JdbcClientRepository (JdbcTemplate jdbcTemplate, ClientRowMapper clientRowMapper){
        this.jdbcTemplate = jdbcTemplate;
        this.clientRowMapper = clientRowMapper;
    }

    private ClientDAO insert (ClientDAO client){
        String sql = """
                INSERT INTO clients
                (client_name, client_surname, birthday, gender, registration_date, address_id)
                VALUES (?, ?, ?, ? , ?, ?)
                RETURNING id
                """;

        // Выполняем INSERT

        UUID newId = jdbcTemplate.queryForObject(sql, UUID.class,
                client.getClientName(),
                client.getClientSurname(),
                java.sql.Date.valueOf(client.getBirthday()),
                client.getGender(),
                // Убедись что registrationDate не null
                client.getRegistrationDate() != null ?
                        java.sql.Date.valueOf(client.getRegistrationDate()) :
                        java.sql.Date.valueOf(LocalDate.now()), // Значение по умолчанию
                client.getAddressId()
                );

        // Устанавливаем сгенерированный ID
        client.setId(newId);

        return client;
    }
    private ClientDAO update (ClientDAO client){
        String sql = """
                UPDATE clients
                SET client_name = ?,
                    client_surname = ?,
                    birthday = ?,
                    gender = ?,
                    registration_date =?,
                    address_id =?
                WHERE id = ?
                """;

        // Выполняем UPDATE

        int rowsUpdated = jdbcTemplate.update(sql,
                client.getClientName(),
                client.getClientSurname(),
                java.sql.Date.valueOf(client.getBirthday()),
                client.getGender(),

                client.getRegistrationDate() != null ?
                        java.sql.Date.valueOf(client.getRegistrationDate()) :
                        java.sql.Date.valueOf(LocalDate.now()), // Добавить default

                client.getAddressId(),
                client.getId()
                );

        // Проверяем, что клиент существовал
        if (rowsUpdated == 0) {
            throw new RuntimeException("Client not found with id: " + client.getId());
        }

        return client;
    }

    @Override
    public ClientDAO save(ClientDAO client){
        if (client.getId() == null){
            return insert(client);  // Новый клиент
        } else {
            return update(client);  // Существующий клиент
        }
    }

    @Override
    public boolean deleteById(UUID id) {
        String sql = "DELETE FROM clients WHERE id = ?";

        // update() возвращает количество измененных строк
        int rowsDeleted = jdbcTemplate.update(sql, id);

        // Возвращаем true если удалена хотя бы 1 строка
        return rowsDeleted > 0;
    }

    @Override
    public List<ClientDAO> findByNameAndSurname(String name, String surname) {
        String sql = "SELECT * FROM clients WHERE client_name = ? AND client_surname = ?";
        return jdbcTemplate.query(sql, clientRowMapper, name, surname);
    }

    @Override
    public Optional<ClientDAO> findById(UUID id) {
        String sql = "SELECT * FROM clients WHERE id = ?";

        try{
            ClientDAO client = jdbcTemplate.queryForObject(sql, clientRowMapper, id);
            return Optional.ofNullable(client); //вернет пустой Optional
        } catch (EmptyResultDataAccessException e){
            // Клиент не найден
            return Optional.empty();
        }
    }

    @Override
    public List<ClientDAO> findAll() {
        String sql = "SELECT * FROM clients";
        return jdbcTemplate.query(sql, clientRowMapper);
    }

    @Override
    public List<ClientDAO> findAll(int limit, int offset) {
        String sql = "SELECT * FROM clients ORDER BY registration_date DESC, id LIMIT ? OFFSET ?";
        return jdbcTemplate.query(sql, clientRowMapper, limit, offset);
    }

    @Override
    public boolean updateClientAddress(UUID clientId, UUID newAddressId) {
        String sql = "UPDATE clients SET address_id = ? WHERE id = ?";

        // update() возвращает количество измененных строк
        int rowsUpdated = jdbcTemplate.update(sql, newAddressId, clientId);

        // Возвращаем true если изменена хотя бы 1 строка
        return rowsUpdated > 0;
    }
}
