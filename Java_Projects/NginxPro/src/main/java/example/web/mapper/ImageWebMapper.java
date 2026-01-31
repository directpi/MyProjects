package example.web.mapper;

import example.domain.exception.ValidationException;
import example.domain.model.ImageDTO;
import example.web.model.response.ImageResponse;
import org.springframework.stereotype.Component;

import java.util.UUID;

@Component
public class ImageWebMapper {

    public ImageDTO toDTO(byte[] imageData, UUID productId) {
        if (imageData == null || imageData.length == 0) {
            throw new ValidationException("Данные изображения не могут быть пустыми");
        }
        if (imageData.length > 10 * 1024 * 1024) {
            throw new ValidationException("Размер изображения не должен превышать 10MB");
        }
        if (productId == null) {
            throw new ValidationException("ID товара обязателен");
        }

        ImageDTO dto = new ImageDTO();
        dto.setImageData(imageData);
        dto.setProductId(productId);
        dto.setContentType("application/octet-stream");
        dto.setSize((long) imageData.length);
        dto.setFileName("image_" + productId + ".bin");
        return dto;
    }

    public ImageResponse toResponse(ImageDTO dto) {
        if (dto == null)
            return null;

        ImageResponse response = new ImageResponse();
        response.setId(dto.getId());
        response.setProductId(dto.getProductId());
        response.setFileName(dto.getFileName() != null ? dto.getFileName() : "image_" + dto.getId() + ".bin");
        response.setSize(
                dto.getSize() != null ? dto.getSize() : (dto.getImageData() != null ? dto.getImageData().length : 0));
        response.setContentType("application/octet-stream");

        return response;
    }
}
