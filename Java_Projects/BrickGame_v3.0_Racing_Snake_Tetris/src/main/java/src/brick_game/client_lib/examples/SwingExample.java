/**
 * @file: SwingExample.java
 * @description: Пример desktop GUI (Swing) поверх client_lib для визуализации состояния
 * @dependencies: javax.swing.*, src.brick_game.client_lib.*
 * @created: 2025-10-05
 */
package src.brick_game.client_lib.examples;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.SwingUtilities;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.Rectangle;
import javax.swing.ImageIcon;
import src.brick_game.client_lib.BrickGameClient;
import src.brick_game.client_lib.RestBrickGameClient;
import src.brick_game.client_lib.dto.GameState;

public class SwingExample extends JPanel {
  private final BrickGameClient client = new RestBrickGameClient("http://localhost:8080/api");
  private volatile GameState state;
  private volatile boolean hold = false;
  private volatile boolean spaceHeld = false; // Для Snake ускорения
  private volatile boolean lastAccelState = false; // Последнее состояние ускорения
  private volatile boolean lastSnakeAccelState = false; // Последнее состояние ускорения Snake
  private int snakeAccelTickCounter = 0; // Счётчик тиков для throttling ускорения Snake
  private volatile int edgePhase = 0; // смещение узора краёв дороги
  // private volatile int prevOppMinY = -1; // предыдущая минимальная y соперников
  // (для синхронизации
  // // краёв)
  private volatile int playerVisualOffset = 0; // визуальный выход за край [-EDGE_COLS..EDGE_COLS]
  // без дополнительной интерполяции; простая отрисовка текущего кадра
  private volatile boolean gamePaused = false; // Состояние паузы
  private volatile boolean gameStarted = false; // Состояние старта игры
  private volatile boolean gameOver = false; // Состояние окончания игры
  private volatile boolean seenRunning = false; // Видели ли активное состояние игры
  private int lastTickForEdges = -1; // Последний tickCounter, учтённый для движения краёв
  private int lastDivForEdges = -1; // Последнее значение (tickCounter / updateInterval)
  private int lastUpdateInterval = -1; // Последний применённый updateInterval

  private int selectedGame = 1; // 1=Racing, 2=Tetris, 3=Snake
  private boolean showGameMenu = true;
  private volatile boolean stopPolling = false; // Флаг для остановки опроса сервера
  private JFrame parentFrame = null; // Ссылка на родительское окно для изменения заголовка
  private final java.util.Map<String, Integer> persistentCarColors = new java.util.HashMap<>(); // Постоянные
                                                                                                // цвета
  // Визуальные настройки дорожного полотна
  private static final int ROAD_WIDTH = 10; // логическая ширина игры
  private static final int EDGE_COLS = 2; // визуальные края слева/справа
  private static final int TOTAL_COLS = ROAD_WIDTH + EDGE_COLS * 2; // 14
  private static final int TOTAL_ROWS = 20; // высота поля
  private static final int SIDEBAR_WIDTH_PX = 200; // ширина правой панели (увеличена)

  // Иконки игр (попытка загрузки из ресурсов и из файловой системы)
  private static final int ICON_SIZE = 32; // Увеличен размер для лучшей видимости
  private static final ImageIcon ICON_RACING = scaleIcon(tryLoadIcon("racing-car.png"), ICON_SIZE);
  private static final ImageIcon ICON_TETRIS = scaleIcon(tryLoadIcon("tetris.png"), ICON_SIZE);
  private static final ImageIcon ICON_SNAKE = scaleIcon(tryLoadIcon("snake.png"), ICON_SIZE);
  private static final Rectangle[] MENU_RECTS = new Rectangle[] { new Rectangle(), new Rectangle(), new Rectangle() };

  private static ImageIcon scaleIcon(ImageIcon original, int size) {
    if (original == null || original.getIconWidth() <= 0) {
      System.out.println("[ICON] Cannot scale - original is null or invalid");
      return null;
    }
    Image scaled = original.getImage().getScaledInstance(size, size, Image.SCALE_SMOOTH);
    ImageIcon result = new ImageIcon(scaled);
    System.out.println("[ICON] Scaled to " + size + "x" + size);
    return result;
  }

  private static ImageIcon tryLoadIcon(String name) {
    // 1) Абсолютный путь ресурсов в jar
    java.net.URL cpAbs = SwingExample.class.getResource("/images/" + name);
    if (cpAbs != null) {
      ImageIcon icon = new ImageIcon(cpAbs);
      System.out.println("[ICON] Loaded from /images/" + name + " [" + icon.getIconWidth() + "x"
          + icon.getIconHeight() + "]");
      return icon;
    }
    // 2) Относительно classloader
    java.net.URL cp = SwingExample.class.getClassLoader().getResource("images/" + name);
    if (cp != null) {
      ImageIcon icon = new ImageIcon(cp);
      System.out.println("[ICON] Loaded from classpath images/" + name + " [" + icon.getIconWidth()
          + "x" + icon.getIconHeight() + "]");
      return icon;
    }
    ImageIcon rel = new ImageIcon("BrickGame/images/" + name);
    if (rel.getIconWidth() > 0) {
      System.out.println("[ICON] Loaded from relative BrickGame/images/" + name + " ["
          + rel.getIconWidth() + "x" + rel.getIconHeight() + "]");
      return rel;
    }
    String base = System.getProperty("user.dir");
    ImageIcon abs = new ImageIcon(base + "/BrickGame/images/" + name);
    if (abs.getIconWidth() > 0) {
      System.out.println("[ICON] Loaded from absolute " + base + "/BrickGame/images/" + name + " ["
          + abs.getIconWidth() + "x" + abs.getIconHeight() + "]");
      return abs;
    }
    System.out.println("[ICON] Not found: " + name + " (tried all paths)");
    return null;
  }

