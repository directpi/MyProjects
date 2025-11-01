/**
 * @file: HighScoreRepository.java
 * @description: Простое файловое хранилище рекорда (high score)
 * @dependencies: com.fasterxml.jackson.databind.ObjectMapper
 * @created: 2025-10-05
 */
package src.brick_game.server.repository;

import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.node.ObjectNode;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardOpenOption;
import org.springframework.stereotype.Repository;

@Repository
public class HighScoreRepository {
  private static final String DEFAULT_PATH = "data/highscore.json";
  private final ObjectMapper objectMapper = new ObjectMapper();
  private final Path storagePath;

  public HighScoreRepository() {
    this.storagePath = Path.of(DEFAULT_PATH);
  }

  public synchronized int getHighScore() {
    if (!Files.exists(storagePath)) {
      return 0;
    }
    try {
      byte[] bytes = Files.readAllBytes(storagePath);
      ObjectNode node = (ObjectNode) objectMapper.readTree(bytes);
      return node.path("high_score").asInt(0);
    } catch (IOException e) {
      return 0;
    }
  }

  public synchronized void saveHighScore(int highScore) {
    try {
      if (storagePath.getParent() != null) {
        Files.createDirectories(storagePath.getParent());
      }
      ObjectNode node = objectMapper.createObjectNode();
      node.put("high_score", highScore);
      byte[] bytes = objectMapper.writerWithDefaultPrettyPrinter().writeValueAsBytes(node);
      Files.write(storagePath, bytes, StandardOpenOption.CREATE,
          StandardOpenOption.TRUNCATE_EXISTING, StandardOpenOption.WRITE);
    } catch (IOException ignored) {
      // swallow IO errors to not break gameplay
    }
  }
}

