package s21.example.domain.service;

// Импорты для работы с JPA и Spring:
import jakarta.persistence.OptimisticLockException; // Для обработки оптимистичной блокировки
import org.springframework.beans.factory.annotation.Autowired; // Для внедрения зависимостей
import org.springframework.stereotype.Service; // Для пометки класса как сервиса
import org.springframework.transaction.annotation.Transactional; // Для управления транзакциями

// Импорты для работы с мапперами и сущностями:
import s21.example.datasource.mapper.GameMapper; // Маппер для преобразования Game <-> GameEntity
import s21.example.datasource.model.GameEntity; // Сущность игры в БД
import s21.example.datasource.repository.GameRepository; // Репозиторий для работы с играми
import s21.example.datasource.repository.UserRepository; // Репозиторий для работы с пользователями

// Импорты доменных моделей:
import s21.example.domain.model.Game; // Доменная модель игры
import s21.example.domain.model.GameBoard; // Доменная модель игрового поля
import s21.example.domain.model.GameState; // Перечисление состояний игры

// Импорты для логирования:
import org.slf4j.Logger; // Интерфейс логгера
import org.slf4j.LoggerFactory; // Фабрика логгеров

// Импорты утилит:
import s21.example.utils.Utils; // Вспомогательные утилиты

// Импорты Java стандартной библиотеки:
import java.time.LocalDateTime; // Для работы с датой/временем
import java.util.List; // Для работы со списками
import java.util.Optional; // Для работы с Optional
import java.util.UUID; // Для генерации уникальных идентификаторов
import java.util.stream.Collectors; // Для работы со стримами


public class GameServiceImpl implements GameService {
    private final GameRepository gameRepository;

    private final UserRepository userRepository;
    private final Utils utils = new Utils();
    private static final Logger logger = LoggerFactory.getLogger(GameServiceImpl.class);

    @Autowired
    public GameServiceImpl(GameRepository gameRepository,
                           UserRepository userRepository) {
        this.gameRepository = gameRepository;
        this.userRepository = userRepository;
    }

    @Override
    @Transactional
    public Game startNewGame(UUID playerId) {
        try {
            logger.info("Создание новой игры...");
            Game game = new Game();
            logger.info("Игра создана: {}", game.getId());
            game.setPreviousBoard(utils.copyBoard(game.getBoard().getBoard()));
            logger.info("Предыдущее состояние доски установлено.");
            GameEntity entity = GameMapper.toEntity(game);
            logger.info("Сущность GameEntity создана: {}", entity.getId());

            entity.setVersion(0);

            gameRepository.save(entity);
            logger.info("Игра сохранена в базу данных: {}", entity.getId());
            return game;
        } catch (Exception e) {
            logger.error("Ошибка при создании новой игры: {}", e.getMessage());
            throw new RuntimeException("Не удалось создать новую игру", e);
        }
    }

