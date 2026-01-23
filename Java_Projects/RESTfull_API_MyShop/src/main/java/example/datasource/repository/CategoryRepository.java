package example.datasource.repository;

import example.datasource.model.CategoryDAO;

import java.util.Optional;
import java.util.UUID;

public interface CategoryRepository {
    Optional<CategoryDAO> findById(UUID id);
    Optional<CategoryDAO> findByCode(String code);
}
