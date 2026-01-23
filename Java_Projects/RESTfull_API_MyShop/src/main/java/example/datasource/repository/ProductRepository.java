package example.datasource.repository;

import example.datasource.model.ProductDAO;

import java.util.List;
import java.util.Optional;
import java.util.UUID;

public interface ProductRepository {// CRUD операции

    // CRUD операции
    ProductDAO save(ProductDAO product);
    Optional<ProductDAO> findById(UUID id);
    List<ProductDAO> findAll();
    boolean deleteById(UUID id);

    // Операции с количеством товара
    boolean decreaseStock(UUID productId, int amount);  // Уменьшение
    boolean increaseStock(UUID productId, int amount);  // Увеличение

    // Опционально: проверка доступного количества
    int getAvailableStock(UUID productId);
}