  public SwingExample() {
    setPreferredSize(new Dimension(500, 420)); // Увеличена ширина для большей панели
    setBackground(Color.BLACK);
    setFocusable(true);
    requestFocusInWindow(); // Запрашиваем фокус сразу
    addKeyListener(new KeyAdapter() {
      @Override
      public void keyPressed(KeyEvent e) {
        if (showGameMenu) {
          handleMenuKey(e);
          return;
        }

        // Контекстное управление в зависимости от выбранной игры
        if (selectedGame == 1) {
          handleRacingKeys(e);
        } else if (selectedGame == 2) {
          handleTetrisKeys(e);
        } else if (selectedGame == 3) {
          handleSnakeKeys(e);
        }
      }

      private void handleRacingKeys(KeyEvent e) {
        switch (e.getKeyCode()) {
          case KeyEvent.VK_A:
          case KeyEvent.VK_LEFT:
            if (gameStarted && !gamePaused && !gameOver) {
              try {
                client.sendAction(3, false).join(); // ACTION_LEFT - мгновенно
              } catch (Exception ignored) {
              }
            }
            break;
          case KeyEvent.VK_D:
          case KeyEvent.VK_RIGHT:
            if (gameStarted && !gamePaused && !gameOver) {
              try {
                client.sendAction(4, false).join(); // ACTION_RIGHT - мгновенно
              } catch (Exception ignored) {
              }
            }
            break;
          case KeyEvent.VK_W:
          case KeyEvent.VK_UP:
            hold = true;
            break;
          case KeyEvent.VK_SPACE:
            if (!gameStarted || gameOver) {
              try {
                // Создаем игру при первом нажатии или после Game Over
                // Сначала сбрасываем флаги и старое состояние
                gameOver = false;
                gamePaused = false;
                seenRunning = false;
                if (state != null) {
                  state.setPauseState(0);
                }
                // Стартуем игру
                client.startGame(selectedGame).join();
                gameStarted = true;
                // Убираем автоматическое ускорение - скорость теперь управляется уровнем
              } catch (Exception ignored) {
              }
            } else if (gameStarted && !gameOver) {
              // Пробел также работает как пауза во время игры
              gamePaused = !gamePaused;
              try {
                client.sendAction(1, gamePaused).join(); // ACTION_PAUSE
              } catch (Exception ignored) {
              }
            }
            break;
          case KeyEvent.VK_P:
          case KeyEvent.VK_PAUSE:
            // P и Pause для паузы (только если игра уже запущена)
            if (gameStarted && !gameOver) {
              gamePaused = !gamePaused;
              try {
                client.sendAction(1, gamePaused).join(); // ACTION_PAUSE
              } catch (Exception ignored) {
              }
            }
            break;
          case KeyEvent.VK_ESCAPE:
            goToMenu();
            break;
        }
      }

      private void handleTetrisKeys(KeyEvent e) {
        switch (e.getKeyCode()) {
          case KeyEvent.VK_LEFT:
            if (gameStarted && !gamePaused && !gameOver) {
              try {
                client.sendAction(3, false).join(); // LEFT
              } catch (Exception ignored) {
              }
            }
            break;
          case KeyEvent.VK_RIGHT:
            if (gameStarted && !gamePaused && !gameOver) {
              try {
                client.sendAction(4, false).join(); // RIGHT
              } catch (Exception ignored) {
              }
            }
            break;
          case KeyEvent.VK_DOWN:
            if (gameStarted && !gamePaused && !gameOver) {
              try {
                client.sendAction(6, false).join(); // DOWN (код 6, не 2!)
              } catch (Exception ignored) {
              }
            }
            break;
          case KeyEvent.VK_SPACE:
            if (gameOver) {
              // Рестарт игры после Game Over по Space
              try {
                System.out.println("[SwingExample] Restarting Tetris, selectedGame=" + selectedGame);
                // Сбрасываем флаги и старое состояние
                gameOver = false;
                gamePaused = false;
                seenRunning = false; // Чтобы заново установить seenRunning
                // Очищаем старое состояние
                state = new GameState();
                state.setField(new boolean[20][10]);
                // startGame автоматически сбросит старую игру на сервере
                client.startGame(selectedGame).join();
                gameStarted = true;
                System.out.println("[SwingExample] Tetris restart request sent");
              } catch (Exception ex) {
                System.err.println("[SwingExample] Failed to restart game: " + ex.getMessage());
                ex.printStackTrace();
              }
            } else if (gameStarted && !gamePaused) {
              // Rotate во время игры
              try {
                client.sendAction(7, false).join(); // ROTATE (Action, код 7, не 5!)
              } catch (Exception ignored) {
              }
            }
            break;
          case KeyEvent.VK_P:
          case KeyEvent.VK_PAUSE:
            if (gameStarted && !gameOver) {
              try {
                client.sendAction(1, true).join(); // PAUSE
                gamePaused = !gamePaused;
              } catch (Exception ignored) {
              }
            }
            break;
          case KeyEvent.VK_ESCAPE:
            goToMenu();
            break;
        }
      }

      private void handleSnakeKeys(KeyEvent e) {
        switch (e.getKeyCode()) {
          case KeyEvent.VK_LEFT:
            if (gameStarted && !gamePaused && !gameOver) {
              try {
                client.sendAction(3, false).join(); // LEFT = 3
              } catch (Exception ignored) {
              }
            }
            break;
          case KeyEvent.VK_RIGHT:
            if (gameStarted && !gamePaused && !gameOver) {
              try {
                client.sendAction(4, false).join(); // RIGHT = 4
              } catch (Exception ignored) {
              }
            }
            break;
          case KeyEvent.VK_UP:
            if (gameStarted && !gamePaused && !gameOver) {
              try {
                client.sendAction(5, false).join(); // UP = 5
              } catch (Exception ignored) {
              }
            }
            break;
          case KeyEvent.VK_DOWN:
            if (gameStarted && !gamePaused && !gameOver) {
              try {
                client.sendAction(6, false).join(); // DOWN = 6
              } catch (Exception ignored) {
              }
            }
            break;
          case KeyEvent.VK_SPACE:
            if (gameOver) {
              // Рестарт игры после Game Over по Space
              try {
                System.out.println("[SwingExample] Restarting Snake, selectedGame=" + selectedGame);
                gameOver = false;
                gamePaused = false;
                seenRunning = false;
                state = new GameState();
                state.setField(new boolean[20][10]);
                client.startGame(selectedGame).join();
                gameStarted = true;
                System.out.println("[SwingExample] Snake restart request sent");
              } catch (Exception ex) {
                System.err.println("[SwingExample] Failed to restart game: " + ex.getMessage());
                ex.printStackTrace();
              }
            } else if (gameStarted && !gamePaused) {
              // Ускорение (Action) - устанавливаем флаг удержания
              spaceHeld = true;
              System.out.println("[Snake] Space pressed, spaceHeld = true");
            }
            break;
          case KeyEvent.VK_P:
          case KeyEvent.VK_PAUSE:
            if (gameStarted && !gameOver) {
              try {
                client.sendAction(1, true).join(); // PAUSE = 1
                gamePaused = !gamePaused;
              } catch (Exception ignored) {
              }
            }
            break;
          case KeyEvent.VK_ESCAPE:
            goToMenu();
            break;
        }
      }

      @Override
      public void keyReleased(KeyEvent e) {
        if (e.getKeyCode() == KeyEvent.VK_W || e.getKeyCode() == KeyEvent.VK_UP) {
          hold = false;
        }
        if (e.getKeyCode() == KeyEvent.VK_SPACE && selectedGame == 3) {
          spaceHeld = false;
          System.out.println("[Snake] Space released, spaceHeld = false");
        }
      }
    });

    // Клик мышью по пункту меню для выбора/старта
    addMouseListener(new MouseAdapter() {
      @Override
      public void mouseClicked(MouseEvent e) {
        if (!showGameMenu)
          return;
        for (int i = 0; i < MENU_RECTS.length; i++) {
          if (MENU_RECTS[i] != null && MENU_RECTS[i].contains(e.getPoint())) {
            selectedGame = i + 1;
            // ЛКМ — старт сразу
            if (e.getButton() == MouseEvent.BUTTON1) {
              System.out.println(
                  "[SwingExample] Mouse click: Starting game with selectedGame=" + selectedGame);
              resetLocalStateForNewGame();
              showGameMenu = false;
              try {
                System.out.println(
                    "[SwingExample] Mouse: Calling client.startGame(" + selectedGame + ")");
                client.startGame(selectedGame).join();
                gameStarted = true;
                gameOver = false;
                gamePaused = false;
                updateWindowTitle(); // Обновляем заголовок при запуске игры через клик
                System.out.println("[SwingExample] Mouse: Game started successfully, selectedGame="
                    + selectedGame);
              } catch (Exception ex) {
                System.err.println("[SwingExample] Failed to start game: " + ex.getMessage());
                ex.printStackTrace();
                showGameMenu = true;
              }
              repaint();
            }
            break;
          }
        }
      }
    });

    // Фоновый поток: опрос состояния и простая отрисовка (без интерполяции)
    new Thread(() -> {
      try {
        // Не создаем игру сразу - ждем нажатия пробела
        // Устанавливаем пустое состояние для показа стартового экрана
        state = new GameState(); // Пустое состояние
        state.setField(new boolean[20][10]); // Пустое поле для избежания NPE
        // Принудительно перерисовываем окно
        SwingUtilities.invokeLater(() -> repaint());
        while (!stopPolling) {
          boolean shouldUpdateEdges = false;

          if (gameStarted && !showGameMenu) {
            // Отправляем ускорение только при изменении состояния
            if (selectedGame == 1 && !gamePaused && !gameOver && hold != lastAccelState) {
              client.sendAction(5, hold).join(); // ACTION_ACCELERATE для Racing
              lastAccelState = hold;
            }

            // Отправляем ускорение для Snake (Action = 7 с hold)
            // ВАЖНО: используем throttling - отправляем каждые 5 тиков (~165мс) вместо
            // каждого тика
            // Это даёт контролируемое ускорение без "выстрела" в стену
            if (selectedGame == 3 && !gamePaused && !gameOver && spaceHeld) {
              snakeAccelTickCounter++;
              if (snakeAccelTickCounter >= 5) { // Каждый 5-й тик (~165мс) - медленное ускорение
                client.sendAction(7, true).join(); // ACTION для Snake с hold=true
                snakeAccelTickCounter = 0;
                lastSnakeAccelState = true;
              }
            } else if (selectedGame == 3 && !gamePaused && !gameOver && !spaceHeld && lastSnakeAccelState) {
              // Отправляем hold=false только один раз при отпускании
              client.sendAction(7, false).join();
              lastSnakeAccelState = false;
              snakeAccelTickCounter = 0; // Сбрасываем счётчик
            }

            // Получаем состояние каждый тик для плавной прорисовки машин
            state = client.getState().join();

            // ЖЁСТКАЯ СИНХРОНИЗАЦИЯ КРАЁВ С FSM: учитываем все срабатывания движения между
            // состояниями
            if (state != null && selectedGame == 1) {
              int currentSpeed = state.getSpeed();
              int movementSpeed = 1; // шаг всегда 1 клетка (частота задаёт скорость)
              // Согласуем с новой шкалой сервера: speed:1..7 → 7..1, >=8 → 1
              int capped = Math.min(currentSpeed, 7);
              int updateInterval = Math.max(1, 8 - capped);
              int tick = state.getTickCounter();
              if (!gamePaused && !gameOver && movementSpeed > 0) {
                // Если интервал изменился (например, при ускорении), перенастроим базу
                // сравнения
                if (lastUpdateInterval != updateInterval) {
                  lastDivForEdges = tick / updateInterval;
                  lastTickForEdges = tick;
                  lastUpdateInterval = updateInterval;
                  // если событие попало точно на границу нового интервала — обработаем один раз
                  if (tick % updateInterval == 0) {
                    for (int i = 0; i < movementSpeed; i++) {
                      edgePhase = (edgePhase - 1 + 4) % 4;
                    }

                  }
                }
                int divNow = tick / updateInterval;
                if (lastDivForEdges < 0 || lastTickForEdges < 0) {
                  lastDivForEdges = divNow;
                  lastTickForEdges = tick;
                  lastUpdateInterval = updateInterval;
                } else {
                  int events = Math.max(0, divNow - lastDivForEdges);
                  if (events > 0) {
                    int steps = events * movementSpeed;
                    // Один шаг edgePhase соответствует смещению узора на одну строку
                    for (int i = 0; i < steps; i++) {
                      edgePhase = (edgePhase - 1 + 4) % 4; // навстречу игроку
                    }
                    lastDivForEdges = divNow;

                  }
                  lastTickForEdges = tick;
                }
              }
            }
          } else {
            // До старта игры не дергаем сервер, чтобы избежать NO_GAME_RUNNING
            // Локально поддерживаем пустое состояние для экрана меню/стартового экрана
            if (state == null || state.getField() == null) {
              state = new GameState();
              state.setField(new boolean[20][10]);
            }
          }

          // Отслеживаем первый кадр с активной игрой
          if (state != null && state.isGameRunning()) {
            seenRunning = true;
          }
          // Проверяем, закончилась ли игра
          // Для Tetris: pauseState==3 означает game over
          // Для Snake: pauseState==5 означает game over
          // Для Racing: !isGameRunning()
          if (gameStarted && state != null && seenRunning && !gameOver) {
            boolean isGameOverState = false;
            if (selectedGame == 2 && state.getPauseState() != null && state.getPauseState() == 3) {
              isGameOverState = true; // Tetris Game Over
            } else if (selectedGame == 3 && state.getPauseState() != null && state.getPauseState() == 5) {
              isGameOverState = true; // Snake Game Over
            } else if (!state.isGameRunning()) {
              isGameOverState = true; // Racing или общий флаг
            }
            if (isGameOverState) {
              gameOver = true;
              System.out.println("[SwingExample] Game Over detected! selectedGame=" + selectedGame
                  + ", pauseState=" + state.getPauseState());
            }
          }

          // Рассчитываем центр машины игрока по текущему кадру
          if (state != null && state.getField() != null) {
            Integer center = findPlayerCenter(state.getField());
          }

          // Убираем визуальное смещение - машина игрока строго в пределах дороги
          // Центр машины может занимать позиции 1-8 (8 клеток для центра, 10 для всей
          // машины)
          playerVisualOffset = 0;

          // Сброс при паузе/окончании/отсутствии состояния
          if (gamePaused || gameOver || state == null) {
            lastTickForEdges = -1;
            lastDivForEdges = -1;
            lastUpdateInterval = -1;

          }

          repaint();
          Thread.sleep(33);
        }
      } catch (Exception ignored) {
      }
    }).start();
  }

