package s21.example.datasource.repository;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Repository; // Импортируем аннотацию
import s21.example.datasource.mapper.GameMapper;
import s21.example.datasource.model.GameEntity;
import s21.example.domain.model.Game;
import s21.example.domain.model.GameBoard;
import s21.example.utils.Utils;

import java.util.Map;
import java.util.Optional;
import java.util.UUID;
import java.util.concurrent.ConcurrentHashMap;

@Repository // Добавляем аннотацию
public class GameRepository {
    private final Map<UUID, GameEntity> games = new ConcurrentHashMap<>();
    private static final Logger logger = LoggerFactory.getLogger(GameRepository.class);
    private final Utils utils = new Utils();

    /**
     * Сохраняет игру в репозитории.
     *
     * @param game Игра для сохранения.
     */
    public void save(Game game) {
        if (game == null) {
            logger.error("Попытка сохранить null игру"); // Логирование ошибки
            throw new IllegalArgumentException("Игра не может быть null");
        }

        // Сохраняем текущее состояние в previousBoard
        int [][] currentBoard = game.getBoard().getBoard();
        game.setPreviousBoard(utils.copyBoard(currentBoard));

        GameEntity entity = GameMapper.toEntity(game); // Преобразуем Game в GameEntity

        games.put(entity.getId(), entity);
        //logger.info("Игра сохранена: {}", game.getId()); // Логирование
    }

    /**
     * Находит игру по её ID.
     *
     * @param id ID игры.
     * @return Optional с игрой, если она найдена, иначе пустой Optional.
     */
    public Optional<Game> findById(UUID id) {
        if (id == null) {
            logger.error("Попытка найти игру с null ID"); // Логирование ошибки
            throw new IllegalArgumentException("ID игры не может быть null");
        }
        GameEntity entity = games.get(id);
        if (entity != null) {
            //logger.info("Игра найдена: {}", entity.getId()); // Логирование успеха
            return Optional.of(GameMapper.toDomain(entity)); // Преобразуем GameEntity в Game
        } else {
            logger.warn("Игра с ID {} не найдена", id); // Логирование предупреждения
        }
        return Optional.empty();
    }

    /**
     * Удаляет игру по её ID.
     *
     * @param id ID игры для удаления.
     */
    public void delete(UUID id) {
        if (id == null) {
            logger.error("Попытка удалить игру с null ID"); // Логирование ошибки
            throw new IllegalArgumentException("ID игры не может быть null");
        }
        games.remove(id);
        //logger.info("Игра удалена: {}", id); // Логирование
    }
}
