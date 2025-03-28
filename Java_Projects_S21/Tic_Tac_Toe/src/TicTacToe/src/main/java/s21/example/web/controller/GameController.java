package s21.example.web.controller;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;
import s21.example.domain.model.Game;
import s21.example.domain.service.GameService;
import s21.example.datasource.repository.GameRepository;
import s21.example.utils.Utils;
import s21.example.web.model.GameDTO;
import s21.example.web.mapper.GameMapper;

import java.util.UUID;

@RestController
@RequestMapping("/game")
public class GameController {
    private final GameService gameService;
    private final GameRepository gameRepository;
    private static final Logger logger = LoggerFactory.getLogger(GameController.class);
    private final Utils utils = new Utils();

    @Autowired
    public GameController(GameService gameService, GameRepository gameRepository) {
        this.gameService = gameService;
        this.gameRepository = gameRepository;
    }

    @PostMapping("/start")
    public GameDTO startGame() {

        Game game = new Game();
        game.setPreviousBoard(utils.copyBoard(game.getBoard().getBoard())); // Обновляем previousBoard

        gameRepository.save(game);
        logger.info("Новая игра создана и сохранена: {}", game.getId()); // Логирование
        return GameMapper.toDTO(game);
    }

    @PostMapping("/move/{id}")
    public GameDTO makeMove(@PathVariable String id, @RequestBody GameDTO gameDTO) {

        UUID gameId = UUID.fromString(id);
        logger.info("Получен запрос на ход для игры с ID: {}", gameId);

        Game game = gameRepository.findById(gameId)
                .orElseThrow(() -> {
                    logger.error("Игра с ID {} не найдена", gameId);
                    return new RuntimeException("Игра с ID " + gameId + " не найдена");
                });

        Game updatedGameData = GameMapper.toDomain(gameDTO);

        game.setPreviousBoard(utils.copyBoard(game.getBoard().getBoard()));

        game.getBoard().setBoard(updatedGameData.getBoard().getBoard());


        if (!gameService.validateBoard(game)) {
            logger.error("Введены некорректные данные для игры с ID: {}", gameId);
            throw new IllegalArgumentException("Введены некорректные данные\n");
        }

        gameRepository.save(game);

        // Получаем следующий ход от компьютера
        Game updatedGame = gameService.getNextMove(game);

        gameRepository.save(updatedGame);
        //logger.info("Игра с ID {} обновлена: {}", gameId, updatedGame);

        return GameMapper.toDTO(updatedGame);
    }
}