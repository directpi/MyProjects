/**
 * @file: SnakeJniBridge.java
 * @description: JNI bridge для интеграции C++ логики Snake
 * @dependencies: libsnake_jni.so, GameInfoData
 * @created: 2025-10-06
 */
package src.brick_game.snake.jni;

public class SnakeJniBridge {
    private static boolean libraryLoaded = false;
    private static UnsatisfiedLinkError loadError = null;

    private static synchronized void ensureLibraryLoaded() {
        if (libraryLoaded) {
            return;
        }
        if (loadError != null) {
            throw loadError;
        }

        boolean loaded = false;
        System.out.println("[JNI] SnakeJniBridge: Starting library load");

        // Попытка 1: загрузка из java.library.path
        try {
            System.loadLibrary("snake_jni");
            System.out.println("[JNI] Loaded libsnake_jni.so from system path");
            loaded = true;
        } catch (UnsatisfiedLinkError e) {
            System.out.println("[JNI] Not in system path: " + e.getMessage());
        }

        // Попытка 2: загрузка из ресурсов classpath
        if (!loaded) {
            try (java.io.InputStream in = SnakeJniBridge.class.getResourceAsStream("/native/libsnake_jni.so")) {
                if (in == null) {
                    throw new UnsatisfiedLinkError(
                            "Resource /native/libsnake_jni.so not found in classpath");
                }
                java.io.File tempFile = java.io.File.createTempFile("libsnake_jni", ".so");
                tempFile.deleteOnExit(); // Удалить временный файл при выходе JVM

                try (java.io.FileOutputStream out = new java.io.FileOutputStream(tempFile)) {
                    byte[] buffer = new byte[1024];
                    int readBytes;
                    while ((readBytes = in.read(buffer)) != -1) {
                        out.write(buffer, 0, readBytes);
                    }
                }
                System.load(tempFile.getAbsolutePath());
                System.out.println("[JNI] ✅ Loaded from classpath resource: " + tempFile.getAbsolutePath());
                loaded = true;
            } catch (Exception e) {
                System.err.println("[JNI] Could not load from classpath resource: " + e.getMessage());
            }
        }

        if (!loaded) {
            loadError = new UnsatisfiedLinkError("Failed to load libsnake_jni.so - Snake JNI not available. "
                    + "java.library.path: " + System.getProperty("java.library.path"));
            System.err.println("[JNI] FAILED: " + loadError.getMessage());
            throw loadError;
        }

        libraryLoaded = true;
        System.out.println("[JNI] Library loaded successfully");
    }

    // Native методы
    private native long initGameNative();

    private native void freeGameNative(long gamePtr);

    private native GameInfoData updateStateNative(long gamePtr, int actionId, boolean hold);

    // Публичные методы с ленивой загрузкой
    public long initGame() {
        ensureLibraryLoaded();
        return initGameNative();
    }

    public void freeGame(long gamePtr) {
        ensureLibraryLoaded();
        freeGameNative(gamePtr);
    }

    public GameInfoData updateState(long gamePtr, int actionId, boolean hold) {
        ensureLibraryLoaded();
        return updateStateNative(gamePtr, actionId, hold);
    }
}
