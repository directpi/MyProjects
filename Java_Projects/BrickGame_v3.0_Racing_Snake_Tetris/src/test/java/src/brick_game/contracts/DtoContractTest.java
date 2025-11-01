/**
 * @file: DtoContractTest.java
 * @description: Контрактные тесты совместимости серверных и клиентских DTO по полям
 * @dependencies: org.junit.jupiter.api.*, java.lang.reflect.*
 * @created: 2025-09-30
 */
package src.brick_game.contracts;

import static org.junit.jupiter.api.Assertions.assertTrue;

import java.lang.reflect.Field;
import java.util.HashSet;
import java.util.Set;
import org.junit.jupiter.api.Test;
import src.brick_game.client_lib.dto.GameState;
import src.brick_game.server.api.dto.GameStateDto;

class DtoContractTest {

  @Test
  void gameState_fields_compatibility() {
    Set<String> serverFields = getFieldNames(GameStateDto.class);
    Set<String> clientFields = getFieldNames(GameState.class);

    // Разрешаем различие в названии highScore vs high_score через аннотацию
    // @JsonProperty
    clientFields.add("high_score");
    clientFields.remove("highScore");

    // Проверяем, что все серверные поля покрыты клиентскими
    assertTrue(clientFields.containsAll(serverFields),
        () -> "Client GameState missing fields: " + difference(serverFields, clientFields));
  }

  private Set<String> getFieldNames(Class<?> clazz) {
    Set<String> names = new HashSet<>();
    for (Field f : clazz.getDeclaredFields()) {
      names.add(f.getName());
    }
    return names;
  }

  private Set<String> difference(Set<String> a, Set<String> b) {
    Set<String> diff = new HashSet<>(a);
    diff.removeAll(b);
    return diff;
  }
}

