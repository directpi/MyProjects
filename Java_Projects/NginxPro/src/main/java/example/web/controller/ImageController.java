package example.web.controller;

import example.domain.exception.EntityNotFoundException;
import example.domain.exception.ValidationException;
import example.domain.service.ImageService;
import example.web.mapper.ImageWebMapper;
import example.web.model.response.ErrorResponse;
import example.web.model.response.ImageResponse;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.media.Content;
import io.swagger.v3.oas.annotations.media.Schema;
import io.swagger.v3.oas.annotations.responses.ApiResponse;
import io.swagger.v3.oas.annotations.tags.Tag;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.*;
import org.springframework.web.bind.annotation.*;
import java.util.UUID;

@Slf4j
@RestController
@RequestMapping("/api/v1/images")
@Tag(name = "Images", description = "API для работы с изображениями товаров")
public class ImageController {
    private final ImageService imageService;
    private final ImageWebMapper imageWebMapper;

    @Autowired
    public ImageController(ImageService imageService, ImageWebMapper imageWebMapper) {
        this.imageService = imageService;
        this.imageWebMapper = imageWebMapper;
    }

    // 1. Добавление изображения (с привязкой к товару)
    @PostMapping(consumes = MediaType.APPLICATION_OCTET_STREAM_VALUE)
    @Operation(summary = "Добавление изображения", description = "Загружает изображение и привязывает к товару", responses = {
            @ApiResponse(responseCode = "201", description = "Изображение создано", content = @Content(schema = @Schema(implementation = ImageResponse.class))),
            @ApiResponse(responseCode = "400", description = "Неверные данные", content = @Content(schema = @Schema(implementation = ErrorResponse.class))),
            @ApiResponse(responseCode = "404", description = "Товар не найден", content = @Content(schema = @Schema(implementation = ErrorResponse.class))),
            @ApiResponse(responseCode = "415", description = "Неподдерживаемый тип данных")
    })
    public ResponseEntity<ImageResponse> createImage(
            @io.swagger.v3.oas.annotations.parameters.RequestBody(description = "Бинарные данные изображения", required = true, content = @Content(mediaType = MediaType.APPLICATION_OCTET_STREAM_VALUE, schema = @Schema(type = "string", format = "binary"))) @RequestBody byte[] imageData,
            @Parameter(description = "ID товара для привязки", required = true) @RequestParam("productId") UUID productId) {

        log.info("POST /api/v1/images - Добавление изображения. Размер: {}, productId: {}",
                imageData != null ? imageData.length : 0, productId);

        try {
            if (productId == null) {
                throw new ValidationException("ID товара обязателен");
            }

            var imageDTO = imageWebMapper.toDTO(imageData, productId);
            var createdImage = imageService.createImage(imageDTO);
            var response = imageWebMapper.toResponse(createdImage);

            log.info("Изображение успешно создано: ID={}, для товара: {}",
                    response.getId(), response.getProductId());

            return ResponseEntity.status(HttpStatus.CREATED).body(response);

        } catch (ValidationException | EntityNotFoundException e) {
            // Эти исключения пробрасываем дальше для обработки в GlobalExceptionHandler
            log.warn("Ошибка при создании изображения: {}", e.getMessage());
            throw e;
        } catch (Exception e) {
            // Все остальные исключения логируем и оборачиваем
            log.error("Непредвиденная ошибка при создании изображения", e);
            throw new RuntimeException("Внутренняя ошибка сервера при создании изображения", e);
        }
    }

    // 2. Полная замена изображения
    @PutMapping(value = "/{id}", consumes = MediaType.APPLICATION_OCTET_STREAM_VALUE)
    @Operation(summary = "Полная замена изображения", description = "Полностью заменяет изображение по ID", responses = {
            @ApiResponse(responseCode = "204", description = "Изображение заменено"),
            @ApiResponse(responseCode = "400", description = "Неверные данные", content = @Content(schema = @Schema(implementation = ErrorResponse.class))),
            @ApiResponse(responseCode = "404", description = "Изображение не найдено", content = @Content(schema = @Schema(implementation = ErrorResponse.class))),
            @ApiResponse(responseCode = "415", description = "Неподдерживаемый тип данных")
    })
    public ResponseEntity<Void> replaceImage(
            @Parameter(description = "ID изображения", required = true) @PathVariable UUID id,
            @io.swagger.v3.oas.annotations.parameters.RequestBody(description = "Бинарные данные нового изображения", required = true, content = @Content(mediaType = MediaType.APPLICATION_OCTET_STREAM_VALUE, schema = @Schema(type = "string", format = "binary"))) @RequestBody byte[] imageData) {
        log.info("PUT /api/v1/images/{} - Замена изображения. Размер: {}",
                id, imageData != null ? imageData.length : 0);

        try {
            // Вызываем сервис
            imageService.replaceImage(id, imageData);

            log.info("Изображение {} успешно заменено", id);
            return ResponseEntity.noContent().build();

        } catch (ValidationException | EntityNotFoundException e) {
            log.warn("Ошибка при замене изображения {}: {}", id, e.getMessage());
            throw e;
        } catch (Exception e) {
            log.error("Непредвиденная ошибка при замене изображения {}", id, e);
            throw new RuntimeException("Внутренняя ошибка сервера при замене изображения", e);
        }
    }

