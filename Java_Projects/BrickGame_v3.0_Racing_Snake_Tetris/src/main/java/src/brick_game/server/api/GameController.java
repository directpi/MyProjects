/**
 * @file: GameController.java
 * @description: REST контроллер для операций с играми: список и запуск игры
 * @dependencies: src.brick_game.server.api.dto.*, org.springframework.web.bind.annotation.*
 * @created: 2025-10-05
 */
package src.brick_game.server.api;

import java.util.List;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;
import src.brick_game.server.api.dto.GameInfoDto;
import src.brick_game.server.api.dto.GamesListDto;
import src.brick_game.server.service.GameService;

@RestController
@RequestMapping("/api")
public class GameController {
  private final GameService gameService;

  @Autowired
  public GameController(GameService gameService) {
    this.gameService = gameService;
  }

  @GetMapping("/games")
  public ResponseEntity<GamesListDto> getGames() {
    List<GameInfoDto> games = gameService.getGames();
    return ResponseEntity.ok(new GamesListDto(games));
  }

  @PostMapping("/games/{gameId}")
  public ResponseEntity<Void> startGame(@PathVariable("gameId") int gameId) {
    gameService.startGame(gameId);
    return ResponseEntity.ok().build();
  }
}
