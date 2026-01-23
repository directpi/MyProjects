package example.datasource.repository.impl;

import example.datasource.model.ImageDAO;
import example.datasource.repository.ImageRepository;
import example.domain.exception.EntityNotFoundException;
import lombok.extern.slf4j.Slf4j;
import org.springframework.dao.EmptyResultDataAccessException;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Repository;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.List;
import java.util.Optional;
import java.util.UUID;

@Slf4j
@Repository
public class JdbcImageRepository implements ImageRepository{
    private final JdbcTemplate jdbcTemplate;

    public JdbcImageRepository(JdbcTemplate jdbcTemplate) {
        this.jdbcTemplate = jdbcTemplate;
    }

    @Override
    public ImageDAO save(ImageDAO imageDAO) {
        if (imageDAO.getId() == null) {
            return insert(imageDAO);
        } else {
            return update(imageDAO);
        }
    }

    private ImageDAO insert(ImageDAO imageDAO) {
        String sql = "INSERT INTO images (image_data) VALUES (?) RETURNING id";
        UUID newId = jdbcTemplate.queryForObject(sql, UUID.class,imageDAO.getImageData());
        imageDAO.setId(newId);
        return imageDAO;
    }

    private ImageDAO update(ImageDAO imageDAO) {
    String sql = "UPDATE images SET image_data = ? WHERE id = ?";
    int rowUpdates = jdbcTemplate.update(sql, imageDAO.getImageData(), imageDAO.getId());

    if (rowUpdates == 0) {
        throw new EntityNotFoundException("Изображение", imageDAO.getId());
    } else {
        return imageDAO;
    }
    }

    @Override
    public boolean deleteById(UUID id) {

        log.debug("Удаление изображения с ID: {}", id);

        // 1. Отвязываем все товары от этого изображения
        String unlinkSql = "UPDATE products SET image_id = NULL WHERE image_id = ?";
        int productsUnlinked = jdbcTemplate.update(unlinkSql, id);
        log.debug("Отвязано товаров: {} от изображения: {}", productsUnlinked, id);

        // 2. Удаляем само изображение
        String deleteSql = "DELETE FROM images WHERE id = ?";
        int rowsDeleted = jdbcTemplate.update(deleteSql, id);

        boolean deleted = rowsDeleted > 0;
        if (deleted) {
            log.info("Изображение {} успешно удалено (отвязано от {} товаров)", id, productsUnlinked);
        } else {
            log.warn("Изображение {} не найдено для удаления", id);
        }

        return deleted;
    }

    @Override
    public Optional<ImageDAO> findById(UUID id) {
       String sql = "SELECT * FROM images WHERE id = ?";
       try {
           ImageDAO imageDAO = jdbcTemplate.queryForObject(sql, this::mapRow, id);
        return Optional.ofNullable((imageDAO));
       } catch (EmptyResultDataAccessException e){
           return Optional.empty();
        }
    }


    @Override
    public Optional<ImageDAO> findByProductId(UUID productId) {
        String sql = "SELECT * FROM images i JOIN products p ON i.id = p.image_id WHERE p.id = ?";
        try{
            ImageDAO imageDAO = jdbcTemplate.queryForObject(sql,this::mapRow, productId);
            return Optional.ofNullable(imageDAO);
        } catch (EmptyResultDataAccessException e) {
            return Optional.empty();
        }
    }

    @Override
    public boolean linkImageToProduct(UUID imageId, UUID productId) {
        log.debug("Привязка изображения {} к товару {}", imageId, productId);
        // Сначала отвязываем текущее изображение товара (если есть)
        String unlinkSql = "UPDATE products SET image_id = NULL WHERE id = ? AND image_id IS NOT NULL";
        int unlinked = jdbcTemplate.update(unlinkSql, productId);
        if (unlinked > 0) {
            log.debug("Отвязано предыдущее изображение от товара: {}", productId);
        }
        // Теперь привязываем новое изображение
        String linkSql  = "UPDATE products SET image_id = ? WHERE id = ?";
        int rowsUpdated = jdbcTemplate.update(linkSql , imageId, productId);

        boolean linked = rowsUpdated > 0;
        if (linked) {
            log.info("Изображение {} привязано к товару {}", imageId, productId);
        } else {
            log.warn("Товар {} не найден при попытке привязки изображения {}", productId, imageId);
        }

        return linked;
    }

    @Override
    public boolean updateImageData(UUID id, byte[] newImageData) {
        String sql = "UPDATE images SET image_data = ? WHERE id =?";
        int rowsUpdated = jdbcTemplate.update(sql, newImageData, id);
        return rowsUpdated > 0;
    }

    private ImageDAO mapRow(ResultSet rs, int rowNum) throws SQLException{
        ImageDAO imageDAO = new ImageDAO();
                imageDAO.setId(rs.getObject("id", UUID.class));
        imageDAO.setImageData(rs.getBytes("image_data"));
        return imageDAO;
    }
}
