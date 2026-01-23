package example.datasource.repository.impl;

import java.util.Optional;
import java.util.UUID;

import example.datasource.mapper.AddressRowMapper;
import example.datasource.model.AddressDAO;
import example.datasource.repository.AddressRepository;
import org.springframework.dao.EmptyResultDataAccessException;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Repository;

@Repository
public class JdbcAddressRepository implements AddressRepository {
    private final JdbcTemplate jdbcTemplate;
    private final AddressRowMapper addressRowMapper;

    public JdbcAddressRepository(JdbcTemplate jdbcTemplate, AddressRowMapper addressRowMapper) {
        this.jdbcTemplate = jdbcTemplate;
        this.addressRowMapper = addressRowMapper;
    }

    // ============ PUBLIC METHODS ============
    @Override
    public AddressDAO save (AddressDAO address){
        if (address.getId() == null) {
            return insert(address);
        } else {
            return update(address);
        }
    }


    @Override
    public boolean existsById(UUID id) {

        String sql = "SELECT EXISTS(SELECT 1 FROM addresses WHERE id = ?)";
        return Boolean.TRUE.equals(jdbcTemplate.queryForObject(sql, Boolean.class, id));

    }

    // ============ PRIVATE HELPER METHODS ============

    private AddressDAO insert (AddressDAO address){
        String sql = """
                INSERT INTO addresses (country, city, street)
                VALUES (?, ?, ?)
                RETURNING id
                """;
        UUID newId = jdbcTemplate.queryForObject(sql, UUID.class,
                address.getCountry(),
                address.getCity(),
                address.getStreet()
        );
        address.setId(newId);
        return address;
    }

    private AddressDAO update (AddressDAO address){
        String sql = """
                UPDATE addresses
                SET country = ?, city = ?, street = ?
                WHERE id = ?
                """;
        int rowsUpdated = jdbcTemplate.update(sql,
                address.getCountry(),
                address.getCity(),
                address.getStreet(),
                address.getId()
        );
        if (rowsUpdated == 0) {
            throw new RuntimeException("Address not found with id: " + address.getId());        }
        return address;
    }

    @Override
    public Optional<AddressDAO> findById(UUID id){
        String sql = "SELECT * FROM addresses WHERE id = ?";
        try{
            AddressDAO address = jdbcTemplate.queryForObject(sql, addressRowMapper, id);
            return Optional.ofNullable(address);
        }catch (EmptyResultDataAccessException e) {
            return Optional.empty();
        }
    }

    @Override
    public void deleteById(UUID id) {
        String sql = "DELETE FROM addresses WHERE id = ?";
        jdbcTemplate.update(sql, id);
    }

}
