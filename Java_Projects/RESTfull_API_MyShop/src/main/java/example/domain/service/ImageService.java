package example.domain.service;

import example.datasource.model.ImageDAO;
import example.datasource.repository.ImageRepository;
import example.domain.exception.EntityNotFoundException;
import example.domain.exception.ValidationException;
import example.domain.mapper.ImageMapper;
import example.domain.model.ImageDTO;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import java.util.UUID;

@Slf4j
@Service
public class ImageService {
    private final ImageRepository imageRepository;
    private final ImageMapper imageMapper;
    private final ProductService productService;

    @Autowired
    public ImageService(ImageRepository imageRepository, ImageMapper imageMapper, ProductService productService) {
        this.imageRepository = imageRepository;
        this.imageMapper = imageMapper;
        this.productService = productService;
    }

    // 1. Добавление изображения
    @Transactional
    public ImageDTO createImage(ImageDTO imageDTO) {
        log.debug("Добавление изображения для товара");

        validateImage(imageDTO);

        // Проверяем существование товара
        if (imageDTO.getProductId() != null) {
            productService.getById(imageDTO.getProductId());
        }

        ImageDAO imageDAO = imageMapper.toDAO(imageDTO);
        ImageDAO savedDAO = imageRepository.save(imageDAO);

        // Привязываем изображение к товару (если указан productId)
        if (imageDTO.getProductId() != null) {
            boolean linked = imageRepository.linkImageToProduct(savedDAO.getId(), imageDTO.getProductId());
            if (!linked) {
                throw new EntityNotFoundException("Товар", imageDTO.getProductId());
            }
        }

        ImageDTO created = imageMapper.toDTO(savedDAO);
        created.setProductId(imageDTO.getProductId());

        log.info("Изображение создано с id: {} для товара: {}",
                created.getId(), created.getProductId());
        return created;
    }

    // 2. Полная замена изображения
    @Transactional
    public void replaceImage(UUID imageId, byte[] newImageData) {
        log.debug("Полная замена изображения: {}", imageId);

        validateImageId(imageId);
        validateImageData(newImageData, null);// contentType не известен при замене

        boolean updated = imageRepository.updateImageData(imageId, newImageData);
        if (!updated) {
            throw new EntityNotFoundException("Изображение", imageId);
        }

        log.info("Изображение полностью заменено: {}", imageId);
    }

    // 3. Удаление изображения по ID
    @Transactional
    public void deleteImage(UUID id) {
        log.debug("Удаление изображения: {}", id);

        validateImageId(id);

        getById(id); // Проверяем существование

        boolean deleted = imageRepository.deleteById(id);
        if (!deleted) {
            throw new EntityNotFoundException("Изображение", id);
        }

        log.info("Изображение удалено: {}", id);
    }

    // 4. Получение изображения товара по id товара
    public byte[] getImageDataByProductId(UUID productId) {
        log.debug("Получение изображения для товара: {}", productId);

        validateProductId(productId);

        // Проверяем существование товара
        productService.getById(productId);

        return imageRepository.findByProductId(productId)
                .map(ImageDAO::getImageData)
                .orElseThrow(() -> new EntityNotFoundException("Изображение для товара", productId));
    }

    // 5. Получение изображения по ID
    public byte[] getImageDataById(UUID id) {
        log.debug("Получение изображения по id: {}", id);

        validateImageId(id);

        ImageDTO imageDTO = getById(id);
        return imageDTO.getImageData();
    }

    // 6. Получение полной информации об изображении
    public ImageDTO getById(UUID id) {

        validateImageId(id);

        return imageRepository.findById(id)
                .map(imageMapper::toDTO)
                .orElseThrow(() -> new EntityNotFoundException("Изображение", id));
    }

