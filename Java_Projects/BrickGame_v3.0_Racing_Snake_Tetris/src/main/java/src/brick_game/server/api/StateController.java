/**
 * @file: StateController.java
 * @description: REST контроллер для получения текущего состояния игры
 * @dependencies: src.brick_game.server.api.dto.GameStateDto,
 *                org.springframework.web.bind.annotation.*
 * @created: 2025-10-05
 */
package src.brick_game.server.api;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;
import src.brick_game.server.api.dto.GameStateDto;
import src.brick_game.server.service.GameService;

@RestController
@RequestMapping("/api")
public class StateController {
  private final GameService gameService;

  @Autowired
  public StateController(GameService gameService) {
    this.gameService = gameService;
  }

  @GetMapping("/state")
  public ResponseEntity<GameStateDto> getState() {
    return ResponseEntity.ok(gameService.getState());
  }
}
