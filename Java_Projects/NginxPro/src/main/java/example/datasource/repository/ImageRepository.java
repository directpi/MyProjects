package example.datasource.repository;
import example.datasource.model.ImageDAO;
import java.util.Optional;
import java.util.UUID;

public interface ImageRepository{

    // CRUD операции
    ImageDAO save(ImageDAO imageDAO);
    Optional<ImageDAO> findById(UUID id);
    boolean deleteById(UUID id);
    boolean updateImageData(UUID id, byte[] newImageData);

    // Специальные методы
    Optional<ImageDAO> findByProductId(UUID productId);
    boolean linkImageToProduct(UUID imageId, UUID productId);
}