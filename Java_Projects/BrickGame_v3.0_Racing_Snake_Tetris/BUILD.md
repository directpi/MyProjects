# 🔨 Инструкция по сборке и запуску BrickGame v3.0

## Требования

- **JDK 21+** (OpenJDK или Oracle JDK)
- **gcc** (для компиляции Tetris JNI)
- **g++** (для компиляции Snake JNI, C++17)
- **Linux/macOS** (для JNI библиотек Tetris и Snake)

## Быстрый старт

### 1️⃣ Полная сборка проекта (рекомендуется)

```bash
# Собрать JNI библиотеки + собрать проект
./gradlew fullBuild
```

Эта команда выполнит:
1. ✅ Компиляцию JNI библиотек (`buildTetrisJni`)
2. ✅ Копирование библиотек в `resources/native/` (`copyTetrisJniToResources`)
3. ✅ Полную сборку проекта (`build`)

### 2️⃣ Запуск сервера

```bash
# Запустить Spring Boot сервер (порт 8080)
./gradlew bootRun
```

Сервер будет доступен на `http://localhost:8080`

### 3️⃣ Запуск клиентов

#### Swing GUI (Desktop)
```bash
# В отдельном терминале (сервер должен быть запущен!)
./gradlew runSwingExample
```

#### Web GUI
Откройте браузер: `http://localhost:8080/index.html`

#### Console CLI
```bash
# Racing
./gradlew runCliRacing

# Tetris
./gradlew runCliTetris

# Snake (в разработке)
./gradlew runCliSnake
```

---

## Gradle задачи

### Сборка проекта

| Задача | Описание |
|--------|----------|
| `./gradlew fullBuild` | **Полная сборка с JNI** (рекомендуется) |
| `./gradlew build` | Обычная сборка проекта (JNI копируется автоматически) |
| `./gradlew build -x test` | Сборка без тестов |
| `./gradlew clean build` | Чистая пересборка |
| `./gradlew cleanAll` | **Очистка всех исполняемых файлов** (JNI, скрипты, данные) |

### JNI библиотеки

| Задача | Описание |
|--------|----------|
| `./gradlew rebuildAllJni` | **Собрать все JNI библиотеки** (Tetris + Snake) |
| `./gradlew rebuildTetrisJni` | Только Tetris JNI (gcc) |
| `./gradlew rebuildSnakeJni` | Только Snake JNI (g++ C++17) |
| `./gradlew buildTetrisJni` | Компиляция Tetris (без копирования) |
| `./gradlew buildSnakeJni` | Компиляция Snake (без копирования) |

### Консольные (терминальные) версии

| Задача | Описание |
|--------|----------|
| `./gradlew runTerminal` | Собрать + показать инструкцию для запуска |
| `./gradlew buildTerminal` | Собрать консольную версию (Tetris + Snake, ncurses) |

### Запуск приложения

| Задача | Описание |
|--------|----------|
| `./gradlew bootRun` | Запустить Spring Boot сервер |
| `./gradlew runSwingExample` | Запустить Swing GUI клиент |
| `./gradlew runConsoleExample` | Запустить консольный клиент |
| `./gradlew runCliRacing` | CLI для Racing игры |
| `./gradlew runCliTetris` | CLI для Tetris игры |

### Тестирование

| Задача | Описание |
|--------|----------|
| `./gradlew test` | Запустить все тесты |
| `./gradlew test --tests RacingFsmTest` | Запустить конкретный тест |

---

## Консольные (терминальные) версии игр

**BrickGame Terminal** - универсальная консольная программа с меню (C/C++ + ncurses):

### Сборка и запуск
```bash
# Шаг 1: Собрать игру
./gradlew runTerminal

# Шаг 2: Запустить игру (короткая команда)
./run-terminal.sh
```

> 💡 **Совет**: Скрипт `run-terminal.sh` создается автоматически при выполнении `./gradlew runTerminal`.
> Команду легко набрать вручную, если не получается скопировать.
> Для вставки в WSL терминале используй **Shift+Insert** или **правая кнопка мыши**.

