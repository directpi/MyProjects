/**
 * @file: RestBrickGameClient.java
 * @description: REST-реализация клиентской библиотеки BrickGame
 * @dependencies: java.net.http.HttpClient
 * @created: 2025-10-05
 */
package src.brick_game.client_lib;

import com.fasterxml.jackson.databind.ObjectMapper;
import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
import java.nio.charset.StandardCharsets;
import java.time.Duration;
import java.util.concurrent.CompletableFuture;
import src.brick_game.client_lib.dto.GameState;

public class RestBrickGameClient implements BrickGameClient {
  private final HttpClient httpClient;
  private final ObjectMapper objectMapper = new ObjectMapper();
  private String baseUrl;

  public RestBrickGameClient(String baseUrl) {
    this.baseUrl = baseUrl;
    this.httpClient = HttpClient.newBuilder().connectTimeout(Duration.ofSeconds(5))
        .version(HttpClient.Version.HTTP_2).build();
  }

  @Override
  public String getBaseUrl() {
    return baseUrl;
  }

  @Override
  public void setBaseUrl(String baseUrl) {
    this.baseUrl = baseUrl;
  }

  @Override
  public CompletableFuture<Void> startGame(int gameId) {
    HttpRequest request = HttpRequest.newBuilder().uri(URI.create(baseUrl + "/games/" + gameId))
        .timeout(Duration.ofSeconds(5)).POST(HttpRequest.BodyPublishers.noBody()).build();
    return httpClient.sendAsync(request, HttpResponse.BodyHandlers.discarding())
        .thenApply(r -> null);
  }

  @Override
  public CompletableFuture<Void> sendAction(int actionId, boolean hold) {
    String json;
    try {
      json = objectMapper.writeValueAsString(new UserAction(actionId, hold));
    } catch (Exception e) {
      CompletableFuture<Void> f = new CompletableFuture<>();
      f.completeExceptionally(e);
      return f;
    }
    HttpRequest request = HttpRequest.newBuilder().uri(URI.create(baseUrl + "/actions"))
        .timeout(Duration.ofSeconds(5)).header("Content-Type", "application/json")
        .POST(HttpRequest.BodyPublishers.ofString(json, StandardCharsets.UTF_8)).build();
    return httpClient.sendAsync(request, HttpResponse.BodyHandlers.discarding())
        .thenApply(r -> null);
  }

  // Удобные методы согласно спецификации библиотеки
  public CompletableFuture<Void> userInput(Action action, boolean hold) {
    return sendAction(action.ordinal(), hold);
  }

  public CompletableFuture<GameState> updateCurrentState() {
    return getState();
  }

  @Override
  public CompletableFuture<GameState> getState() {
    HttpRequest request = HttpRequest.newBuilder().uri(URI.create(baseUrl + "/state"))
        .timeout(Duration.ofSeconds(5)).GET().build();
    return httpClient.sendAsync(request, HttpResponse.BodyHandlers.ofString(StandardCharsets.UTF_8))
        .thenApply(HttpResponse::body).thenApply(this::readState);
  }

  private GameState readState(String json) {
    try {
      if (json == null || json.trim().isEmpty()) {
        throw new RuntimeException("Empty response from server");
      }
      return objectMapper.readValue(json, GameState.class);
    } catch (Exception e) {
      throw new RuntimeException("Failed to parse GameState: " + json, e);
    }
  }

  private static final class UserAction {
    public int action_id;
    public boolean hold;

    public UserAction(int actionId, boolean hold) {
      this.action_id = actionId;
      this.hold = hold;
    }
  }
}
