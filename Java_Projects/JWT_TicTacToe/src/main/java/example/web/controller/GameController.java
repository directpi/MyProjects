package example.web.controller;

import example.domain.model.Game;
import example.domain.model.PlayerStats;
import example.domain.service.GameService;
import example.utils.AuthUtils;
import example.web.mapper.GameMapper;
import example.web.model.GameDTO;
import example.web.model.MoveRequestDTO;
import example.web.model.PlayerStatsDTO;
import jakarta.validation.Valid;
import jakarta.validation.constraints.NotNull;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.*;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.List;
import java.util.Map;
import java.util.Optional;
import java.util.UUID;
import java.util.stream.Collectors;

@RestController
@RequestMapping("/game")
public class GameController {
    private final GameService gameService;
    private final AuthUtils authUtils;
    private static final Logger logger = LoggerFactory.getLogger(GameController.class);
    // private final Utils utils = new Utils();

    @Autowired
    public GameController(GameService gameService, AuthUtils authUtils) {
        this.gameService = gameService;
        this.authUtils = authUtils;
    }

    @PostMapping("/start")
    public ResponseEntity<GameDTO> startGame() {
        // Получаем ID авторизованного пользователя из SecurityContext
        UUID userId = authUtils.getCurrentPlayerId();

        Game game = gameService.startNewGame(userId);
        logger.info("Новая игра создана пользователем {}: {}", userId, game.getId());
        return ResponseEntity.status(HttpStatus.CREATED).body(GameMapper.toDTO(game));
    }

    @PostMapping("/move/{id}")
    public ResponseEntity<GameDTO> makeMove(
            @PathVariable UUID id,
            @RequestBody @Valid MoveRequestDTO moveRequest) {

        // Получаем ID авторизованного пользователя из SecurityContext
        UUID userId = authUtils.getCurrentPlayerId();
        Game updatedGame = gameService.makeMove(
                id,
                moveRequest.getBoard(),
                moveRequest.getVersion(),
                // GameMapper.toDomain(gameDTO),
                userId);
        return ResponseEntity.ok(GameMapper.toDTO(updatedGame));
    }

    @PostMapping("/create") // без playerId
    public ResponseEntity<GameDTO> createGame(
            @RequestParam @NotNull boolean vsComputer) {
        try {
            // Получаем ID авторизованного пользователя из SecurityContext
            UUID playerId = authUtils.getCurrentPlayerId();

            Game game = gameService.createGame(playerId, vsComputer);
            return ResponseEntity.ok(GameMapper.toDTO(game));
        } catch (IllegalArgumentException | IllegalStateException e) {
            // Обработка бизнес-ошибок (невалидные параметры, конфликты)
            GameDTO errorDto = new GameDTO();
            errorDto.setErrorMessage(e.getMessage());
            return ResponseEntity.badRequest().body(errorDto);
        } catch (Exception e) {
            // Обработка unexpected ошибок (для безопасности)
            GameDTO errorDto = new GameDTO();
            errorDto.setErrorMessage("Внутренняя ошибка сервера");
            return ResponseEntity.status(HttpStatus.INTERNAL_SERVER_ERROR).body(errorDto);
        }
    }

    @PostMapping("/join/{gameId}")
    public ResponseEntity<GameDTO> joinGame(
            @PathVariable UUID gameId) {
        try {
            // Получаем ID авторизованного пользователя из SecurityContext
            UUID playerId = authUtils.getCurrentPlayerId();

            Game game = gameService.joinGame(gameId, playerId);
            return ResponseEntity.ok(GameMapper.toDTO(game));
        } catch (Exception e) {
            return ResponseEntity.internalServerError().build();
        }
    }

    @GetMapping("/{id}")
    public ResponseEntity<GameDTO> getGame(
            @PathVariable UUID id) {
        try {
            Game game = gameService.getGameById(id);
            return ResponseEntity.ok(GameMapper.toDTO(game));
        } catch (Exception e) {
            return ResponseEntity.internalServerError().build();
        }
    }

    @GetMapping("/available")
    public ResponseEntity<?> getAvailableGames() {
        try {
            List<GameDTO> games = gameService.getAvailableGames().stream()
                    .map(GameMapper::toDTO)
                    .collect(Collectors.toList());
            return ResponseEntity.ok(games);
        } catch (IllegalStateException e) {
            return ResponseEntity
                    .status(HttpStatus.CONFLICT) // 409
                    .body(e.getMessage());
        }
    }