  private Integer findPlayerCenter(boolean[][] field) {
    int baseY = TOTAL_ROWS - 1;
    for (int cx = 0; cx < ROAD_WIDTH; cx++) {
      boolean ok = true;
      if (!(cx - 1 >= 0 && field[baseY][cx - 1]))
        ok = false;
      if (!field[baseY][cx])
        ok = false;
      if (!(cx + 1 < ROAD_WIDTH && field[baseY][cx + 1]))
        ok = false;
      if (!(baseY - 1 >= 0 && field[baseY - 1][cx]))
        ok = false;
      if (!(baseY - 2 >= 0 && cx - 1 >= 0 && field[baseY - 2][cx - 1]))
        ok = false;
      if (!(baseY - 2 >= 0 && field[baseY - 2][cx]))
        ok = false;
      if (!(baseY - 2 >= 0 && cx + 1 < ROAD_WIDTH && field[baseY - 2][cx + 1]))
        ok = false;
      if (!(baseY - 3 >= 0 && field[baseY - 3][cx]))
        ok = false;
      if (ok)
        return cx;
    }
    return null;
  }

  private String findOpponentCarKey(boolean[][] field, int x, int y) {
    // Ищем центр машины соперника по шаблону 1-3-1-3-1 (сверху вниз)
    // Проверяем, является ли данная клетка частью машины с центром в разных
    // позициях
    for (int cy = Math.max(0, y - 4); cy <= Math.min(field.length - 5, y); cy++) {
      for (int cx = Math.max(0, x - 1); cx <= Math.min(ROAD_WIDTH - 1, x + 1); cx++) {
        if (isOpponentCarAt(field, cx, cy) && isPartOfOpponentCar(x, y, cx, cy)) {
          // Используем центр машины для уникального ключа каждой машины
          return "car_" + cx + "_" + cy;
        }
      }
    }
    return "car_" + x + "_" + y; // Fallback
  }

