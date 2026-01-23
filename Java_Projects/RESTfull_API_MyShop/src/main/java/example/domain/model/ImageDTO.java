package example.domain.model;

import lombok.Getter;
import lombok.Setter;
import java.util.UUID;

@Getter
@Setter
public class ImageDTO {
    private UUID id;
    private byte[] imageData;
    private UUID productId; // Связь с товаром
    private String fileName;
    private String contentType;
    private Long size;
}
