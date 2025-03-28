package s21.example.datasource.repository;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Repository; // Импортируем аннотацию
import s21.example.datasource.mapper.GameMapper;
import s21.example.datasource.model.GameEntity;
import s21.example.domain.model.Game;

import java.util.Map;
import java.util.Optional;
import java.util.UUID;
import java.util.concurrent.ConcurrentHashMap;

@Repository
public class ArchivedGameRepository {
    // Хранилище завершённых игр
    private final Map<UUID, GameEntity> archivedGames = new ConcurrentHashMap<>();
    private static final Logger logger = LoggerFactory.getLogger(ArchivedGameRepository.class);

    /**
     * Архивирует игру.
     *
     * @param game Игра для архивирования.
     */
    public void archive(Game game) {
        if (game == null) {
            logger.error("Попытка архивировать null игру");
            throw new IllegalArgumentException("Игра не может быть null");
        }
        GameEntity entity = GameMapper.toEntity(game); // Преобразуем Game в GameEntity

        archivedGames.put(entity.getId(), entity);
        //logger.info("Игра заархивирована: {}", game.getId());
    }

    /**
     * Находит архивированную игру по её ID.
     *
     * @param id ID игры.
     * @return Optional с игрой, если она найдена, иначе пустой Optional.
     */
    public Optional<Game> findArchivedById(UUID id) {
        if (id == null) {
            logger.error("Попытка найти архивированную игру с null ID");
            throw new IllegalArgumentException("ID игры не может быть null");
        }
        GameEntity entity = archivedGames.get(id);
        if (entity != null) {
            //logger.info("Архивированная игра найдена: {}", entity.getId());
            return Optional.of(GameMapper.toDomain(entity)); // Преобразуем GameEntity в Game
        } else {
            logger.warn("Архивированная игра с ID {} не найдена", id);
        }
        return Optional.empty();
    }

    /**
     * Проверяет, существует ли архивированная игра с указанным ID.
     *
     * @param id ID игры.
     * @return true, если игра существует, иначе false.
     */
    public boolean existsById(UUID id) {
        if (id == null) {
            logger.error("Попытка проверить существование архивированной игры с null ID");
            throw new IllegalArgumentException("ID игры не может быть null");
        }
        boolean exists = archivedGames.containsKey(id);
        logger.info("Проверка существования архивированной игры с ID {}: {}", id, exists);

        return exists;
    }

    /**
     * Возвращает количество архивированных игр.
     *
     * @return Количество архивированных игр.
     */
    public int count() {
        int count = archivedGames.size();
        logger.info("Количество архивированных игр: {}", count);
        return archivedGames.size();
    }
}