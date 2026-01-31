package example.datasource.repository.impl;

import example.datasource.mapper.ProductRowMapper;
import example.datasource.model.ProductDAO;
import example.datasource.repository.CategoryRepository;
import example.datasource.repository.ProductRepository;
import org.springframework.dao.EmptyResultDataAccessException;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Repository;

import java.util.List;
import java.util.Optional;
import java.util.UUID;

@Repository
public class JdbcProductRepository implements ProductRepository {
    private final JdbcTemplate jdbcTemplate;
    private final ProductRowMapper productRowMapper;
    private final CategoryRepository categoryRepository;

    public JdbcProductRepository(JdbcTemplate jdbcTemplate, ProductRowMapper productRowMapper, CategoryRepository categoryRepository) {
        this.jdbcTemplate = jdbcTemplate;
        this.productRowMapper = productRowMapper;
        this.categoryRepository = categoryRepository;
    }

    @Override
    public ProductDAO save(ProductDAO product){
        if (product.getId() == null){
            // INSERT
            String sql = """
                    INSERT INTO products
                    (name, category_id, price, available_stock, last_update_date, supplier_id, image_id)
                    VALUES (?, ?, ?, ?, ?, ?, ?)
                    RETURNING id
                    """;
            UUID newId = jdbcTemplate.queryForObject(sql, UUID.class,
                    product.getName(),
                    product.getCategoryId(),
                    product.getPrice(),
                    product.getAvailableStock(),
                    product.getLastUpdateDate() != null ?
                            java.sql.Date.valueOf(product.getLastUpdateDate()) : null,
                    product.getSupplierId(),
                    product.getImageId()
            );

            product.setId(newId);
        } else {
            // UPDATE
            String sql = """
                    UPDATE products SET
                    name = ?,
                    category_id = ?,
                    price = ?,
                    available_stock = ?,
                    last_update_date = ?,
                    supplier_id = ?,
                    image_id = ?
                    WHERE id = ?
                    """;

            jdbcTemplate.update(sql,
                    product.getName(),
                    product.getCategoryId(),
                    product.getPrice(),
                    product.getAvailableStock(),
                    product.getLastUpdateDate() != null ?
                            java.sql.Date.valueOf(product.getLastUpdateDate()) : null,                    product.getSupplierId(),
                    product.getImageId(),
                    product.getId()
                    );
        }

        return product;
    }

    @Override
    public Optional<ProductDAO> findById(UUID id){
        String sql = """
                SELECT p.*, c.code as category_code
                FROM products p
                LEFT JOIN categories c ON p.category_id = c.id
                WHERE p.id = ?
                """;

        try {
            ProductDAO product = jdbcTemplate.queryForObject(sql, productRowMapper, id);

            return Optional.ofNullable(product);
        } catch (EmptyResultDataAccessException e) {
            return Optional.empty();
        }
    }

    @Override
    public List<ProductDAO> findAll() {
        String sql = """
                SELECT p.*, c.code as category_code
                FROM products p
                LEFT JOIN categories c ON p.category_id = c.id
                """;

        return jdbcTemplate.query(sql, productRowMapper);
    }

    @Override
    public boolean deleteById(UUID id) {
        String sql = "DELETE FROM products WHERE id = ?";
        int rowsDeleted = jdbcTemplate.update(sql, id);
        return rowsDeleted > 0;
    }

    @Override
    public boolean decreaseStock(UUID productId, int amount) {
        if (amount <= 0) {
            throw new IllegalArgumentException("Количество должно быть больше нуля");
        }

        String sql = """
                UPDATE products
                SET available_stock = available_stock - ?,
                    last_update_date = CURRENT_DATE
                WHERE id = ? AND available_stock >= ?
                """;
        int rowsUpdated = jdbcTemplate.update(sql, amount, productId, amount);
        return rowsUpdated > 0;
    }

    @Override
    public boolean increaseStock(UUID productId, int amount) {
        if (amount <= 0) {
            throw new IllegalArgumentException("Количество должно быть больше нуля");
        }

        String sql = """
                UPDATE products
                SET available_stock = available_stock + ?,
                    last_update_date = CURRENT_DATE
                WHERE id = ?
                """;
        int rowsUpdated = jdbcTemplate.update(sql, amount, productId);
        return rowsUpdated > 0;
    }

    @Override
    public int getAvailableStock(UUID productId) {
        String sql = "SELECT available_stock FROM products WHERE id = ?";
        try {
            Integer stock = jdbcTemplate.queryForObject(sql, Integer.class, productId);
            return stock != null ? stock : 0;
        } catch (EmptyResultDataAccessException e){
            return 0;
        }
    }

}

