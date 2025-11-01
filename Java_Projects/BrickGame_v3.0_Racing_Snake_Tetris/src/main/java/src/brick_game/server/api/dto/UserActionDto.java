/**
 * @file: UserActionDto.java
 * @description: DTO команды игрока
 * @dependencies: none
 * @created: 2025-10-05
 */
package src.brick_game.server.api.dto;

public class UserActionDto {
  private int action_id;
  private boolean hold;

  public UserActionDto() {}

  public UserActionDto(int actionId, boolean hold) {
    this.action_id = actionId;
    this.hold = hold;
  }

  public int getAction_id() {
    return action_id;
  }

  public void setAction_id(int action_id) {
    this.action_id = action_id;
  }

  public boolean isHold() {
    return hold;
  }

  public void setHold(boolean hold) {
    this.hold = hold;
  }
}

