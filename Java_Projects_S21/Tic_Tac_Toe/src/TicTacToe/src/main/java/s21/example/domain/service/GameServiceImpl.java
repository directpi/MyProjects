package s21.example.domain.service;

import s21.example.datasource.repository.GameRepository;
import s21.example.datasource.repository.ArchivedGameRepository;
import s21.example.domain.model.Game;
import s21.example.domain.model.GameBoard;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import s21.example.utils.Utils;

public class GameServiceImpl implements GameService {
    private final GameRepository gameRepository;
    private final ArchivedGameRepository archivedGameRepository; // Новое поле
    private final Utils utils = new Utils();

    private static final Logger logger = LoggerFactory.getLogger(GameServiceImpl.class);

    public GameServiceImpl(GameRepository gameRepository, ArchivedGameRepository archivedGameRepository) {
        this.gameRepository = gameRepository;
        this.archivedGameRepository = archivedGameRepository; // Инициализация

    }

    @Override
    public Game getNextMove(Game game) {
        // Проверяем, что ID игры не равен null
        if (game.getId() == null) {
            logger.error("ID игры не может быть null");
            throw new IllegalArgumentException("ID игры не может быть null");
        }

        Game currentGame = gameRepository.findById(game.getId())
                .orElseThrow(() -> {
                    logger.error("Игра с ID {} не найдена", game.getId());
                    return new RuntimeException("Игра с ID " + game.getId() + " не найдена");
                });

        GameBoard board = currentGame.getBoard();
        int[][] currentBoard = board.getBoard();

        if (handleGameOver(currentGame, currentBoard)) {
            return currentGame;
        }

        int bestScore = Integer.MIN_VALUE;
        int[] bestMove = new int[]{-1, -1};
        logger.info("Выполняется поиск оптимального хода для игры с ID: {}", game.getId());

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (currentBoard[i][j] == 0) {
                    currentBoard[i][j] = 2; // Ход компьютера
                    int score = minimax(currentBoard, 0, false);
                    currentBoard[i][j] = 0; // Отменяем ход
                    if (score > bestScore) {
                        bestScore = score;
                        bestMove[0] = i;
                        bestMove[1] = j;
                    }
                }
            }
        }
        // Делаем лучший ход
        if (bestMove[0] != -1 && bestMove[1] != -1) {
            currentGame.setPreviousBoard(utils.copyBoard(currentBoard));
            currentBoard[bestMove[0]][bestMove[1]] = 2;
            board.setBoard(currentBoard);
            currentGame.setBoard(board);
        }


        gameRepository.save(currentGame);
        //logger.info("Игра с ID {} обновлена: {}", game.getId(), currentGame);

        handleGameOver(currentGame, currentBoard);

        return currentGame;
    }

    @Override
    public boolean validateBoard(Game game) {
        int[][] board = game.getBoard().getBoard();
        int[][] previousBoard = game.getPreviousBoard();
        int changes = 0;
        boolean isValid = true;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] != 0 && board[i][j] != 1 && board[i][j] != 2) {
                    logger.error("Недопустимое значение в клетке [{}, {}]: {}", i, j, board[i][j]);
                    isValid = false;
                }

                if (board[i][j] != previousBoard[i][j] && previousBoard[i][j] != 0) {
                    logger.error("Недопустимое изменение: клетка [{}, {}] уже занята.", i, j);
                    isValid = false;
                }

                if (board[i][j] != previousBoard[i][j]) {
                    changes++;
                    if (changes > 1) {
                        logger.error("Недопустимое изменение: более одной клетки изменено за один ход.");
                        isValid = false;
                    }
                }
            }
        }

        if (isValid) {
            game.setPreviousBoard(utils.copyBoard(board));
        }
        return isValid;
    }

    @Override
    public boolean isGameOver(Game game) {
        return isGameOver(game.getBoard().getBoard());
    }

    private boolean isGameOver(int[][] board) {
        if (checkWinner(board, 1) || checkWinner(board, 2)) {
            return true;
        }

        // Проверка на ничью
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == 0) {
                    return false; // Есть свободные клетки, игра не окончена
                }
            }
        }

        // Если все клетки заняты и нет выигрышной комбинации, это ничья
        return true;
    }

    private int minimax(int[][] board, int depth, boolean isMaximizing) {
        // Проверяем, окончена ли игра
        if (isGameOver(board)) {
            return evaluate(board);
        }

        int bestScore;
        if (isMaximizing) {
            bestScore = Integer.MIN_VALUE;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (board[i][j] == 0) {
                        board[i][j] = 2; // Ход компьютера
                        int score = minimax(board, depth + 1, false);
                        board[i][j] = 0; // Отменяем ход
                        bestScore = Math.max(score, bestScore);
                    }
                }
            }
        } else {
            bestScore = Integer.MAX_VALUE;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (board[i][j] == 0) {
                        board[i][j] = 1; // Ход игрока
                        int score = minimax(board, depth + 1, true);
                        board[i][j] = 0; // Отменяем ход
                        bestScore = Math.min(score, bestScore);
                    }
                }
            }
        }
        return bestScore;
    }

    private int evaluate(int[][] board) {
        // Проверяем, выиграл ли компьютер
        if (checkWinner(board, 2)) {
            return 10;
        }
        // Проверяем, выиграл ли игрок
        if (checkWinner(board, 1)) {
            return -10;
        }
        // Ничья
        return 0;
    }

    private boolean checkWinner(int[][] board, int player) {
        // Проверка строк и столбцов
        for (int i = 0; i < 3; i++) {
            if (board[i][0] == player && board[i][1] == player && board[i][2] == player) {
                return true;
            }
            if (board[0][i] == player && board[1][i] == player && board[2][i] == player) {
                return true;
            }
        }
        // Проверка диагоналей
        if (board[0][0] == player && board[1][1] == player && board[2][2] == player) {
            return true;
        }
        return board[0][2] == player && board[1][1] == player && board[2][0] == player;
    }

    private boolean handleGameOver(Game game, int[][] currentBoard) {
        if (isGameOver(currentBoard)) {
            if (checkWinner(currentBoard, 1)) {
                logger.info("Игра с ID {} завершена. Победил игрок 1.", game.getId());
            } else if (checkWinner(currentBoard, 2)) {
                logger.info("Игра с ID {} завершена. Победил игрок 2.", game.getId());
            } else {
                logger.info("Игра с ID {} завершена. Ничья.", game.getId());
            }
            archivedGameRepository.archive(game);
            gameRepository.delete(game.getId());
            return true;
        }
        return false;
    }
}