    // 3. Удаление изображения по ID
    @DeleteMapping("/{id}")
    @Operation(summary = "Удаление изображения", description = "Удаляет изображение по ID", responses = {
            @ApiResponse(responseCode = "204", description = "Изображение удалено"),
            @ApiResponse(responseCode = "404", description = "Изображение не найдено", content = @Content(schema = @Schema(implementation = ErrorResponse.class)))
    })
    public ResponseEntity<Void> deleteImage(
            @Parameter(description = "ID изображения") @PathVariable UUID id) {

        log.info("DELETE /api/v1/images/{} - Удаление изображения", id);
        try {
            imageService.deleteImage(id);
            log.info("Изображение {} успешно удалено", id);
            return ResponseEntity.noContent().build();
        } catch (EntityNotFoundException e) {
            log.warn("Попытка удаления несуществующего изображения: {}", id);
            throw e;
        } catch (Exception e) {
            log.error("Непредвиденная ошибка при удалении изображения {}", id, e);
            throw new RuntimeException("Внутренняя ошибка сервера при удалении изображения", e);
        }
    }

    // 4. Получение изображения товара (application/octet-stream)
    @GetMapping(value = "/product/{productId}", produces = MediaType.APPLICATION_OCTET_STREAM_VALUE)
    @Operation(summary = "Получение изображения товара", description = "Возвращает изображение конкретного товара для скачивания", responses = {
            @ApiResponse(responseCode = "200", description = "Изображение найдено", content = @Content(mediaType = "application/octet-stream")),
            @ApiResponse(responseCode = "404", description = "Товар или изображение не найдено", content = @Content(schema = @Schema(implementation = ErrorResponse.class)))
    })
    public ResponseEntity<byte[]> getImageByProductId(
            @Parameter(description = "ID товара", required = true) @PathVariable UUID productId) {

        log.debug("Получение изображения для товара: {}", productId);

        log.debug("GET /api/v1/images/product/{} - Получение изображения товара", productId);

        try {
            byte[] imageData = imageService.getImageDataByProductId(productId);

            HttpHeaders headers = new HttpHeaders();
            headers.setContentType(MediaType.APPLICATION_OCTET_STREAM);
            headers.setContentDisposition(
                    ContentDisposition.attachment()
                            .filename("product_" + productId + ".bin")
                            .build());
            headers.setContentLength(imageData.length);

            log.debug("Изображение для товара {} успешно получено ({} байт)",
                    productId, imageData.length);

            return new ResponseEntity<>(imageData, headers, HttpStatus.OK);

        } catch (EntityNotFoundException e) {
            log.warn("Изображение для товара {} не найдено", productId);
            throw e;
        } catch (Exception e) {
            log.error("Ошибка при получении изображения товара {}", productId, e);
            throw new RuntimeException("Внутренняя ошибка сервера при получении изображения товара", e);
        }
    }

    // 5. Получение изображения по ID (application/octet-stream)
    @GetMapping(value = "/{id}", produces = MediaType.APPLICATION_OCTET_STREAM_VALUE)
    @Operation(summary = "Получение изображения по ID", description = "Возвращает изображение как бинарные данные", responses = {
            @ApiResponse(responseCode = "200", description = "Изображение найдено", content = @Content(mediaType = "application/octet-stream")),
            @ApiResponse(responseCode = "404", description = "Изображение не найдено", content = @Content(schema = @Schema(implementation = ErrorResponse.class)))
    })

    public ResponseEntity<byte[]> getImageById(
            @Parameter(description = "ID изображения", required = true) @PathVariable UUID id) {

        log.debug("GET /api/v1/images/{} - Получение изображения по ID", id);

        try {
            byte[] imageData = imageService.getImageDataById(id);

            HttpHeaders headers = new HttpHeaders();
            headers.setContentType(MediaType.APPLICATION_OCTET_STREAM);
            headers.setContentDisposition(
                    ContentDisposition.attachment()
                            .filename("image_" + id + ".bin")
                            .build());
            headers.setContentLength(imageData.length);

            log.debug("Изображение {} успешно получено ({} байт)", id, imageData.length);

            return new ResponseEntity<>(imageData, headers, HttpStatus.OK);
        } catch (EntityNotFoundException e) {
            log.warn("Изображение {} не найдено", id);
            throw e;
        } catch (Exception e) {
            log.error("Ошибка при получении изображения {}", id, e);
            throw new RuntimeException("Внутренняя ошибка сервера при получении изображения", e);
        }
    }

    // ==================== ВСПОМОГАТЕЛЬНЫЕ МЕТОДЫ ====================
}