  private boolean isOpponentCarAt(boolean[][] field, int cx, int cy) {
    // Проверяем шаблон машины соперника 1-3-1-3-1 с центром в (cx, cy)
    if (cy < 0 || cy + 4 >= field.length || cx < 0 || cx >= ROAD_WIDTH)
      return false;

    return field[cy][cx] && // верх
        (cx - 1 >= 0 && field[cy + 1][cx - 1]) && field[cy + 1][cx]
        && (cx + 1 < ROAD_WIDTH && field[cy + 1][cx + 1]) && // 2-я строка
        field[cy + 2][cx] && // 3-я строка
        (cx - 1 >= 0 && field[cy + 3][cx - 1]) && field[cy + 3][cx]
        && (cx + 1 < ROAD_WIDTH && field[cy + 3][cx + 1]) && // 4-я строка
        field[cy + 4][cx]; // низ
  }

  private boolean isPartOfOpponentCar(int x, int y, int cx, int cy) {
    // Проверяем, является ли клетка (x,y) частью машины с центром (cx,cy)
    if (y == cy && x == cx)
      return true; // верх
    if (y == cy + 1 && (x == cx - 1 || x == cx || x == cx + 1))
      return true; // 2-я строка
    if (y == cy + 2 && x == cx)
      return true; // 3-я строка
    if (y == cy + 3 && (x == cx - 1 || x == cx || x == cx + 1))
      return true; // 4-я строка
    if (y == cy + 4 && x == cx)
      return true; // низ
    return false;
  }

