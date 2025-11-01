/**
 * @file: GameInfoDto.java
 * @description: DTO с информацией об игре
 * @dependencies: lombok.Data, lombok.AllArgsConstructor, lombok.NoArgsConstructor
 * @created: 2025-10-05
 */
package src.brick_game.server.api.dto;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@NoArgsConstructor
@AllArgsConstructor
public class GameInfoDto {
  private int id;
  private String name;
}
