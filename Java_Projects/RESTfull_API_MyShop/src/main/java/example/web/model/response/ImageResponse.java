package example.web.model.response;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Getter;
import lombok.Setter;
import java.util.UUID;

@Getter
@Setter
public class ImageResponse {
    @Schema(description = "ID изображения", example = "eeeeeeee-eeee-eeee-eeee-eeeeeeeeeee1")
    private UUID id;
    @Schema(description = "Имя файла", example = "image_eeeeeeee-eeee-eeee-eeee-eeeeeeeeeee1.bin")
    private String fileName;
    @Schema(description = "Размер файла в байтах", example = "68")
    private long size;
    @Schema(description = "Content-Type", example = "application/octet-stream")
    private String contentType;
    @Schema(description = "ID товара", example = "dddddddd-dddd-dddd-dddd-ddddddddddd1")
    private UUID productId;
}