  @Override
  protected void paintComponent(Graphics g) {
    super.paintComponent(g);
    // Пиксельный рендеринг текста (без сглаживания)
    if (g instanceof java.awt.Graphics2D) {
      java.awt.Graphics2D g2 = (java.awt.Graphics2D) g;
      g2.setRenderingHint(java.awt.RenderingHints.KEY_TEXT_ANTIALIASING,
          java.awt.RenderingHints.VALUE_TEXT_ANTIALIAS_OFF);
      g2.setRenderingHint(java.awt.RenderingHints.KEY_ANTIALIASING,
          java.awt.RenderingHints.VALUE_ANTIALIAS_OFF);
    }
    if (showGameMenu) {
      drawGameMenu(g);
      return;
    }
    if (state == null) {
      // Показываем универсальный экран загрузки
      g.setColor(Color.BLACK);
      g.fillRect(0, 0, getWidth(), getHeight());

      // Название игры
      g.setColor(new Color(255, 215, 0));
      g.setFont(new Font("DialogInput", Font.BOLD, 48));
      String gameName = (selectedGame == 1) ? "RACING" : (selectedGame == 2) ? "TETRIS" : "SNAKE";
      FontMetrics fm = g.getFontMetrics();
      int textWidth = fm.stringWidth(gameName);
      int textX = (getWidth() - textWidth) / 2;
      int textY = getHeight() / 2 - 50;
      g.drawString(gameName, textX, textY);

      // Текст загрузки
      g.setColor(Color.WHITE);
      g.setFont(new Font("DialogInput", Font.PLAIN, 18));
      String loadingText = "Initializing game...";
      fm = g.getFontMetrics();
      textWidth = fm.stringWidth(loadingText);
      textX = (getWidth() - textWidth) / 2;
      textY += 60;
      g.drawString(loadingText, textX, textY);

      g.setFont(new Font("DialogInput", Font.PLAIN, 14));
      String instructionText = "Press ENTER to start when ready";
      fm = g.getFontMetrics();
      textWidth = fm.stringWidth(instructionText);
      textX = (getWidth() - textWidth) / 2;
      textY += 40;
      g.drawString(instructionText, textX, textY);

      // Copyright
      g.setColor(Color.GRAY);
      g.setFont(new Font("DialogInput", Font.PLAIN, 12));
      String copyrightText = "©directpi";
      fm = g.getFontMetrics();
      textWidth = fm.stringWidth(copyrightText);
      textX = (getWidth() - textWidth) / 2;
      textY += 50;
      g.drawString(copyrightText, textX, textY);
      return;
    }

    // Если игра не запущена, показываем простой стартовый экран
    if (!gameStarted && !gameOver) {
      g.setColor(Color.BLACK);
      g.fillRect(0, 0, getWidth(), getHeight());

      // Название игры
      g.setColor(new Color(255, 215, 0));
      g.setFont(new Font("DialogInput", Font.BOLD, 48));
      String gameName = (selectedGame == 1) ? "RACING" : (selectedGame == 2) ? "TETRIS" : "SNAKE";
      FontMetrics fm = g.getFontMetrics();
      int textWidth = fm.stringWidth(gameName);
      int textX = (getWidth() - textWidth) / 2;
      int textY = getHeight() / 2 - 50;
      g.drawString(gameName, textX, textY);

      // Инструкция по запуску
      g.setColor(Color.WHITE);
      g.setFont(new Font("DialogInput", Font.PLAIN, 18));
      String startText = "Press ENTER to start";
      fm = g.getFontMetrics();
      textWidth = fm.stringWidth(startText);
      textX = (getWidth() - textWidth) / 2;
      textY += 60;
      g.drawString(startText, textX, textY);

      // Copyright
      g.setColor(Color.GRAY);
      g.setFont(new Font("DialogInput", Font.PLAIN, 12));
      String copyrightText = "©directpi";
      fm = g.getFontMetrics();
      textWidth = fm.stringWidth(copyrightText);
      textX = (getWidth() - textWidth) / 2;
      g.drawString(copyrightText, textX, getHeight() - 40);
      return;
    }
    boolean[][] field = state.getField();

    // Для Racing используем расширенное поле с визуальными краями (14 столбцов)
    // Для Tetris и Snake используем только игровое поле (10 столбцов)
    int displayCols = (selectedGame == 1) ? TOTAL_COLS : ROAD_WIDTH;
    int edgeOffset = (selectedGame == 1) ? EDGE_COLS : 0; // смещение для краёв дороги

    // Квадратные клетки, рассчитываем от высоты
    int cellH = getHeight() / TOTAL_ROWS;
    int cellW = cellH;
    int gridWidthPx = displayCols * cellW;
    int gridHeightPx = TOTAL_ROWS * cellH;

    // Центрирование игрового поля по горизонтали с учетом сайдбара
    int totalContentWidth = gridWidthPx + 16 + SIDEBAR_WIDTH_PX; // игровое поле + отступ + сайдбар
    int originX = Math.max(10, (getWidth() - totalContentWidth) / 2); // центрируем, но не меньше
                                                                      // 10px от края
    int originY = (getHeight() - gridHeightPx) / 2; // центрирование по вертикали
    int sidebarX = originX + gridWidthPx + 16;

    // Фон дорожного полотна
    g.setColor(new Color(20, 20, 20));
    g.fillRect(originX, originY, gridWidthPx, gridHeightPx);

    // Рисуем визуальные края дороги только для Racing
    if (selectedGame == 1) {
      // 2 левых и 2 правых колонки
      // 1-й и 14-й столбцы (ex=0) - цвет основной дороги (не перерисовываем)
      // 2-й и 13-й столбцы (ex=1) - чередование: 2 ячейки выделенные, 2 ячейки цвет
      // дороги
      for (int y = 0; y < TOTAL_ROWS; y++) {
        // Для 2-го и 13-го столбцов: чередование каждые 2 ячейки
        boolean isHighlighted = ((y + edgePhase) % 4) < 2; // 2 выделенные, 2 обычные

        // Левый край: только 2-й столбец (ex=1)
        if (isHighlighted) {
          g.setColor(new Color(80, 80, 80)); // Выделенный цвет
          int px = originX + 1 * cellW; // 2-й столбец
          int py = originY + y * cellH;
          g.fillRect(px, py, cellW - 1, cellH - 1);
        }
        // 1-й столбец остается цветом основной дороги (не перерисовываем)

        // Правый край: только 13-й столбец
        if (isHighlighted) {
          g.setColor(new Color(80, 80, 80)); // Выделенный цвет
          int px = originX + (TOTAL_COLS - 2) * cellW; // 13-й столбец
          int py = originY + y * cellH;
          g.fillRect(px, py, cellW - 1, cellH - 1);
        }
        // 14-й столбец остается цветом основной дороги (не перерисовываем)
      }
    }

    // Разметка (линиями) для игрового поля
    g.setColor(new Color(50, 50, 50));

    // Вертикальные линии - для Racing 14 столбцов, для Tetris/Snake 10 столбцов
    for (int x = 0; x <= displayCols; x++) {
      int gx = originX + x * cellW;
      g.drawLine(gx, originY, gx, originY + gridHeightPx);
    }

    // Горизонтальные линии для всех строк
    for (int y = 0; y <= TOTAL_ROWS; y++) {
      int gy = originY + y * cellH;
      g.drawLine(originX, gy, originX + gridWidthPx, gy);
    }

    // Рисуем машины (только для Racing)
    if (selectedGame == 1) {
      Color[] allColors = { Color.BLACK, // 0 = пусто (не используется)
          Color.GREEN, // 1 = игрок
          Color.YELLOW, // 2 = соперник 1
          Color.RED, // 3 = соперник 2
          Color.BLUE // 4 = соперник 3
      };

      // Рисуем все машины используя цвета от сервера
      int[][] colors = state.getColors();
      if (colors != null) {
        for (int y = 0; y < field.length && y < colors.length; y++) {
          for (int x = 0; x < field[y].length && x < colors[y].length; x++) {
            if (!field[y][x])
              continue;

            int colorIndex = colors[y][x];
            if (colorIndex > 0 && colorIndex < allColors.length) {
              g.setColor(allColors[colorIndex]);
              int screenX = originX + (edgeOffset + x) * cellW + 1;
              int screenY = originY + y * cellH + 1;
              g.fillRect(screenX, screenY, cellW - 2, cellH - 2);
            }
          }
        }
      }
    } else {
      // Для Tetris и Snake рисуем простые блоки
      g.setColor(Color.WHITE);
      for (int y = 0; y < field.length; y++) {
        for (int x = 0; x < field[y].length; x++) {
          if (field[y][x]) {
            int screenX = originX + x * cellW + 1; // без смещения для краёв
            int screenY = originY + y * cellH + 1;
            g.fillRect(screenX, screenY, cellW - 2, cellH - 2);
          }
        }
      }
    }

    /*
     * СТАРЫЙ КОД (закомментирован): // Находим центр машины игрока по шаблону
     * 1-3-1-3 в нижних
     * рядах int baseY = TOTAL_ROWS - 1; Integer playerCenterX = null; for (int cx =
     * 0; cx <
     * ROAD_WIDTH; cx++) { boolean ok = true; ok &= (cx - 1 >= 0 && field[baseY][cx
     * - 1]); ok &=
     * (field[baseY][cx]); ok &= (cx + 1 < ROAD_WIDTH && field[baseY][cx + 1]); ok
     * &= (baseY - 1 >=
     * 0 && field[baseY - 1][cx]); ok &= (baseY - 2 >= 0 && cx - 1 >= 0 &&
     * field[baseY - 2][cx -
     * 1]); ok &= (baseY - 2 >= 0 && field[baseY - 2][cx]); ok &= (baseY - 2 >= 0 &&
     * cx + 1 <
     * ROAD_WIDTH && field[baseY - 2][cx + 1]); ok &= (baseY - 3 >= 0 && field[baseY
     * - 3][cx]); if
     * (ok) { playerCenterX = cx; break; } }
     * 
     * final int playerCenter = (playerCenterX == null) ? -1 : playerCenterX;
     * java.util.function.BiPredicate<Integer, Integer> isPlayerCell = (xx, yy) -> {
     * if
     * (playerCenter < 0) return false; if (yy == baseY) return (xx == playerCenter
     * - 1 || xx ==
     * playerCenter || xx == playerCenter + 1); if (yy == baseY - 1) return (xx ==
     * playerCenter); if
     * (yy == baseY - 2) return (xx == playerCenter - 1 || xx == playerCenter || xx
     * == playerCenter
     * + 1); if (yy == baseY - 3) return (xx == playerCenter); return false; };
     * 
     * // 1) Соперники (без смещения) - каждая машина однотонная // Очищаем старые
     * цвета машин,
     * которых больше нет на поле java.util.Set<String> currentCars = new
     * java.util.HashSet<>();
     * 
     * // Сначала собираем все текущие машины for (int y = 0; y < field.length; y++)
     * { for (int x =
     * 0; x < field[y].length; x++) { if (!field[y][x]) continue; if
     * (isPlayerCell.test(x, y))
     * continue; String carKey = findOpponentCarKey(field, x, y);
     * currentCars.add(carKey); } }
     * 
     * // Удаляем цвета машин, которых больше нет
     * persistentCarColors.keySet().retainAll(currentCars);
     * 
     * // Рисуем машины с постоянными цветами for (int y = 0; y < field.length; y++)
     * { for (int x =
     * 0; x < field[y].length; x++) { if (!field[y][x]) continue; if
     * (isPlayerCell.test(x, y))
     * continue;
     * 
     * // Определяем, к какой машине принадлежит клетка (по центру машины) String
     * carKey =
     * findOpponentCarKey(field, x, y); if
     * (!persistentCarColors.containsKey(carKey)) { // Назначаем
     * случайный цвет для новой машины (стабильный по ключу)
     * persistentCarColors.put(carKey, new
     * java.util.Random(carKey.hashCode()).nextInt(3)); }
     * 
     * int colorIndex = persistentCarColors.get(carKey);
     * g.setColor(opponentColors[colorIndex]); int
     * screenX = originX + (EDGE_COLS + x) * cellW + 1; int screenY = originY + y *
     * cellH + 1;
     * g.fillRect(screenX, screenY, cellW - 2, cellH - 2); } }
     * 
     * // 2) Машина игрока: рисуем все её клетки с единым смещением
     * g.setColor(Color.GREEN); //
     * Зелёный цвет игрока int offset = (playerCenter >= 0) ? playerVisualOffset :
     * 0; for (int y =
     * 0; y < field.length; y++) { for (int x = 0; x < field[y].length; x++) { if
     * (!field[y][x])
     * continue; if (!isPlayerCell.test(x, y)) continue; int screenX = originX +
     * (EDGE_COLS + x +
     * offset) * cellW + 1; int screenY = originY + y * cellH + 1;
     * g.fillRect(screenX, screenY,
     * cellW - 2, cellH - 2); } }
     */

    // Правая панель информации - футуристичный дизайн
    // Темный фон с градиентом
    g.setColor(new Color(10, 10, 15));
    g.fillRoundRect(sidebarX - 15, originY - 10, SIDEBAR_WIDTH_PX + 20, gridHeightPx + 20, 12, 12);

    // Светящаяся рамка
    g.setColor(new Color(0, 150, 255, 120));
    g.drawRoundRect(sidebarX - 15, originY - 10, SIDEBAR_WIDTH_PX + 20, gridHeightPx + 20, 12, 12);
    g.setColor(new Color(0, 100, 200, 60));
    g.drawRoundRect(sidebarX - 14, originY - 9, SIDEBAR_WIDTH_PX + 18, gridHeightPx + 18, 11, 11);

    int ty = originY + 25;
    int dy = 35; // Увеличенные отступы

    // Футуристичный шрифт для основной информации
    // Пиксельные/моноширинные шрифты для всего сайдбара
    Font futuristicFont = new Font("DialogInput", Font.BOLD, 18);
    Font controlsFont = new Font("DialogInput", Font.BOLD, 16);
    Font controlsDetailFont = new Font("DialogInput", Font.PLAIN, 14);

    // Основная информация с неоновыми цветами
    g.setFont(futuristicFont);
    g.setColor(new Color(0, 255, 150)); // Неоново-зеленый
    g.drawString("SCORE", sidebarX, ty);
    g.setColor(new Color(255, 255, 255));
    g.drawString(String.valueOf(state.getScore()), sidebarX + 10, ty + 20);
    ty += dy + 10;

    g.setColor(new Color(255, 150, 0)); // Неоново-оранжевый
    g.drawString("HI-SCORE", sidebarX, ty);
    g.setColor(new Color(255, 255, 255));
    g.drawString(String.valueOf(state.getHighScore()), sidebarX + 10, ty + 20);
    ty += dy + 10;

    g.setColor(new Color(0, 200, 255)); // Неоново-синий
    g.drawString("LEVEL", sidebarX, ty);
    g.setColor(new Color(255, 255, 255));
    g.drawString(String.valueOf(state.getLevel()), sidebarX + 10, ty + 20);
    ty += dy + 10;

    // SPEED отображаем только для Racing (gameId=1), для Tetris и Snake это
    // технический параметр
    if (selectedGame == 1) {
      g.setColor(new Color(255, 50, 150)); // Неоново-розовый
      g.drawString("SPEED", sidebarX, ty);
      g.setColor(new Color(255, 255, 255));
      g.drawString(String.valueOf(state.getSpeed()), sidebarX + 10, ty + 20);
      ty += dy + 15;
    } else {
      ty += 5; // Небольшой отступ для Tetris/Snake
    }

    // Следующая фигура (для Tetris)
    if (selectedGame == 2 && state.getNext() != null) {
      g.setFont(futuristicFont);
      g.setColor(new Color(255, 200, 0)); // Золотой цвет
      g.drawString("NEXT", sidebarX, ty);
      ty += 25;

      // Отрисовываем следующую фигуру (4x4)
      boolean[][] next = state.getNext();
      int nextCellSize = 15; // Половина размера обычной клетки (30/2)
      int nextStartX = sidebarX + 20;
      int nextStartY = ty;

      g.setColor(new Color(0, 255, 255)); // Голубой цвет для фигуры
      for (int y = 0; y < 4 && y < next.length; y++) {
        for (int x = 0; x < 4 && x < next[y].length; x++) {
          if (next[y][x]) {
            int px = nextStartX + x * nextCellSize;
            int py = nextStartY + y * nextCellSize;
            g.fillRect(px, py, nextCellSize - 1, nextCellSize - 1);
          }
        }
      }
      ty += 4 * nextCellSize + 15;
    }

    // Статус паузы
    g.setFont(futuristicFont.deriveFont(18f));
    g.setColor(gamePaused ? new Color(255, 50, 50) : new Color(50, 255, 50));
    g.drawString("PAUSE: " + (gamePaused ? "ON" : "OFF"), sidebarX, ty);
    ty += 40;

    // Управление - футуристичный стиль
    g.setFont(controlsFont);
    g.setColor(new Color(100, 200, 255)); // Светло-синий заголовок
    g.drawString(">>> CONTROLS <<<", sidebarX, ty);
    ty += 25;

    g.setFont(controlsDetailFont);
    g.setColor(new Color(200, 200, 255)); // Светло-фиолетовый
    if (selectedGame == 1) {
      // Racing
      g.drawString("◄ ► MOVE", sidebarX + 5, ty);
      ty += 22;
      g.drawString("▲ BOOST", sidebarX + 5, ty);
      ty += 22;
      g.drawString("SPACE PAUSE", sidebarX + 5, ty);
      ty += 22;
      g.drawString("P PAUSE", sidebarX + 5, ty);
      ty += 22;
    } else if (selectedGame == 2) {
      // Tetris
      g.drawString("◄ ► ▼ MOVE", sidebarX + 5, ty);
      ty += 22;
      g.drawString("▲ / SPACE ROTATE", sidebarX + 5, ty);
      ty += 22;
      g.drawString("P PAUSE", sidebarX + 5, ty);
      ty += 22;
    } else {
      // Snake
      g.drawString("◄ ► ▲ ▼ MOVE", sidebarX + 5, ty);
      ty += 22;
      g.drawString("SPACE SPEED UP", sidebarX + 5, ty);
      ty += 22;
      g.drawString("P PAUSE", sidebarX + 5, ty);
      ty += 22;
    }
    g.drawString("ESC EXIT", sidebarX + 5, ty);

    // Экран GAME OVER
    boolean isTetrisGameOver = (selectedGame == 2 && state != null && state.getPauseState() != null
        && state.getPauseState() == 3);
    boolean isSnakeGameOver = (selectedGame == 3 && state != null && state.getPauseState() != null
        && state.getPauseState() == 5);
    if (gameOver || isTetrisGameOver || isSnakeGameOver) {
      // Полупрозрачный фон
      g.setColor(new Color(0, 0, 0, 180));
      g.fillRect(originX, originY, gridWidthPx, gridHeightPx);

      // Текст GAME OVER
      g.setColor(new Color(255, 50, 50));
      g.setFont(g.getFont().deriveFont(Font.BOLD, 32f));
      String gameOverText = "GAME OVER";
      FontMetrics fm = g.getFontMetrics();
      int textWidth = fm.stringWidth(gameOverText);
      int textX = originX + (gridWidthPx - textWidth) / 2;
      int textY = originY + gridHeightPx / 2 - 40;
      g.drawString(gameOverText, textX, textY);

      // Инструкции
      g.setColor(Color.WHITE);
      g.setFont(g.getFont().deriveFont(Font.PLAIN, 16f));
      String restartText = "Press SPACE to restart";
      String exitText = "Press ESC to exit";
      fm = g.getFontMetrics();

      textWidth = fm.stringWidth(restartText);
      textX = originX + (gridWidthPx - textWidth) / 2;
      textY += 50;
      g.drawString(restartText, textX, textY);

      textWidth = fm.stringWidth(exitText);
      textX = originX + (gridWidthPx - textWidth) / 2;
      textY += 25;
      g.drawString(exitText, textX, textY);
    }
  }

