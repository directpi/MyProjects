package s21.example.utils;

public class Utils {
    /**
     * Копирует двумерный массив (доску).
     *
     * @param board Исходный массив для копирования.
     * @return Новый массив с копией данных.
     * @throws IllegalArgumentException Если исходный массив равен null.
     */
    public int[][] copyBoard(int[][] board) {
        if (board == null) {
            throw new IllegalArgumentException("Исходный массив не может быть null");
        }

        // Создаем новый массив с той же размерностью
        int[][] copy = new int[board.length][];
        for (int i = 0; i < board.length; i++) {
            if (board[i] == null) {
                throw new IllegalArgumentException("Строка " + i + " исходного массива равна null");
            }
            copy[i] = new int[board[i].length];
            System.arraycopy(board[i], 0, copy[i], 0, board[i].length);
        }
        return copy;
    }
}