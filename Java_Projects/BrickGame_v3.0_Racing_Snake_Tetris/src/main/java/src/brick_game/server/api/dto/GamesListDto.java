/**
 * @file: GamesListDto.java
 * @description: DTO со списком доступных игр
 * @dependencies: src.brick_game.server.api.dto.GameInfoDto
 * @created: 2025-10-05
 */
package src.brick_game.server.api.dto;

import java.util.List;

public class GamesListDto {
  private List<GameInfoDto> games;

  public GamesListDto() {}

  public GamesListDto(List<GameInfoDto> games) {
    this.games = games;
  }

  public List<GameInfoDto> getGames() {
    return games;
  }

  public void setGames(List<GameInfoDto> games) {
    this.games = games;
  }
}