  @Override
  public Dimension getPreferredSize() {
    return new Dimension(800, 600);
  }

  public static void main(String[] args) {
    SwingUtilities.invokeLater(() -> {
      JFrame f = new JFrame("BrickGame v3.0 ©directpi");
      f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
      SwingExample panel = new SwingExample();
      panel.parentFrame = f; // Передаём ссылку на окно для обновления заголовка
      f.setContentPane(panel);
      f.setSize(800, 600); // Устанавливаем размер окна
      f.setLocationRelativeTo(null);
      f.setVisible(true);
      f.requestFocus(); // Запрашиваем фокус
      panel.requestFocusInWindow(); // Фокус на панель для обработки клавиш

      // Дополнительная попытка получить фокус через небольшую задержку
      javax.swing.Timer focusTimer = new javax.swing.Timer(100, e -> {
        panel.requestFocusInWindow();
        ((javax.swing.Timer) e.getSource()).stop();
      });
      focusTimer.start();
    });
  }

  private void handleMenuKey(KeyEvent e) {
    switch (e.getKeyCode()) {
      case KeyEvent.VK_UP:
        selectedGame = Math.max(1, selectedGame - 1);
        repaint();
        break;
      case KeyEvent.VK_DOWN:
        selectedGame = Math.min(3, selectedGame + 1);
        repaint();
        break;
      case KeyEvent.VK_ESCAPE:
        // Выход из приложения только из меню
        System.exit(0);
        break;
      case KeyEvent.VK_ENTER:
        // Сброс локального состояния перед стартом новой игры
        System.out.println("[SwingExample] Starting game with selectedGame=" + selectedGame);
        resetLocalStateForNewGame();
        showGameMenu = false;
        try {
          System.out.println("[SwingExample] Calling client.startGame(" + selectedGame + ")");
          client.startGame(selectedGame).join();
          gameStarted = true;
          gameOver = false;
          gamePaused = false;
          updateWindowTitle(); // Обновляем заголовок при запуске игры
          System.out
              .println("[SwingExample] Game started successfully, selectedGame=" + selectedGame);
        } catch (Exception ex) {
          System.err.println("[SwingExample] Failed to start game: " + ex.getMessage());
          ex.printStackTrace();
          showGameMenu = true; // Возвращаемся в меню при ошибке
        }
        break;
    }
  }