    // Вспомогательные методы
    private void validateImage(ImageDTO imageDTO) {
        if (imageDTO == null) {
            throw new ValidationException("Данные изображения не могут быть null");
        }
        validateImageData(imageDTO.getImageData(), imageDTO.getContentType());

        // Дополнительная валидация полей DTO
        if (imageDTO.getFileName() != null && imageDTO.getFileName().length() > 255) {
            throw new ValidationException("Имя файла не может превышать 255 символов");
        }

        if (imageDTO.getSize() != null && imageDTO.getSize() > 10 * 1024 * 1024) {
            throw new ValidationException("Размер файла не должен превышать 10MB");
        }
    }

    /**
     * Валидация данных изображения
     * 
     * @param imageData   байты изображения
     * @param contentType MIME-тип (может быть null)
     */
    private void validateImageData(byte[] imageData, String contentType) {
        if (imageData == null || imageData.length == 0) {
            throw new ValidationException("Данные изображения не могут быть пустыми");
        }
        // Проверка размера
        if (imageData.length > 10 * 1024 * 1024) { // 10MB limit
            throw new ValidationException("Размер изображения не должен превышать 10MB");
        }

        // Проверка Content-Type если он предоставлен
        // По требованиям API вход может быть byte array, поэтому допускаем
        // application/octet-stream.
        if (contentType != null
                && !contentType.startsWith("image/")
                && !contentType.equals("application/octet-stream")) {
            throw new ValidationException("Файл должен быть изображением. Получен тип: " + contentType);
        }

        // Проверка сигнатуры файла (если данные достаточного размера)
        if (imageData.length >= 4 && !isValidImageSignature(imageData)) {
            throw new ValidationException("Файл не является валидным изображением (PNG, JPEG, GIF)");
        }
    }

    /**
     * Проверка сигнатуры изображения по первым байтам
     */
    private boolean isValidImageSignature(byte[] data) {
        if (data.length < 4)
            return false;

        // PNG signature: 89 50 4E 47
        if (data[0] == (byte) 0x89 && data[1] == (byte) 0x50 &&
                data[2] == (byte) 0x4E && data[3] == (byte) 0x47) {
            log.debug("Обнаружена PNG сигнатура");
            return true;
        }

        // JPEG signature: FF D8
        if (data[0] == (byte) 0xFF && data[1] == (byte) 0xD8) {
            log.debug("Обнаружена JPEG сигнатура");
            return true;
        }

        // GIF signature"GIF"
        if (data[0] == (byte) 0x47 && data[1] == (byte) 0x49 &&
                data[2] == (byte) 0x46) {
            log.debug("Обнаружена GIF сигнатура");
            return true;
        }

        // BMP signature: "BM"
        if (data[0] == (byte) 0x42 && data[1] == (byte) 0x4D) {
            log.debug("Обнаружена BMP сигнатура");
            return true;
        }

        // WebP signature: "RIFF" + "WEBP"
        if (data.length >= 12 &&
                data[0] == (byte) 0x52 && data[1] == (byte) 0x49 &&
                data[2] == (byte) 0x46 && data[3] == (byte) 0x46 &&
                data[8] == (byte) 0x57 && data[9] == (byte) 0x45 &&
                data[10] == (byte) 0x42 && data[11] == (byte) 0x50) {
            log.debug("Обнаружена WebP сигнатура");
            return true;
        }

        log.warn("Неизвестная сигнатура файла: [{}, {}, {}, {}]",
                String.format("%02X", data[0] & 0xFF),
                String.format("%02X", data[1] & 0xFF),
                String.format("%02X", data[2] & 0xFF),
                String.format("%02X", data[3] & 0xFF));

        return false;
    }

    /**
     * Валидация ID изображения
     */
    private void validateImageId(UUID id) {
        if (id == null) {
            throw new ValidationException("ID изображения не может быть null");
        }
    }

    /**
     * Валидация ID товара
     */
    private void validateProductId(UUID productId) {
        if (productId == null) {
            throw new ValidationException("ID товара не может быть null");
        }
    }

    /**
     * Дополнительный метод для проверки существования изображения
     * (используется в контроллере перед операциями)
     * public boolean existsById(UUID id) {
     * validateImageId(id);
     * return imageRepository.findById(id).isPresent();
     * }
     */

}
