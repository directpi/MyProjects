package src.brick_game.server.api;

import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.ControllerAdvice;
import org.springframework.web.bind.annotation.ExceptionHandler;
import java.util.Map;

@ControllerAdvice
public class RestExceptionHandler {
  @ExceptionHandler(IllegalArgumentException.class)
  public ResponseEntity<Map<String, String>> handleIllegalArgument(IllegalArgumentException ex) {
    String msg = ex.getMessage();
    if ("GAME_NOT_FOUND".equals(msg)) {
      return ResponseEntity.status(HttpStatus.NOT_FOUND).body(Map.of("message", "Game not found"));
    }
    return ResponseEntity.status(HttpStatus.BAD_REQUEST).body(Map.of("message", msg));
  }

  @ExceptionHandler(IllegalStateException.class)
  public ResponseEntity<Map<String, String>> handleIllegalState(IllegalStateException ex) {
    String msg = ex.getMessage();
    if ("CONFLICT_GAME_RUNNING".equals(msg)) {
      return ResponseEntity.status(HttpStatus.CONFLICT)
          .body(Map.of("message", "Another game is already running"));
    }
    if ("NO_GAME_RUNNING".equals(msg)) {
      return ResponseEntity.status(HttpStatus.BAD_REQUEST)
          .body(Map.of("message", "No game started"));
    }
    return ResponseEntity.status(HttpStatus.INTERNAL_SERVER_ERROR).body(Map.of("message", msg));
  }

  @ExceptionHandler(Exception.class)
  public ResponseEntity<Map<String, String>> handleGeneric(Exception ex) {
    return ResponseEntity.status(HttpStatus.INTERNAL_SERVER_ERROR)
        .body(Map.of("message", "Internal server error"));
  }
}
