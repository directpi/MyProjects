package example.datasource.repository.impl;

import example.datasource.model.SupplierDAO;
import example.datasource.repository.SupplierRepository;
import example.datasource.mapper.SupplierRowMapper;
import org.springframework.dao.EmptyResultDataAccessException;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Repository;

import java.util.List;
import java.util.Optional;
import java.util.UUID;

@Repository
public class JdbcSupplierRepository implements SupplierRepository{
    private final JdbcTemplate jdbcTemplate;
    private final SupplierRowMapper supplierRowMapper;

    public JdbcSupplierRepository(JdbcTemplate jdbcTemplate, SupplierRowMapper supplierRowMapper) {
        this.jdbcTemplate = jdbcTemplate;
        this.supplierRowMapper = supplierRowMapper;
    }

    private SupplierDAO insert (SupplierDAO supplier){
        String sql = """
                INSERT INTO suppliers
                (name, address_id, phone_number)
                VALUES (?, ?, ?)
                RETURNING id
                """;

        // Выполняем INSERT

        UUID newId = jdbcTemplate.queryForObject(sql, UUID.class,
                supplier.getName(),
                supplier.getAddressId(),
                supplier.getPhoneNumber()
        );

        // Устанавливаем сгенерированный ID
        supplier.setId(newId);

        return supplier;
    }
    private SupplierDAO update (SupplierDAO supplier){
        String sql = """
                UPDATE suppliers
                SET name = ?,
                    address_id = ?,
                    phone_number = ?
                WHERE id = ?
                """;

        // Выполняем UPDATE

        int rowsUpdated = jdbcTemplate.update(sql,
                supplier.getName(),
                supplier.getAddressId(),
                supplier.getPhoneNumber(),
                supplier.getId()
        );

        // Проверяем, что поставщик существовал
        if (rowsUpdated == 0) {
            throw new RuntimeException("Supplier not found with id: " + supplier.getId());
        }

        return supplier;
    }
    @Override
    public SupplierDAO save(SupplierDAO supplier) {
        if (supplier.getId() == null){
            return insert(supplier);  // Новый поставщик
        } else {
            return update(supplier);  // Существующий поставщик
        }
    }

    @Override
    public boolean updateSupplierAddress(UUID supplierId, UUID newAddressId) {
        String sql = "UPDATE suppliers SET address_id = ? WHERE id = ?";
        int rowsUpdated = jdbcTemplate.update(sql, newAddressId, supplierId);
        return rowsUpdated > 0;
    }

    @Override
    public boolean deleteById(UUID id) {
        String sql = "DELETE FROM suppliers WHERE id = ?";

        // update() возвращает количество измененных строк
        int rowsDeleted = jdbcTemplate.update(sql, id);

        // Возвращаем true если удалена хотя бы 1 строка
        return rowsDeleted > 0;
    }

    @Override
    public List<SupplierDAO> findAll() {
        String sql = "SELECT * FROM suppliers";
        return jdbcTemplate.query(sql, supplierRowMapper);
    }

    @Override
    public Optional<SupplierDAO> findById(UUID id) {
        String sql = "SELECT * FROM suppliers WHERE id = ?";

        try{
            SupplierDAO supplier = jdbcTemplate.queryForObject(sql, supplierRowMapper, id);
            return Optional.ofNullable(supplier); //вернет пустой Optional
        } catch (EmptyResultDataAccessException e){
            // Поставщик не найден
            return Optional.empty();
        }
    }
}
