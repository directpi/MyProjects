package example.domain.service;


import example.datasource.model.ProductDAO;
import example.datasource.repository.ProductRepository;
import example.domain.exception.EntityNotFoundException;
import example.domain.mapper.ProductMapper;
import example.domain.model.ProductDTO;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import example.domain.exception.ValidationException;
import org.springframework.transaction.annotation.Transactional;

import java.math.BigDecimal;
import java.time.LocalDate;
import java.util.List;
import java.util.Optional;
import java.util.UUID;
import java.util.stream.Collectors;

@Slf4j
@Service
public class ProductService {
    private final ProductRepository productRepository;
    private final ProductMapper productMapper;

    @Autowired
    public ProductService(ProductRepository productRepository, ProductMapper productMapper) {
        this.productRepository = productRepository;
        this.productMapper = productMapper;
    }

    // ============ PUBLIC METHODS ============

    // 1. Добавление товара
    @Transactional
    public ProductDTO createProduct(ProductDTO productDTO){
        try{
            log.debug("Попытка создать продукт: {}", productDTO.getName());
            // Валидация
            validateProduct(productDTO);

            // Устанавливаем дату обновления, если не задана
            if (productDTO.getLastUpdateDate() == null) {
                productDTO.setLastUpdateDate(LocalDate.now());
            }

            // Маппинг DTO → DAO
            ProductDAO productDAO = productMapper.toDAO(productDTO);

            // Сохранение в БД
            ProductDAO savedDAO = productRepository.save(productDAO);

            // Маппинг обратно DAO → DTO
            ProductDTO created = productMapper.toDTO(savedDAO);
            log.info("Продукт : {} создан успешно с ID: {}", created.getName(), created.getId());
            return created;
        } catch (Exception e){
            log.error("Не удалось создать продукт c ID: {}", e.getMessage(), e);
            throw new ValidationException("Не удалось создать продукт. Пожалуйста, проверьте данные.");
        }
    }

    // 2. Уменьшение количества товара
    @Transactional
    public void decreaseStock(UUID productId, int amount) {
        log.debug("Попытка уменьшить количество товара с ID : {} на : {}", productId, amount);
        if (productId == null) {
            throw new ValidationException("Продукт Id не должен быть null");
        }

        if (amount <= 0) {
            throw new ValidationException("Количество не должно быть меньше нуля");
        }
        // Проверяем, что товар существует
        getById(productId);

        // Уменьшаем количество
        boolean success = productRepository.decreaseStock(productId, amount);
        if (!success) {
            log.warn("Не удалось уменьшить количество товара с ID: {}. Недостаточно количества.", productId);
            throw new RuntimeException("Невозможно уменьшить количество товара. Не достаточно количества");
        }
        int newStock = productRepository.getAvailableStock(productId);
        log.info("Количество товара с ID: {} уменьшено, новое количество: {}", productId, newStock);
    }

    public ProductDTO getById(UUID productId) {
        return findById(productId).orElseThrow(() -> new EntityNotFoundException("Товар",productId));
    }

    // 3. Получение товара по ID
    private Optional<ProductDTO> findById (UUID id) {
        if (id == null) {
            throw new ValidationException("Id продукта не должен быть null");
        }
        return productRepository.findById(id).map(productMapper::toDTO);
    }

    // 4. Получение всех доступных товаров
    public List<ProductDTO> getAllAvailableProducts (){
        // Получаем все товары и фильтруем по available_stock > 0
        return productRepository.findAll().stream()
                .filter(product -> product.getAvailableStock() > 0) // Закомментировать для получения всех в т.ч. и недоступных
                .map(productMapper::toDTO)
                .collect(Collectors.toList());
    }

    // 5. Удаление товара по ID
    @Transactional
    public void deleteById (UUID id){
        log.debug("Попытка удалить товар с ID: {}", id);
        if (id == null){
            throw new ValidationException("Id товара не может быть null");
        }

        // Проверяем существование
        getById(id);

        //Удаляем
        boolean deleted = productRepository.deleteById(id);
        if (!deleted){
            log.warn("Невозможно удалить товар с ID: {}.", id);
            throw new EntityNotFoundException("Товар", id);
        }
        log.info("Товар с ID: {} удален", id);
    }

    // 6. Увеличение количества товара (дополнительно)
    @Transactional
    public void increaseStock(UUID productId, int amount) {
        log.debug("Попытка увеличить количество товара с ID: {} на: {}", productId, amount);
        if (productId == null) {
            throw new ValidationException("Id товара не может быть null");
        }
        if (amount <= 0) {
            throw new ValidationException("Количество не должно быть меньше нуля");
        }

        // Проверяем, что товар существует
        getById(productId);

        // Увеличиваем количество
        boolean success = productRepository.increaseStock(productId, amount);
        if (!success) {
            log.warn("Не удалось увеличить количество товара с ID: {}.", productId);

            throw new EntityNotFoundException("Товар", productId);
        }
        log.info("Количество товара с ID: {} увеличено, новое количество: {}",
                productId, getCurrentStock(productId));
    }

    // 7. Получение текущего количества товара
    public int getCurrentStock (UUID productId){
        if (productId == null) {
            throw new ValidationException("Id продукта не может быть null");
        }

        // Проверяем существование товара
        getById(productId);

        return productRepository.getAvailableStock(productId);
    }

    // ============ PRIVATE HELPER METHODS ============

    private void validateProduct(ProductDTO productDTO) {
        if (productDTO == null) {
            throw new ValidationException("Данные продукта не могут быть null");
        }

        //Валидация названия
        validateName(productDTO.getName());

        //Валидация категории
        validateCategory(productDTO.getCategoryCode());

        // Валидация цены
        validatePrice(productDTO.getPrice());

        // Валидация количества
        validateStock(productDTO.getAvailableStock());

        //Валидация даты последнего изменения
        validateDate(productDTO.getLastUpdateDate());

    }

    private void validateName(String name) {
        if (name == null || name.trim().isEmpty()){
            throw new ValidationException("Название продукта не может быть пустым");
        }
        if (name.length() < 2) {
            throw new ValidationException("Название продукта должно состоять как минимум из 2-х символов");
        }
        if (name.length() > 100) {
            throw new ValidationException("Название продукта должно состоять не более чем из 100 символов");
        }
    }

    private void validateCategory(String categoryCode) {
        if (categoryCode == null || categoryCode.trim().isEmpty()) {
            throw new ValidationException("Код категории продукта не может быть пустой");
        }
    }

    private void validatePrice(BigDecimal price) {
        if (price == null){
            throw new ValidationException("Цена продукта не может быть null");
        }
        if (price.compareTo(BigDecimal.ZERO) < 0) {
            throw new ValidationException("Цена продукта не может быть отрицательной");
        }
        if (price.compareTo(new BigDecimal("9999999.99")) > 0){
            throw new ValidationException("Цена продукта слишком большая");
        }
    }

    private void validateStock(Integer availableStock) {
        if (availableStock == null){
            throw new ValidationException("Количество продукта не может быть null");
        }
        if (availableStock < 0) {
            throw new ValidationException("Количество продукта не может быть отрицательным");
        }
    }
    private void validateDate(LocalDate lastUpdateDate) {
        if (lastUpdateDate == null) {
            return;
        }
        if (lastUpdateDate.isAfter(LocalDate.now())){
            throw new ValidationException("Дата изменения не может быть в будущем");
        }
        if (lastUpdateDate.isBefore(LocalDate.of(2000, 1, 1))){
            throw new ValidationException("Дата изменения должна быть после 01.01.2000");
        }
    }
}