  private void resetLocalStateForNewGame() {
    // Очищаем локальную матрицу и служебные флаги, чтобы не тянуть GAME OVER
    // НЕ сбрасываем gameStarted - он будет установлен после вызова
    // client.startGame()
    this.state = new GameState();
    this.state.setField(new boolean[20][10]);
    this.edgePhase = 0;
    this.hold = false;
    this.lastAccelState = false;
    this.playerVisualOffset = 0;
    this.gameOver = false;
    this.gamePaused = false;
    this.seenRunning = false;
  }

  private void updateWindowTitle() {
    if (parentFrame == null)
      return;
    String gameName = "Menu";
    if (!showGameMenu) {
      switch (selectedGame) {
        case 1:
          gameName = "Racing";
          break;
        case 2:
          gameName = "Tetris";
          break;
        case 3:
          gameName = "Snake";
          break;
      }
    }
    parentFrame.setTitle("BrickGame v3.0 - " + gameName + " ©directpi");
  }

  private void goToMenu() {
    // Возврат в меню из игры: не выходим из приложения
    // НЕ вызываем Terminate - сброс произойдет автоматически при следующем
    // startGame()
    System.out.println("[SwingExample] Returning to menu, gameStarted=" + gameStarted);

    showGameMenu = true;
    gameStarted = false;
    gameOver = false;
    gamePaused = false;
    hold = false;
    lastAccelState = false;
    seenRunning = false;
    // Очищаем поле, чтобы не мигало прошлым кадром
    if (state == null)
      state = new GameState();
    state.setField(new boolean[20][10]);
    updateWindowTitle(); // Обновляем заголовок при возврате в меню
    repaint();
  }

  private String[] getGameControls(int gameId) {
    switch (gameId) {
      case 1: // Racing
        return new String[] { "Racing Controls:", "←/→ - Move, ↑ - Accelerate",
            "Space - Pause, P - Pause, ESC - Exit" };
      case 2: // Tetris
        return new String[] { "Tetris Controls:", "←/→/↓ - Move, Space - Rotate",
            "P - Pause, ESC - Exit" };
      case 3: // Snake
        return new String[] { "Snake Controls:", "←/→/↑/↓ - Move, Space - Speed Up",
            "P - Pause, ESC - Exit" };
      default:
        return new String[] { "Select a game to see controls" };
    }
  }

