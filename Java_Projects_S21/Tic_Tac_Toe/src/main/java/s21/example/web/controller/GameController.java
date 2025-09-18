package s21.example.web.controller;

import org.springframework.http.ResponseEntity;
import org.springframework.beans.factory.annotation.*;
import s21.example.domain.model.Game;
import s21.example.domain.service.GameService;
import s21.example.web.model.GameDTO;
import s21.example.web.mapper.GameMapper;

import jakarta.servlet.http.HttpServletRequest;
//import org.antlr.v4.runtime.misc.NotNull;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.*;
import jakarta.validation.Valid;
import jakarta.validation.constraints.NotNull;
import s21.example.web.model.MoveRequestDTO;

import java.util.List;
import java.util.Map;
import java.util.Optional;
import java.util.UUID;
import java.util.stream.Collectors;

@RestController
@RequestMapping("/game")
public class GameController {
    private final GameService gameService;
    private static final Logger logger = LoggerFactory.getLogger(GameController.class);
//    private final Utils utils = new Utils();

    @Autowired
    public GameController(GameService gameService) {
        this.gameService = gameService;
    }

    @PostMapping("/start")
    public ResponseEntity<GameDTO> startGame(@RequestHeader("X-Player-Id") UUID userId) {
        Game game = gameService.startNewGame(userId);
        logger.info("Новая игра создана пользователем {}: {}", userId, game.getId());
        return ResponseEntity.status(HttpStatus.CREATED).body(GameMapper.toDTO(game));
    }

    @PostMapping("/move/{id}")
    public ResponseEntity<GameDTO> makeMove(
            @PathVariable UUID id,
            @RequestBody @Valid MoveRequestDTO moveRequest,
            @RequestHeader("X-Player-Id") UUID userId) {

        Game updatedGame = gameService.makeMove(
                id,
                moveRequest.getBoard(),
                moveRequest.getVersion(),
                //GameMapper.toDomain(gameDTO),
                userId
        );
        return ResponseEntity.ok(GameMapper.toDTO(updatedGame));
    }

    @PostMapping("/create")
    public ResponseEntity<GameDTO> createGame(
            @RequestParam @NotNull boolean vsComputer,
            @RequestHeader("X-Player-Id") @Valid UUID playerId) {
        try {
            Game game = gameService.createGame(playerId, vsComputer);
            return ResponseEntity.ok(GameMapper.toDTO(game));
        } catch (IllegalArgumentException | IllegalStateException e) {
            // Создаем DTO с ошибкой
            GameDTO errorDto = new GameDTO();
            errorDto.setErrorMessage(e.getMessage()); // NEW: Добавьте это поле в GameDTO
            return ResponseEntity.badRequest().body(errorDto);
        }
    }

    @PostMapping("/join/{gameId}")
    public ResponseEntity<GameDTO> joinGame(
            @PathVariable UUID gameId,
            @RequestHeader("X-Player-Id") UUID playerId) {

        Game game = gameService.joinGame(gameId, playerId);
        return ResponseEntity.ok(GameMapper.toDTO(game));
    }

    @GetMapping("/{id}")
    public ResponseEntity<GameDTO> getGame(
            @PathVariable UUID id,
            @RequestHeader("X-Player-Id") UUID playerId) {

        Game game = gameService.getGameById(id);
        return ResponseEntity.ok(GameMapper.toDTO(game));
    }

    @GetMapping("/available")
    public ResponseEntity<List<GameDTO>> getAvailableGames(
            @RequestHeader("X-Player-Id") UUID playerId) {
        return ResponseEntity.ok(
                gameService.getAvailableGames().stream()
                        .map(GameMapper::toDTO)
                        .collect(Collectors.toList())
        );
    }
    @GetMapping("/active")
    public ResponseEntity<GameDTO> getActiveGame(@RequestHeader("X-Player-Id") UUID playerId) {
        try {
            Optional<Game> activeGame = gameService.getActiveGameByPlayer(playerId);
            return activeGame.map(game -> ResponseEntity.ok(GameMapper.toDTO(game)))
                    .orElse(ResponseEntity.notFound().build());
        } catch (Exception e) {
            GameDTO errorDto = new GameDTO();
            errorDto.setErrorMessage("Ошибка при поиске активной игры: " + e.getMessage());
            return ResponseEntity.status(HttpStatus.INTERNAL_SERVER_ERROR).body(errorDto);
        }
    }
//    @PostMapping("/logout")
//    public ResponseEntity<String> logout(HttpServletRequest request) {
//        request.getSession().invalidate(); // Инвалидация сессии
//        return ResponseEntity.ok("Выход выполнен успешно");
//    }


    @DeleteMapping("/{gameId}")
    public ResponseEntity<?> deleteGame(
            @PathVariable UUID gameId,
            @RequestHeader("X-Player-Id") UUID playerId) {
        try {
            gameService.deleteGame(gameId, playerId);
            return ResponseEntity.ok().body(Map.of(
                    "message", "Игра успешно удалена",
                    "gameId", gameId.toString()
            ));
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
    @DeleteMapping("/admin/{gameId}")
    public ResponseEntity<?> deleteGameAdmin(
            @PathVariable UUID gameId,
            @RequestHeader("X-Admin-Token") String adminToken) {

        // Простая проверка админского токена
        if (!"admin-secret-token".equals(adminToken)) {
            logger.warn("Неверный админский токен: {}", adminToken);
            return ResponseEntity.status(HttpStatus.FORBIDDEN)
                    .body(Map.of("error", "Доступ запрещен"));
        }

        try {
            gameService.deleteGameAdmin(gameId);
            return ResponseEntity.ok().body(Map.of(
                    "message", "Игра удалена администратором",
                    "gameId", gameId.toString()
            ));
        } catch (RuntimeException e) {
            logger.warn("Игра не найдена для админского удаления: {}", e.getMessage());
            return ResponseEntity.status(HttpStatus.NOT_FOUND)
                    .body(Map.of("error", e.getMessage()));
        }
    }

}