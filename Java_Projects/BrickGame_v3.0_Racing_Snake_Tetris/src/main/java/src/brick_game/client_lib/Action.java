/**
 * @file: Action.java
 * @description: Перечисление действий пользователя согласно спецификации библиотеки BrickGame
 * @dependencies: none
 * @created: 2025-10-05
 */
package src.brick_game.client_lib;

/**
 * Порядок элементов соответствует спецификации и используется для вычисления action_id (ordinal).
 */
public enum Action {
  Start, Pause, Terminate, Left, Right, Up, Down, Action;

  /**
   * Получить ID действия для совместимости с существующим кодом
   * 
   * @return порядковый номер + 1 (1-8)
   */
  public int getId() {
    return this.ordinal() + 1;
  }
}