    @Override
    @Transactional
    public Game makeMove(UUID gameId, int[][] requestedBoard, Integer version, UUID playerId) {

        GameEntity gameEntity = gameRepository.findById(gameId)
                .orElseThrow(() -> {
                    logger.error("Игра с ID {} не найдена", gameId);
                    return new RuntimeException("Игра с ID " + gameId + " не найдена");
                });

        logger.info("Запрос версии: {}, Текущая версия в БД: {}", version, gameEntity.getVersion());

        Game currentGame = GameMapper.toDomain(gameEntity);

        // Проверка версии (оптимистичная блокировка)
        if (gameEntity.getVersion() != null && !gameEntity.getVersion().equals(version)) {
            throw new OptimisticLockException(
                    "Версия не совпадает. Текущая версия: " + gameEntity.getVersion() +
                            ", ваша версия: " + version + ". Получите актуальное состояние игры."
            );
        }

        validateGameNotFinished(currentGame);

        // Проверяем, чей сейчас ход
        if (currentGame.getState() == GameState.PLAYER_1_TURN) {
            validatePlayerTurn(currentGame, playerId);
        } else if (currentGame.getState() == GameState.PLAYER_2_TURN && !currentGame.isVsComputer()) {
            validatePlayerTurn(currentGame, playerId);
        }

        // Сохраняем предыдущее состояние
        currentGame.setPreviousBoard(utils.copyBoard(currentGame.getBoard().getBoard()));

        // Обрабатываем ход игрока
        processPlayerMove(currentGame, requestedBoard, playerId);

        // Проверяем завершение игры после хода игрока
        if (isGameOver(currentGame)) {
            handleGameOver(currentGame, currentGame.getBoard().getBoard());
            GameEntity finalEntity = gameRepository.save(GameMapper.toEntity(currentGame));
            return GameMapper.toDomain(finalEntity);
        }

        // Меняем состояние для СЛЕДУЮЩЕГО хода
        updateGameState(currentGame);

        // Обрабатываем ход компьютера (только если это игра с компьютером)
        if (currentGame.isVsComputer() && currentGame.getState() == GameState.PLAYER_2_TURN) {
            currentGame = processComputerMove(gameId);

            // Проверяем завершение после хода компьютера
            if (isGameOver(currentGame)) {
                handleGameOver(currentGame, currentGame.getBoard().getBoard());
                GameEntity computerFinalEntity = gameRepository.save(GameMapper.toEntity(currentGame));
                return GameMapper.toDomain(computerFinalEntity);
            }

            // Возвращаем очередь игроку после хода компьютера
            currentGame.setState(GameState.PLAYER_1_TURN);
        }

        // Сохраняем итоговое состояние
        GameEntity updatedEntity = gameRepository.save(GameMapper.toEntity(currentGame));
        return GameMapper.toDomain(updatedEntity);
    }
    private void validatePlayerTurn(Game game, UUID playerId) {
        if ((game.getState() == GameState.PLAYER_1_TURN && !playerId.equals(game.getPlayer1Id()))) {
            throw new IllegalStateException("Сейчас ход игрока 1 (ожидается '1')");
        } else if (game.getState() == GameState.PLAYER_2_TURN && !playerId.equals(game.getPlayer2Id())) {
            throw new IllegalStateException("Сейчас ход игрока 2 (ожидается '2')");
        }
    }

