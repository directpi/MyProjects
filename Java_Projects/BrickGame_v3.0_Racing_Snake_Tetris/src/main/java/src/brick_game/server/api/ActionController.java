/**
 * @file: ActionController.java
 * @description: REST контроллер для выполнения действий игрока
 * @dependencies: src.brick_game.server.api.dto.UserActionDto,
 *                org.springframework.web.bind.annotation.*
 * @created: 2025-10-05
 */
package src.brick_game.server.api;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;
import src.brick_game.server.api.dto.UserActionDto;
import src.brick_game.server.service.GameService;

@RestController
@RequestMapping("/api")
public class ActionController {
  private final GameService gameService;

  @Autowired
  public ActionController(GameService gameService) {
    this.gameService = gameService;
  }

  @PostMapping("/actions")
  public ResponseEntity<Void> performAction(@RequestBody UserActionDto action) {
    gameService.performAction(action.getAction_id(), action.isHold());
    return ResponseEntity.ok().build();
  }
}
