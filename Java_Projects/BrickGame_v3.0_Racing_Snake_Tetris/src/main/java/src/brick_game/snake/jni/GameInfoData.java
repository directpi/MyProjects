/**
 * @file: GameInfoData.java
 * @description: Java representation of C++ GameInfo_t structure for Snake
 * @dependencies: snake JNI
 * @created: 2025-10-06
 */
package src.brick_game.snake.jni;

public class GameInfoData {
    public int[][] field; // 20x10
    public int[][] next; // 4x4 (не используется в Snake, но нужно для совместимости)
    public int score;
    public int highScore;
    public int level;
    public int speed;
    public int pause; // 0=игра, 1=пауза, 5=game over, 7=exit, 8=win

    public GameInfoData() {
        this.field = new int[20][10];
        this.next = new int[4][4];
        this.score = 0;
        this.highScore = 0;
        this.level = 1;
        this.speed = 500;
        this.pause = 0;
    }
}