  private void drawGameMenu(Graphics g) {
    g.setColor(Color.BLACK);
    g.fillRect(0, 0, getWidth(), getHeight());

    // Заголовок
    g.setColor(new Color(255, 215, 0)); // Золотой цвет
    g.setFont(new Font("Monospaced", Font.BOLD, 24));
    FontMetrics fm = g.getFontMetrics();
    String title = "BRICK GAME v3.0";
    int titleX = (getWidth() - fm.stringWidth(title)) / 2;
    g.drawString(title, titleX, 80);

    // Подзаголовок
    g.setColor(Color.WHITE);
    g.setFont(new Font("Monospaced", Font.PLAIN, 14));
    fm = g.getFontMetrics();
    String subtitle = "Select Game";
    int subtitleX = (getWidth() - fm.stringWidth(subtitle)) / 2;
    g.drawString(subtitle, subtitleX, 110);

    // Опции игр
    String[] games = { "1. Racing", "2. Tetris", "3. Snake" };
    String[] descriptions = { "Car racing game", "Classic puzzle game", "Snake game" };

    // Унифицированный шрифт пунктов меню
    Font itemFont = new Font("DialogInput", Font.BOLD, 18);
    Font descFont = new Font("DialogInput", Font.PLAIN, 12);
    g.setFont(itemFont);
    fm = g.getFontMetrics(itemFont);

    int baseY = 150;
    int itemTextHeight = fm.getAscent() + fm.getDescent();
    int verticalPadding = Math.max(10, fm.getAscent() / 3); // равный отступ сверху/снизу

    // Высота описания
    FontMetrics descFm = g.getFontMetrics(descFont);
    int descTextHeight = descFm.getAscent() + descFm.getDescent();
    int gapBetweenLines = Math.max(10, descFm.getAscent() / 2); // увеличен зазор

    // Учитываем высоту иконки при расчёте высоты подсветки
    int maxContentHeight = Math.max(ICON_SIZE, itemTextHeight);
    // Подсветка теперь включает иконку/пункт, зазор и описание
    int highlightHeight = maxContentHeight + gapBetweenLines + descTextHeight + verticalPadding * 2;
    int itemSpacing = highlightHeight + 15; // уменьшено расстояние между пунктами

    for (int i = 0; i < games.length; i++) {
      boolean isSelected = (selectedGame == i + 1);

      int rectTop = baseY + i * itemSpacing;
      int rectLeft = 50;
      int rectWidth = getWidth() - 100;

      if (isSelected) {
        // Менее насыщенный цвет выделения
        g.setColor(new Color(60, 160, 90));
        g.fillRect(rectLeft, rectTop, rectWidth, highlightHeight);
        g.setColor(Color.BLACK);
      } else {
        g.setColor(Color.WHITE);
      }

      // Общие метрики и позиционирование
      g.setFont(itemFont);
      fm = g.getFontMetrics(itemFont);
      int itemBaselineY = rectTop + verticalPadding + fm.getAscent();

      // Иконка для каждой игры (уже предварительно масштабирована)
      ImageIcon icon = (i == 0) ? ICON_RACING : (i == 1) ? ICON_TETRIS : ICON_SNAKE;
      int iconWidth = (icon != null) ? icon.getIconWidth() : ICON_SIZE;
      int iconHeight = (icon != null) ? icon.getIconHeight() : ICON_SIZE;

      // Общая ширина: иконка + зазор + текст
      int textWidth = fm.stringWidth(games[i]);
      int gapX = 15; // увеличен зазор между иконкой и текстом
      int totalWidth = iconWidth + gapX + textWidth;

      // Центрируем блок (иконка+текст) относительно подсветки (а не всей панели)
      int centerX = rectLeft + rectWidth / 2;
      int startX = centerX - totalWidth / 2;
      int textX = startX + iconWidth + gapX;

      // Рисуем иконку, выравнивая по вертикали с текстом
      int iconTop = rectTop + verticalPadding + Math.max(0, (maxContentHeight - iconHeight) / 2);
      if (icon != null && icon.getImage() != null) {
        g.drawImage(icon.getImage(), startX, iconTop, this);
      } else {
        // Фолбэк: ASCII-иконки, если ресурс не найден
        g.setFont(new Font("Monospaced", Font.BOLD, 12));
        g.setColor(isSelected ? Color.BLACK : new Color(180, 180, 180));
        String placeholder = (i == 0) ? ">>" : (i == 1) ? "[]" : "~~";
        g.drawString(placeholder, startX, iconTop + 12);
      }

      // Текст пункта, выравнивая по вертикали с иконкой
      g.setFont(itemFont);
      g.setColor(isSelected ? Color.BLACK : Color.WHITE);
      int textBaselineY = rectTop + verticalPadding
          + Math.max(0, (maxContentHeight - itemTextHeight) / 2) + fm.getAscent();
      g.drawString(games[i], textX, textBaselineY);

      // Описание под пунктом (под иконкой и текстом)
      g.setFont(descFont);
      fm = g.getFontMetrics(descFont);
      g.setColor(isSelected ? Color.BLACK : Color.GRAY);
      // Подпись центрируем по горизонтали
      int descWidth = fm.stringWidth(descriptions[i]);
      int descX = centerX - descWidth / 2;
      // Размещаем описание ниже области иконки/текста
      int descY = rectTop + verticalPadding + maxContentHeight + gapBetweenLines + fm.getAscent();
      g.drawString(descriptions[i], descX, descY);

      // Запоминаем прямоугольник клика для мыши
      MENU_RECTS[i].setBounds(rectLeft, rectTop, rectWidth, highlightHeight);
    }

    // Вычисляем позицию сразу после последнего пункта меню
    int afterMenuY = baseY + games.length * itemSpacing + 20;

    // Инструкции по навигации
    g.setColor(Color.YELLOW);
    g.setFont(new Font("Monospaced", Font.PLAIN, 12));
    fm = g.getFontMetrics();
    String instructions = "Use UP/DOWN arrows to select, ENTER to start";
    int instX = (getWidth() - fm.stringWidth(instructions)) / 2;
    g.drawString(instructions, instX, afterMenuY);
    afterMenuY += 25;

    // Управление для выбранной игры
    String[] controls = getGameControls(selectedGame);
    g.setColor(Color.CYAN);
    g.setFont(new Font("Monospaced", Font.PLAIN, 10));
    fm = g.getFontMetrics();
    for (int i = 0; i < controls.length; i++) {
      int controlX = (getWidth() - fm.stringWidth(controls[i])) / 2;
      g.drawString(controls[i], controlX, afterMenuY + i * 12);
    }
    afterMenuY += controls.length * 12 + 15;

    // Copyright теперь в заголовке окна
  }

  private static boolean[][] deepCopy(boolean[][] src) {
    if (src == null)
      return null;
    boolean[][] dst = new boolean[src.length][];
    for (int i = 0; i < src.length; i++) {
      dst[i] = new boolean[src[i].length];
      System.arraycopy(src[i], 0, dst[i], 0, src[i].length);
    }
    return dst;
  }
}
