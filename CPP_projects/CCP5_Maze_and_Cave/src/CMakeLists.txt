cmake_minimum_required(VERSION 3.28)
project(A1_Maze_CPP VERSION 1.0 LANGUAGES CXX)

# ----------------------------- Общие настройки -----------------------------

set(CMAKE_VERBOSE_MAKEFILE ON)
cmake_policy(SET CMP0071 NEW)  # Автоматическая обработка Qt RCC

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

# Тип сборки по умолчанию
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
endif()

message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# ---------------------------- Подключение Qt -------------------------------

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR OFF)

# Принудительно ищем Qt 6 первым
find_package(Qt6 QUIET COMPONENTS Widgets Core Gui)
if(Qt6_FOUND)
    set(QT_VERSION_MAJOR 6)
    message(STATUS "Found Qt 6")
else()
    # Если Qt 6 не найден, ищем Qt 5
    find_package(Qt5 REQUIRED COMPONENTS Widgets Core Gui)
    set(QT_VERSION_MAJOR 5)
    message(STATUS "Found Qt 5")
endif()

message(STATUS "Using Qt version: ${QT_VERSION_MAJOR}")

# -------------------------- Пути и файлы проекта ---------------------------

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)

# Исходные файлы
set(SOURCES
    ${SOURCE_DIR}/main.cpp
    ${SOURCE_DIR}/gui/mainwindow.cpp
    ${SOURCE_DIR}/gui/DungeonView.cpp
    ${SOURCE_DIR}/src/api/API.cpp
    ${SOURCE_DIR}/src/common/Utils.cpp
    # ${SOURCE_DIR}/src/common/TemplateMatrix.tpp
    ${SOURCE_DIR}/src/dungeon/core/Cave.cpp
    ${SOURCE_DIR}/src/dungeon/core/DungeonFactory.cpp
    ${SOURCE_DIR}/src/dungeon/core/Maze.cpp
    ${SOURCE_DIR}/src/dungeon/io/CaveLoader.cpp
    ${SOURCE_DIR}/src/dungeon/io/DungeonLoaderFactory.cpp
    ${SOURCE_DIR}/src/dungeon/io/DungeonSaverFactory.cpp
    ${SOURCE_DIR}/src/dungeon/io/MazeLoader.cpp
    ${SOURCE_DIR}/src/dungeon/io/MazeSaver.cpp
    ${SOURCE_DIR}/src/dungeon/pathfinding/MazeBfsPathfinder.cpp
    ${SOURCE_DIR}/src/dungeon/pathfinding/MLAgent.cpp
)

# Заголовочные файлы
set(HEADERS
    ${SOURCE_DIR}/include/gui/mainwindow.h
    ${SOURCE_DIR}/include/gui/DungeonView.h
    ${SOURCE_DIR}/include/api/API.h
    ${SOURCE_DIR}/include/common/Point.h
    ${SOURCE_DIR}/include/common/TemplateMatrix.h
    ${SOURCE_DIR}/include/common/Utils.h
    ${SOURCE_DIR}/include/dungeon/core/Cave.h
    ${SOURCE_DIR}/include/dungeon/core/Dungeon.h
    ${SOURCE_DIR}/include/dungeon/core/DungeonFactory.h
    ${SOURCE_DIR}/include/dungeon/core/DungeonInfo.h
    ${SOURCE_DIR}/include/dungeon/core/DungeonType.h
    ${SOURCE_DIR}/include/dungeon/core/Maze.h
    ${SOURCE_DIR}/include/dungeon/io/CaveLoader.h
    ${SOURCE_DIR}/include/dungeon/io/DungeonLoader.h
    ${SOURCE_DIR}/include/dungeon/io/DungeonLoaderFactory.h
    ${SOURCE_DIR}/include/dungeon/io/DungeonSaver.h
    ${SOURCE_DIR}/include/dungeon/io/DungeonSaverFactory.h
    ${SOURCE_DIR}/include/dungeon/io/MazeLoader.h
    ${SOURCE_DIR}/include/dungeon/io/MazeSaver.h
    ${SOURCE_DIR}/include/dungeon/pathfinding/MazeBfsPathfinder.h
    ${SOURCE_DIR}/include/dungeon/pathfinding/DungeonPathfinder.h
    ${SOURCE_DIR}/include/dungeon/pathfinding/MLAgent.h
)

set(UI_FILES
    ${SOURCE_DIR}/gui/mainwindow.ui
)

set(QRC_FILES
    ${SOURCE_DIR}/resources/resources.qrc
        test/test.cpp
        test/cave_test.cpp
        test/maze_test.cpp
)

# Исключение тестовых файлов
list(FILTER SOURCES EXCLUDE REGEX ".*[Tt]est.*\\.(cpp|cc|cxx|tpp)")
list(FILTER HEADERS EXCLUDE REGEX ".*[Tt]est.*\\.(h|hpp|tpp)")

# UI и ресурсы
# file(GLOB UI_FILES "${SOURCE_DIR}/resources/*.ui")
# file(GLOB QRC_FILES "${SOURCE_DIR}/resources/*.qrc")

# ----------------------------- Создание цели -------------------------------

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(A1_Maze_CPP
        MANUAL_FINALIZATION
        ${SOURCES}
        ${HEADERS}
        ${UI_FILES}
        ${QRC_FILES}
    )
else()
    add_executable(A1_Maze_CPP
        ${SOURCES}
        ${HEADERS}
        ${UI_FILES}
        ${QRC_FILES}
    )
endif()

# ----------------------------- Подключение Qt ------------------------------

target_link_libraries(A1_Maze_CPP PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
)

# -------------------------- Директории include -----------------------------

target_include_directories(A1_Maze_CPP PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gui
    ${CMAKE_CURRENT_SOURCE_DIR}/include/api
    ${CMAKE_CURRENT_SOURCE_DIR}/include/common
    ${CMAKE_CURRENT_SOURCE_DIR}/include/dungeon
    ${CMAKE_CURRENT_SOURCE_DIR}/include/dungeon/core
    ${CMAKE_CURRENT_SOURCE_DIR}/include/dungeon/io
    ${CMAKE_CURRENT_SOURCE_DIR}/include/dungeon/pathfinding
    ${CMAKE_CURRENT_BINARY_DIR}
)

# --------------------- Завершение сборки для Qt6 ---------------------------

if(QT_VERSION_MAJOR EQUAL 6)
    target_include_directories(A1_Maze_CPP PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}/A1_Maze_CPP_autogen/include
    )
    qt_finalize_executable(A1_Maze_CPP)
endif()
# ---------------------------- .gitignore (опц.) ----------------------------

file(WRITE ${CMAKE_BINARY_DIR}/.gitignore "*")
