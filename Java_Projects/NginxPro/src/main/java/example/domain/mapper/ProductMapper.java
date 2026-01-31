package example.domain.mapper;

import example.datasource.model.ProductDAO;
import example.datasource.repository.CategoryRepository;
import example.domain.model.ProductDTO;
import org.springframework.stereotype.Component;

@Component
public class ProductMapper {
    private final CategoryRepository categoryRepository;

    public ProductMapper(CategoryRepository categoryRepository) {
        this.categoryRepository = categoryRepository;
    }

    public final ProductDTO toDTO(ProductDAO dao){
        if (dao == null) {
            return null;
        }
        ProductDTO dto = new ProductDTO();
        dto.setId(dao.getId());
        dto.setName(dao.getName());
        dto.setCategoryCode(dao.getCategoryCode());
        dto.setPrice(dao.getPrice());
        dto.setAvailableStock(dao.getAvailableStock());
        dto.setLastUpdateDate(dao.getLastUpdateDate());
        dto.setSupplierId(dao.getSupplierId());
        dto.setImageId(dao.getImageId());

        return dto;
    }

    public ProductDAO toDAO(ProductDTO dto) {
        if (dto == null) return null;

        ProductDAO dao = new ProductDAO();
        dao.setId(dto.getId());
        dao.setName(dto.getName());
        dao.setCategoryCode(dto.getCategoryCode());

        // Находим ID категории по коду
        var category = categoryRepository.findByCode(dto.getCategoryCode())
                .orElseThrow(() -> new RuntimeException("Категория не найдена: " + dto.getCategoryCode()));
        dao.setCategoryId(category.getId());

        dao.setPrice(dto.getPrice());
        dao.setAvailableStock(dto.getAvailableStock());
        dao.setLastUpdateDate(dto.getLastUpdateDate());
        dao.setSupplierId(dto.getSupplierId());
        dao.setImageId(dto.getImageId());

        return dao;
    }
}
