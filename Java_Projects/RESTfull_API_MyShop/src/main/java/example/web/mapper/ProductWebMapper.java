package example.web.mapper;

import example.domain.model.ProductDTO;
import example.web.model.request.CreateProductRequest;
import example.web.model.response.ProductResponse;
import org.springframework.stereotype.Component;

@Component
public class ProductWebMapper {

    // Request → Domain DTO
    public ProductDTO toDTO(CreateProductRequest request) {
        if (request == null) {
            return null;
        }
        ProductDTO dto = new ProductDTO();
        dto.setName(request.getName());
        dto.setCategoryCode(request.getCategory());// Из request.category → DTO.categoryCode
        dto.setPrice(request.getPrice());
        dto.setAvailableStock(request.getAvailableStock());
        dto.setLastUpdateDate(request.getLastUpdateDate());
        dto.setSupplierId(request.getSupplierId());
        dto.setImageId(request.getImageId());

        return dto;
    }

    // Domain DTO → Response
    public ProductResponse toResponse(ProductDTO dto){
        if (dto == null) {
            return null;
        }
        ProductResponse response = new ProductResponse();
        response.setId(dto.getId());
        response.setName(dto.getName());
        response.setCategory(dto.getCategoryCode());// Из DTO.categoryCode → response.category
        response.setPrice(dto.getPrice());
        response.setAvailableStock(dto.getAvailableStock());
        response.setLastUpdateDate(dto.getLastUpdateDate());
        response.setSupplierId(dto.getSupplierId());
        response.setImageId(dto.getImageId());

        return response;
    }
}
