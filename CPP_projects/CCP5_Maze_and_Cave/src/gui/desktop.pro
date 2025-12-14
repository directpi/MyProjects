TEMPLATE = app
TARGET = maze

# Версия Qt (автоматически определится, но можно указать явно)
QT += widgets gui core

# Проверяем версию g++
COMPILER_VERSION = $$system(g++ -dumpversion | cut -d. -f1)
message("Detected g++ major version: $$COMPILER_VERSION")

greaterThan(COMPILER_VERSION, 14) {
    DEFINES += GPP_VERSION_15_OR_HIGHER
}

CONFIG += c++20

# Тип сборки
CONFIG += release

# Определения компилятора
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    ../main.cpp \
    mainwindow.cpp \
    DungeonView.cpp \
    ../src/api/API.cpp \
    ../src/common/Utils.cpp \
    ../src/dungeon/core/Cave.cpp \
    ../src/dungeon/core/DungeonFactory.cpp \
    ../src/dungeon/core/Maze.cpp \
    ../src/dungeon/io/CaveLoader.cpp \
    ../src/dungeon/io/DungeonLoaderFactory.cpp \
    ../src/dungeon/io/DungeonSaverFactory.cpp \
    ../src/dungeon/io/MazeLoader.cpp \
    ../src/dungeon/io/MazeSaver.cpp \
    ../src/dungeon/pathfinding/MazeBfsPathfinder.cpp \
    ../src/dungeon/pathfinding/MLAgent.cpp \
    ../web/web_server/web_server.cpp \

HEADERS += \
    ../include/gui/mainwindow.h \
    ../include/gui/DungeonView.h \
    ../include/api/API.h \
    ../include/common/Point.h \
    ../include/common/TemplateMatrix.h \
    ../include/common/Utils.h \
    ../include/dungeon/core/Cave.h \
    ../include/dungeon/core/Dungeon.h \
    ../include/dungeon/core/DungeonFactory.h \
    ../include/dungeon/core/DungeonInfo.h \
    ../include/dungeon/core/DungeonType.h \
    ../include/dungeon/core/Maze.h \
    ../include/dungeon/io/CaveLoader.h \
    ../include/dungeon/io/DungeonLoader.h \
    ../include/dungeon/io/DungeonLoaderFactory.h \
    ../include/dungeon/io/DungeonSaver.h \
    ../include/dungeon/io/DungeonSaverFactory.h \
    ../include/dungeon/io/MazeLoader.h \
    ../include/dungeon/io/MazeSaver.h \
    ../include/dungeon/pathfinding/MazeBfsPathfinder.h \
    ../include/dungeon/pathfinding/DungeonPathfinder.h \
    ../include/dungeon/pathfinding/MLAgent.h \
    ../web/web_server/web_server.h \
    ../web/web_server/libs/httplib.h \
    ../web/web_server/libs/json.hpp

FORMS += \
    mainwindow.ui

RESOURCES += \
    ../resources/resources.qrc

INCLUDEPATH += \
    ../include \
    ../include/gui \
    ../include/api \
    ../include/common \
    ../include/dungeon \
    ../include/dungeon/core \
    ../include/dungeon/io \
    ../include/dungeon/pathfinding

QMAKE_CXXFLAGS += -Wall -Wextra -Werror

# Настройки для разных конфигураций
Release {
    DESTDIR = release
}

Debug {
    DESTDIR = debug
}

# Установка выходного каталога
OBJECTS_DIR = obj
MOC_DIR = moc
RCC_DIR = rcc
UI_DIR = ui

test{

TARGET = tests
QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
QMAKE_LFLAGS += -fprofile-arcs -ftest-coverage

SOURCES =
SOURCES += \
    ../test/test.cpp \
    ../test/cave_test.cpp \
    ../test/maze_test.cpp \
    mainwindow.cpp \
    DungeonView.cpp \
    ../src/api/API.cpp \
    ../src/common/Utils.cpp \
    ../src/dungeon/core/Cave.cpp \
    ../src/dungeon/core/DungeonFactory.cpp \
    ../src/dungeon/core/Maze.cpp \
    ../src/dungeon/io/CaveLoader.cpp \
    ../src/dungeon/io/DungeonLoaderFactory.cpp \
    ../src/dungeon/io/DungeonSaverFactory.cpp \
    ../src/dungeon/io/MazeLoader.cpp \
    ../src/dungeon/io/MazeSaver.cpp \
    ../src/dungeon/pathfinding/MazeBfsPathfinder.cpp \
    ../src/dungeon/pathfinding/MLAgent.cpp \

HEADERS =
HEADERS += \
    ../test/test.h \
    ../include/gui/mainwindow.h \
    ../include/gui/DungeonView.h \
    ../include/api/API.h \
    ../include/common/Point.h \
    ../include/common/TemplateMatrix.h \
    ../include/common/Utils.h \
    ../include/dungeon/core/Cave.h \
    ../include/dungeon/core/Dungeon.h \
    ../include/dungeon/core/DungeonFactory.h \
    ../include/dungeon/core/DungeonInfo.h \
    ../include/dungeon/core/DungeonType.h \
    ../include/dungeon/core/Maze.h \
    ../include/dungeon/io/CaveLoader.h \
    ../include/dungeon/io/DungeonLoader.h \
    ../include/dungeon/io/DungeonLoaderFactory.h \
    ../include/dungeon/io/DungeonSaver.h \
    ../include/dungeon/io/DungeonSaverFactory.h \
    ../include/dungeon/io/MazeLoader.h \
    ../include/dungeon/io/MazeSaver.h \
    ../include/dungeon/pathfinding/MazeBfsPathfinder.h \
    ../include/dungeon/pathfinding/DungeonPathfinder.h \
    ../include/dungeon/pathfinding/MLAgent.h \

    LIBS += -lgtest_main -lgtest -lpthread
    }