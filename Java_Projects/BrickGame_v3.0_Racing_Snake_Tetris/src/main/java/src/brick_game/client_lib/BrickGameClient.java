/**
 * @file: BrickGameClient.java
 * @description: Универсальный интерфейс клиентской библиотеки для взаимодействия с сервером
 *               BrickGame
 * @dependencies: none
 * @created: 2025-10-05
 */
package src.brick_game.client_lib;

import java.util.concurrent.CompletableFuture;
import src.brick_game.client_lib.dto.GameState;

public interface BrickGameClient {
  String getBaseUrl();

  void setBaseUrl(String baseUrl);

  CompletableFuture<Void> startGame(int gameId);

  CompletableFuture<Void> sendAction(int actionId, boolean hold);

  CompletableFuture<GameState> getState();

  // Удобные методы согласно спецификации библиотеки
  default CompletableFuture<Void> userInput(Action action, boolean hold) {
    return sendAction(action.ordinal(), hold);
  }

  default CompletableFuture<GameState> updateCurrentState() {
    return getState();
  }
}
