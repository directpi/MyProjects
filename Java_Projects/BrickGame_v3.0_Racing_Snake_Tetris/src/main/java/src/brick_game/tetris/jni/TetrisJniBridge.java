/**
 * @file: TetrisJniBridge.java
 * @description: JNI мост для вызова C функций Tetris из BrickGame v1.0
 * @dependencies: libs21_tetris.so, common_file.h
 * @created: 2025-10-05
 */
package src.brick_game.tetris.jni;

/**
 * JNI мост для взаимодействия с нативной библиотекой Tetris. Загружает
 * libtetris_jni.so и
 * предоставляет методы для работы с игрой.
 */
public class TetrisJniBridge {

  private static boolean libraryLoaded = false;
  private static UnsatisfiedLinkError loadError = null;

  /**
   * Ленивая загрузка JNI библиотеки - вызывается только при первом использовании
   */
  private static synchronized void ensureLibraryLoaded() {
    if (libraryLoaded) {
      return; // Уже загружена
    }

    if (loadError != null) {
      throw loadError; // Ранее была ошибка загрузки
    }

    boolean loaded = false;
    System.out.println("[JNI] TetrisJniBridge: Starting library load");

    // Попытка 1: загрузка из java.library.path
    try {
      System.loadLibrary("tetris_jni");
      System.out.println("[JNI] Loaded libtetris_jni.so from system path");
      loaded = true;
    } catch (UnsatisfiedLinkError e) {
      System.out.println("[JNI] Not in system path: " + e.getMessage());
    }

    // Попытка 2: загрузка из ресурсов classpath (/native/libtetris_jni.so)
    if (!loaded) {
      try {
        java.io.InputStream in = TetrisJniBridge.class.getResourceAsStream("/native/libtetris_jni.so");
        if (in != null) {
          java.nio.file.Path tmp = java.nio.file.Files.createTempFile("libtetris_jni", ".so");
          java.nio.file.Files.copy(in, tmp, java.nio.file.StandardCopyOption.REPLACE_EXISTING);
          in.close();
          System.out.println("[JNI] Trying: " + tmp.toAbsolutePath());
          System.load(tmp.toAbsolutePath().toString());
          loaded = true;
        } else {
          System.err.println("[JNI] Resource /native/libtetris_jni.so not found in classpath");
        }
      } catch (Exception e) {
        System.err.println("[JNI] Could not load from resource: " + e.getMessage());
      }
    }

    if (!loaded) {
      loadError = new UnsatisfiedLinkError("Failed to load libtetris_jni.so - Tetris JNI not available. "
          + "java.library.path: " + System.getProperty("java.library.path"));
      System.err.println("[JNI] FAILED: " + loadError.getMessage());
      throw loadError;
    }

    libraryLoaded = true;
    System.out.println("[JNI] Library loaded successfully");
  }

  /**
   * Инициализирует игру Tetris.
   * 
   * @return указатель на GameInfo_t структуру (long)
   */
  public long initGame() {
    ensureLibraryLoaded();
    return initGameNative();
  }

  private native long initGameNative();

  /**
   * Освобождает память, выделенную для игры.
   * 
   * @param gamePtr указатель на GameInfo_t
   */
  public void freeGame(long gamePtr) {
    ensureLibraryLoaded();
    freeGameNative(gamePtr);
  }

  private native void freeGameNative(long gamePtr);

  /**
   * Обновляет состояние игры на основе пользовательского действия.
   * 
   * @param gamePtr указатель на GameInfo_t
   * @param action  код действия (0=Start, 1=Pause, 2=Terminate, 3=Left, 4=Right,
   *                5=Up, 6=Down,
   *                7=Action)
   * @return обновлённое состояние игры
   */
  public GameInfoData updateState(long gamePtr, int action) {
    ensureLibraryLoaded();
    return updateStateNative(gamePtr, action);
  }

  private native GameInfoData updateStateNative(long gamePtr, int action);

  /**
   * Получает текущее состояние игры без обновления.
   * 
   * @param gamePtr указатель на GameInfo_t
   * @return текущее состояние игры
   */
  public GameInfoData getState(long gamePtr) {
    ensureLibraryLoaded();
    return getStateNative(gamePtr);
  }

  private native GameInfoData getStateNative(long gamePtr);
}
