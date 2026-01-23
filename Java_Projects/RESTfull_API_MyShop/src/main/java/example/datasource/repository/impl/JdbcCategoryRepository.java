package example.datasource.repository.impl;

import example.datasource.mapper.CategoryRowMapper;
import example.datasource.model.CategoryDAO;
import example.datasource.repository.CategoryRepository;
import org.aspectj.apache.bcel.classfile.Module;
import org.springframework.dao.EmptyResultDataAccessException;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Repository;

import java.util.Optional;
import java.util.UUID;

@Repository
public class JdbcCategoryRepository implements CategoryRepository {
    private final JdbcTemplate jdbcTemplate;
    private final CategoryRowMapper categoryRowMapper;

    public JdbcCategoryRepository(JdbcTemplate jdbcTemplate, CategoryRowMapper categoryRowMapper) {
        this.jdbcTemplate = jdbcTemplate;
        this.categoryRowMapper = categoryRowMapper;
    }

    @Override
    public Optional<CategoryDAO> findById(UUID id){
        String sql = "SELECT * FROM categories WHERE id = ?";
        try{
            CategoryDAO category = jdbcTemplate.queryForObject(sql, categoryRowMapper, id);
            return Optional.ofNullable(category);
        } catch (EmptyResultDataAccessException e){
            return Optional.empty();
        }
    }

    @Override
    public Optional<CategoryDAO> findByCode (String code){
        String sql = "SELECT * FROM categories WHERE code = ?";
        try {
            CategoryDAO category = jdbcTemplate.queryForObject(sql, categoryRowMapper, code);
            return Optional.ofNullable(category);
        } catch (EmptyResultDataAccessException e) {
            return Optional.empty();
        }
    }
}