    @GetMapping("/active")
    public ResponseEntity<GameDTO> getActiveGame() {
        try {
            // Получаем ID авторизованного пользователя из SecurityContext
            UUID playerId = authUtils.getCurrentPlayerId();

            Optional<Game> activeGame = gameService.getActiveGameByPlayer(playerId);
            return activeGame.map(game -> ResponseEntity.ok(GameMapper.toDTO(game)))
                    .orElse(ResponseEntity.notFound().build());
        } catch (Exception e) {
            GameDTO errorDto = new GameDTO();
            errorDto.setErrorMessage("Ошибка при поиске активной игры: " + e.getMessage());
            return ResponseEntity.status(HttpStatus.INTERNAL_SERVER_ERROR).body(errorDto);
        }
    }

    @DeleteMapping("/{gameId}")
    public ResponseEntity<?> deleteGame(
            @PathVariable UUID gameId) {
        try {
            // Получаем ID авторизованного пользователя из SecurityContext
            UUID playerId = authUtils.getCurrentPlayerId();

            gameService.deleteGame(gameId, playerId);
            return ResponseEntity.ok().body(Map.of(
                    "message", "Игра успешно удалена",
                    "gameId", gameId.toString()));
        } catch (SecurityException e) {
            logger.warn("Попытка удаления чужой игры: {}", e.getMessage());
            return ResponseEntity.status(HttpStatus.FORBIDDEN)
                    .body(Map.of("error", e.getMessage()));
        } catch (IllegalStateException e) {
            logger.warn("Нельзя удалить незавершенную игру: {}", e.getMessage());
            return ResponseEntity.status(HttpStatus.CONFLICT)
                    .body(Map.of("error", e.getMessage()));
        } catch (RuntimeException e) {
            logger.warn("Игра не найдена: {}", e.getMessage());
            return ResponseEntity.status(HttpStatus.NOT_FOUND)
                    .body(Map.of("error", e.getMessage()));
        }
    }


    @GetMapping("/history")
    public ResponseEntity<List<GameDTO>> getGameHistory() {
        try {
            UUID playerId = authUtils.getCurrentPlayerId(); // Из JWT токена
            List<Game> completedGames = gameService.getCompletedGamesByPlayerId(playerId);
            List<GameDTO> gameDTOs = completedGames.stream()
                    .map(GameMapper::toDTO)
                    .collect(Collectors.toList());
            return ResponseEntity.ok(gameDTOs);
        } catch (Exception e) {
            return ResponseEntity.internalServerError().build();
        }
    }

    /**
     * Получение таблицы лидеров (топ N игроков по соотношению побед)
     * 
     * @param limit количество лучших игроков (по умолчанию 10)
     * @return список лучших игроков с их статистикой
     */
    @GetMapping("/leaderboard")
    public ResponseEntity<List<PlayerStatsDTO>> getLeaderboard(
            @RequestParam(defaultValue = "10") int limit) {

        try {
            // Валидация параметра
            if (limit <= 0 || limit > 100) {
                return ResponseEntity.badRequest().build();
            }

            List<PlayerStats> topPlayers = gameService.getTopPlayersByWinRatio(limit);
            List<PlayerStatsDTO> leaderboard = topPlayers.stream()
                    .map(this::convertToDTO)
                    .collect(Collectors.toList());

            return ResponseEntity.ok(leaderboard);

        } catch (IllegalArgumentException e) {
            logger.warn("Неверные параметры для получения таблицы лидеров: {}", e.getMessage());
            return ResponseEntity.badRequest().build();
        } catch (Exception e) {
            logger.error("Ошибка при получении таблицы лидеров: {}", e.getMessage(), e);
            return ResponseEntity.status(HttpStatus.INTERNAL_SERVER_ERROR).build();
        }
    }

    /**
     * Преобразование PlayerStats в PlayerStatsDTO
     */
    private PlayerStatsDTO convertToDTO(PlayerStats stats) {
        return new PlayerStatsDTO(
                stats.getPlayerId(),
                stats.getLogin(),
                stats.getWins(),
                stats.getLosses(),
                stats.getDraws(),
                stats.getWinRatio());
    }

}