    private void processPlayerMove(Game currentGame, int[][] requestedBoard, UUID playerId) {

        int[][] currentBoard = currentGame.getBoard().getBoard();

        int changedRow = -1;
        int changedCol = -1;
        int changes = 0;

        int expectedPlayerSymbol = (currentGame.getPlayer1Id().equals(playerId)) ? 1 : 2;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (requestedBoard[i][j] != currentBoard[i][j]) {

                    // Проверка 1: Клетка должна быть свободна
                    if (currentBoard[i][j] != 0) {
                        logger.error("Клетка [{},{}] уже занята: {}", i, j, currentBoard[i][j]);
                        throw new IllegalArgumentException("Клетка уже занята");
                    }

                    // Проверка 2: Должен ставиться символ текущего игрока
                    if (requestedBoard[i][j] != expectedPlayerSymbol) {
                        logger.error("Ожидался {}, получен {} в [{},{}]",
                                expectedPlayerSymbol, requestedBoard[i][j], i, j);
                        throw new IllegalArgumentException("Неверный символ игрока");
                    }

                    currentBoard[i][j] = requestedBoard[i][j];
                    changedRow = i;
                    changedCol = j;
                    changes++;
                }
            }
        }

        if (changedRow == -1 || changedCol == -1) {
            throw new IllegalArgumentException("Не сделан ни один ход");
        }

        if (changes != 1) {
            logger.error("Требуется изменить ровно 1 клетку. Изменений: {}", changes);
            throw new IllegalArgumentException("Можно изменить только одну клетку");
        }

        // Финальная валидация состояния игры
        if (!validateBoardWithState(currentGame, playerId)) {
            throw new IllegalArgumentException("Некорректные данные для игры");
        }
    }

    // Обновление состояния игры
    private void updateGameState(Game game) {
            game.setState(
                    game.getState() == GameState.PLAYER_1_TURN
                            ? GameState.PLAYER_2_TURN
                            : GameState.PLAYER_1_TURN
            );
    }

    @Transactional
    protected Game processComputerMove(UUID gameId) {
        GameEntity computerGameEntity = gameRepository.findById(gameId)
                .orElseThrow(() -> new RuntimeException("Игра не найдена для хода компьютера"));

        Game computerGame = GameMapper.toDomain(computerGameEntity);

        // Вычисляем лучший ход
        int[] bestMove = findBestMove(computerGame.getBoard().getBoard());

        // Применяем ход компьютера
        if (bestMove[0] != -1) {
            int[][] board = computerGame.getBoard().getBoard();
            board[bestMove[0]][bestMove[1]] = 2;
            computerGame.getBoard().setBoard(board);
        }

        // Возвращаем очередь игроку
        computerGame.setState(GameState.PLAYER_1_TURN);

        // Сохраняем результат
        GameEntity computerEntity = gameRepository.save(GameMapper.toEntity(computerGame));
        return GameMapper.toDomain(computerEntity);
    }

    private int[] findBestMove(int[][] board) {
        int bestScore = Integer.MIN_VALUE;
        int[] bestMove = {-1, -1};

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == 0) {
                    board[i][j] = 2; // Ход компьютера
                    int score = minimax(board, 0, false);
                    board[i][j] = 0; // Отменяем ход
                    if (score > bestScore) {
                        bestScore = score;
                        bestMove[0] = i;
                        bestMove[1] = j;
                    }
                }
            }
        }

        return bestMove;
    }

    @Override
    @Transactional
    public Game createGame(UUID playerId, boolean vsComputer) {

        // Проверяем существование игрока
        if (!userRepository.existsById(playerId)) {
            throw new IllegalArgumentException("Игрок не найден");
        }

        // Проверяем активные игры и получаем информацию о них
        Optional<Game> activeGame = getActiveGameByPlayer(playerId);

        if (activeGame.isPresent()) {
            Game game = activeGame.get();
            throw new IllegalStateException(
                    "У вас уже есть активная игра: " + game.getId() +
                            " (статус: " + game.getState() + ")" +
                            (game.isVsComputer() ? " [против компьютера]" : " [против игрока]")
            );
        }

        // Создаем новую игру
        Game game = new Game();
        game.setPlayer1Id(playerId);

        if (vsComputer) {
            game.setPlayer2Id(null);// Имитация ID компьютера
            game.setVsComputer(true);
            game.setState(GameState.PLAYER_1_TURN); // Первый ход игрока
        } else {
            game.setVsComputer(false);
            game.setPlayer2Id(null); // Будет установлен позже
            game.setState(GameState.WAITING_FOR_PLAYERS); // Ожидание второго игрока
        }

        if (game.getBoard() == null) {
            game.setBoard(new GameBoard());
        }
        game.setPreviousBoard(utils.copyBoard(game.getBoard().getBoard()));

        GameEntity entity = new GameEntity();
        entity.setPlayer1Id(playerId);
        entity.setVsComputer(vsComputer);
        entity.setState(vsComputer ? GameState.PLAYER_1_TURN : GameState.WAITING_FOR_PLAYERS);
        entity.setBoard(new int[3][3]);

        GameEntity saved = gameRepository.save(entity); // Сохраняем сразу

        logger.info("Saved entity version: {}", saved.getVersion());

        return GameMapper.toDomain(saved);
    }

    public Optional<Game> getActiveGameByPlayer(UUID playerId) {

        logger.info("Checking active games for player: {}", playerId);

        // Завершенные состояния
        List<GameState> finishedStates = List.of(
                GameState.DRAW,
                GameState.PLAYER_1_WIN,
                GameState.PLAYER_2_WIN
        );

        // Ищем игры, где пользователем является player1 и игра НЕ завершена
        List<GameEntity> gamesAsPlayer1 = gameRepository.findByPlayer1Id(playerId);
        List<GameEntity> activePlayer1Games = gamesAsPlayer1.stream()
                .filter(game -> !finishedStates.contains(game.getState()))
                .collect(Collectors.toList());

        logger.info("Player1 - total games: {}, active: {}", gamesAsPlayer1.size(), activePlayer1Games.size());

        if (!activePlayer1Games.isEmpty()) {
            logger.info("Found active game as player1: {}", activePlayer1Games.get(0).getId());
            return Optional.of(GameMapper.toDomain(activePlayer1Games.get(0)));
        }

        // Ищем игры, где пользователем является player2 и игра НЕ завершена
        List<GameEntity> gamesAsPlayer2 = gameRepository.findByPlayer2Id(playerId);
        List<GameEntity> activePlayer2Games = gamesAsPlayer2.stream()
                .filter(game -> !finishedStates.contains(game.getState()))
                .collect(Collectors.toList());

        if (!activePlayer2Games.isEmpty()) {
            logger.info("Found active game as player2: {}", activePlayer2Games.get(0).getId());
            return Optional.of(GameMapper.toDomain(activePlayer2Games.get(0)));
        }
        logger.info("No active games found for player: {}", playerId);
        return Optional.empty();
    }

    @Override
    public Game joinGame(UUID gameId, UUID player2Id) {
        // Проверяем существование игрока
        if (!userRepository.existsById(player2Id)) {
            throw new IllegalArgumentException("Игрок не найден");
        }

        GameEntity gameEntity = gameRepository.findById(gameId)
                .orElseThrow(() -> new RuntimeException("Игра не найдена"));

        // Проверка 1: Нельзя присоединиться к игре с компьютером
        if (gameEntity.isVsComputer()) {
            throw new IllegalStateException("К игре с компьютером нельзя присоединиться");
        }

        // Проверка 2: Игра уже имеет второго игрока (корректная проверка)
        if (gameEntity.getPlayer2Id() != null) {
            throw new IllegalArgumentException("Игра уже имеет второго игрока");
        }

        // Проверка 3: Нельзя присоединиться к своей же игре
        if (player2Id.equals(gameEntity.getPlayer1Id())) {
            throw new IllegalArgumentException("Нельзя присоединиться к своей же игре");
        }

        // Проверка 4: Игра должна быть в состоянии ожидания игроков
        if (gameEntity.getState() != GameState.WAITING_FOR_PLAYERS) {
            throw new IllegalStateException("Игра не ожидает присоединения игроков");
        }

        // Обновляем сущность
        gameEntity.setPlayer2Id(player2Id);
        gameEntity.setState(GameState.PLAYER_1_TURN);
        gameEntity.setUpdatedAt(LocalDateTime.now());

        GameEntity savedEntity = gameRepository.save(gameEntity);
        logger.info("Player {} joined game {}", player2Id, gameId);
        return GameMapper.toDomain(savedEntity);
    }

    @Override
    public Game getGameById(UUID gameId) {
        GameEntity gameEntity = gameRepository.findById(gameId)
                .orElseThrow(() -> new RuntimeException("Игра с ID " + gameId + " не найдена"));
        return GameMapper.toDomain(gameEntity);
    }

    @Override
    public Game getNextMove(Game game) {
        Game resultGame = new Game();
        resultGame.setId(game.getId());
        resultGame.setBoard(new GameBoard());
        resultGame.getBoard().setBoard(utils.copyBoard(game.getBoard().getBoard()));
        resultGame.setPlayer1Id(game.getPlayer1Id());
        resultGame.setPlayer2Id(game.getPlayer2Id());
        resultGame.setVsComputer(game.isVsComputer());
        resultGame.setState(game.getState());
        resultGame.setVersion(game.getVersion());

        int[] bestMove = findBestMove(resultGame.getBoard().getBoard());

        if (bestMove[0] != -1) {
            int[][] resultBoard = resultGame.getBoard().getBoard();
            resultBoard[bestMove[0]][bestMove[1]] = 2;
        }

        return resultGame;
    }

    @Override
    public boolean validateBoardWithState(Game game, UUID playerId) {
        int[][] board = game.getBoard().getBoard();
        int[][] previousBoard = game.getPreviousBoard();
        int changes = 0;
        int expectedPlayerSymbol =
                (game.getPlayer1Id().equals(playerId)) ? 1 : 2;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] != previousBoard[i][j]) {
                    // Проверка 1: Клетка должна быть свободна
                    if (previousBoard[i][j] != 0) {
                        logger.error("Клетка [{},{}] уже занята: {}", i, j, previousBoard[i][j]);
                        return false;
                    }

                    // Проверка 2: Должен ставиться символ текущего игрока
                    if (board[i][j] != expectedPlayerSymbol) {
                        logger.error("Ожидался {}, получен {} в [{},{}]",
                                expectedPlayerSymbol, board[i][j], i, j);
                        return false;
                    }

                    changes++;
                }
            }
        }

        if (changes != 1) {
            logger.error("Требуется изменить ровно 1 клетку. Изменений: {}", changes);
            return false;
        }

        return true;
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
        return (board[0][0] == player && board[1][1] == player && board[2][2] == player) ||
                (board[0][2] == player && board[1][1] == player && board[2][0] == player);
    }

    private Game handleGameOver(Game game, int[][] currentBoard) {
        if (isGameOver(currentBoard)) {
            if (checkWinner(currentBoard, 1)) {
                game.setState(GameState.PLAYER_1_WIN);
                logger.info("Игра с ID {} завершена. Победил игрок 1.", game.getId());
            } else if (checkWinner(currentBoard, 2)) {
                game.setState(GameState.PLAYER_2_WIN);
                logger.info("Игра с ID {} завершена. Победил игрок 2.", game.getId());
            } else {
                logger.info("Игра с ID {} завершена. Ничья.", game.getId());
                game.setState(GameState.DRAW);
            }
            // Игра завершена, оставляем в основной таблице для истории
            logger.info("Игра {} завершена со статусом: {}", game.getId(), game.getState());
            return game;
        }
        return game;
    }

    @Override
    public List<Game> getAvailableGames() {
        return gameRepository.findByState(GameState.WAITING_FOR_PLAYERS).stream()
                .map(GameMapper::toDomain)
                .collect(Collectors.toList());
    }

    // Проверка на уже завершенную игру
    private void validateGameNotFinished(Game game) {
        if (game.getState() == GameState.PLAYER_1_WIN ||
                game.getState() == GameState.PLAYER_2_WIN ||
                game.getState() == GameState.DRAW) {
            throw new IllegalStateException("Игра уже завершена");
        }
    }

    @Override
    @Transactional
    public void deleteGame(UUID gameId, UUID playerId) {
        GameEntity gameEntity = gameRepository.findById(gameId)
                .orElseThrow(() -> new RuntimeException("Игра не найдена"));

        // Проверяем, что игрок является участником игры
        if (!gameEntity.getPlayer1Id().equals(playerId) &&
                !gameEntity.getPlayer2Id().equals(playerId)) {
            throw new SecurityException("Вы не являетесь участником этой игры");
        }

        // Проверяем, что игра завершена (опционально)
        if (!isGameFinished(gameEntity.getState())) {
            throw new IllegalStateException("Нельзя удалить незавершенную игру");
        }

        gameRepository.deleteById(gameId);
        logger.info("Игра {} удалена игроком {}", gameId, playerId);
    }

    @Override
    @Transactional
    public void deleteGameAdmin(UUID gameId) {
        if (!gameRepository.existsById(gameId)) {
            throw new RuntimeException("Игра не найдена");
        }
        gameRepository.deleteById(gameId);
        logger.info("Игра {} удалена администратором", gameId);
    }
    private boolean isGameFinished(GameState state) {
        return state == GameState.PLAYER_1_WIN ||
                state == GameState.PLAYER_2_WIN ||
                state == GameState.DRAW;
    }
}