### Структура
- **Местоположение**: `src/main/java/src/brick_game/BrickGame_v2.0-1/`
- **Makefile**: управляет сборкой C/C++ кода
- **Зависимости**: ncurses (уже установлен)
- **Результат**: `build/brickgame` (исполняемый файл)

### Архитектура консольной версии
```
s21_brickgame.cpp (main + меню)
    ⬇️
s21_gui.a (GUI + game loops)
    ⬇️
s21_snake.a + s21_tetris_back.a (FSM логика игр)
```

---

## Структура сборки

### Автоматическая сборка JNI

При выполнении `./gradlew build` или `./gradlew bootRun` JNI библиотеки **автоматически копируются** из исходников в resources:

#### Tetris (C)
```
src/main/java/src/brick_game/BrickGame_v1.0-1/
  ├── tetris_jni.c                    # JNI C код
  ├── brick_game/tetris/s21_tetris_back.c
  └── libtetris_jni.so                # ❌ ВРЕМЕННЫЙ файл (игнорируется git)

                    ⬇️ copyTetrisJniToResources

src/main/resources/native/
  └── libtetris_jni.so                # ✅ ФИНАЛЬНЫЙ файл (включен в JAR)
```

#### Snake (C++)
```
src/main/java/src/brick_game/BrickGame_v2.0-1/
  ├── snake_jni.cpp                   # JNI C++ код
  ├── brick_game/snake/model/s21_model.cpp
  ├── brick_game/snake/controller/s21_controller.cpp
  └── libsnake_jni.so                 # ❌ ВРЕМЕННЫЙ файл (игнорируется git)

                    ⬇️ copySnakeJniToResources

src/main/resources/native/
  └── libsnake_jni.so                 # ✅ ФИНАЛЬНЫЙ файл (включен в JAR)
```

Gradle сам определяет, нужна ли пересборка (up-to-date check).

### Принудительная пересборка JNI

Если изменили C код и хотите пересобрать:

```bash
# Вариант 1: Через fullBuild
./gradlew fullBuild

# Вариант 2: Через rebuildTetrisJni
./gradlew rebuildTetrisJni
./gradlew bootRun
```

---

## Типичные проблемы

### ❌ `libtetris_jni.so: cannot open shared object file`

**Причина:** JNI библиотека не собрана или не скопирована.

**Решение:**
```bash
./gradlew rebuildTetrisJni
./gradlew bootRun
```

### ❌ `gcc: command not found`

**Причина:** Не установлен компилятор C.

**Решение (Ubuntu/Debian):**
```bash
sudo apt-get install build-essential
```

**Решение (macOS):**
```bash
xcode-select --install
```

### ❌ `JAVA_HOME is not set`

**Причина:** Переменная окружения JAVA_HOME не установлена.

**Решение (Linux):**
```bash
export JAVA_HOME=/usr/lib/jvm/java-21-openjdk-amd64
```

**Решение (macOS):**
```bash
export JAVA_HOME=$(/usr/libexec/java_home -v 21)
```

### ❌ Server port 8080 already in use

**Причина:** Порт 8080 уже занят.

**Решение:**
```bash
# Найти процесс
lsof -i :8080

# Убить процесс
kill -9 <PID>
```

---

## Разработка

### Изменение C кода Tetris

1. Отредактируйте файлы:
   - `src/main/java/src/brick_game/BrickGame_v1.0-1/tetris_jni.c`
   - `src/main/java/src/brick_game/BrickGame_v1.0-1/brick_game/tetris/s21_tetris_back.c`

2. Пересоберите JNI:
   ```bash
   ./gradlew rebuildTetrisJni
   ```

3. Перезапустите сервер:
   ```bash
   ./gradlew bootRun
   ```

### Добавление новой JNI библиотеки (например, Snake)

1. Создайте задачу в `build.gradle.kts` по аналогии с `buildTetrisJni`
2. Создайте задачу копирования по аналогии с `copyTetrisJniToResources`
3. Обновите `fullBuild` чтобы включить новую библиотеку

---

## Полезные ссылки

- [Документация проекта](docs/Project.md)
- [Трекер задач](docs/Tasktracker.md)
- [Дневник разработки](docs/Diary.md)
- [Changelog](docs/changelog.md)
- [JNI Integration Guide](docs/JNI_Integration_Guide.md)

