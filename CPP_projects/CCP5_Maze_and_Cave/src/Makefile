SHELL = /bin/sh

CXX := g++
CXXFLAGS := -Wall -Wextra -Werror -std=c++20
LFLAGS := -lpthread -lgtest -pthread -lstdc++ -lm
GFLAGS := -fprofile-arcs -ftest-coverage
BUILD_DIR := ../build
#QT5_QMAKE = /usr/lib/qt5/bin/qmake
DIST_FILES := $(wildcard files/*.txt) \
              $(wildcard gui/*.cpp gui/*.pro gui/*.ui) \
              $(wildcard include/) \
              $(wildcard resources/) \
              $(wildcard src/) \
              $(wildcard test/) \
              $(wildcard web/web_frontend/public/ web/web_frontend/src/ web/web_frontend/.gitignore web/web_frontend/*.json web/web_frontend/*.md) \
              $(wildcard web/web_server/) \
              $(wildcard main.cpp Makefile CMakeLists.txt readme.md)

ifeq ($(QT_VERSION),)
    # Проверяем, есть ли qmake6
    ifneq ($(shell which qmake6 2>/dev/null),)
        QT_VERSION = 6
    # Если нет qmake6, проверяем версию через qmake
    else ifneq ($(shell qmake --version 2>/dev/null | grep -q "Qt version 5" && echo "found"),)
        QT_VERSION = 5
    # Проверяем, может быть qmake указывает на Qt6
    else ifneq ($(shell qmake --version 2>/dev/null | grep -q "Qt version 6" && echo "found"),)
        QT_VERSION = 6
    else
        QT_VERSION = 6
    endif
endif

ifeq ($(QT_VERSION), 6)
    QMAKE_CMD = qmake6
else
    QMAKE_CMD = qmake
endif

ifeq ($(shell which $(QMAKE_CMD) 2>/dev/null),)
    $(error Команда $(QMAKE_CMD) не найдена. Установите Qt$(QT_VERSION))
endif

all: maze
.PHONY: all

maze:
	cd gui/ && $(QMAKE_CMD) desktop.pro && make && make clean && mv maze ../
	@make clean
web_start:
	@if [ ! -d "web/web_frontend/node_modules" ]; then \
		(cd web/web_frontend && npm install); \
	fi
	@./maze --web & echo $$! > .backend.pid
	@(cd web/web_frontend && npm start) & echo $$! > .frontend.pid
	@tail -f /dev/null
web_stop:
	@if [ -f .backend.pid ]; then \
		echo "Stopping backend server (PID: $(shell cat .backend.pid))..."; \
		kill $(shell cat .backend.pid); \
		rm .backend.pid; \
	else \
		echo "Backend server not running (no .backend.pid file found)."; \
	fi
	@if [ -f .frontend.pid ]; then \
		echo "Stopping frontend server (PID: $(shell cat .frontend.pid))..."; \
		kill $(shell cat .frontend.pid); \
		rm .frontend.pid; \
	else \
		echo "Frontend server not running (no .frontend.pid file found)."; \
	fi
.PHONY: maze

tests:
	cd gui/ && $(QMAKE_CMD) desktop.pro "CONFIG+=test" && make && make clean && mv tests ../
	./tests
	@make clean
.PHONY: tests

gcov_report:
	@echo "🧹 Полная очистка..."
	@rm -rf report/ coverage.info coverage_filtered.info
	@make clean
	
	@echo "🔨 Пересборка тестов..."
	@cd gui/ && $(QMAKE_CMD) desktop.pro "CONFIG+=test" && make clean && make && mv tests ../
	
	@echo "🧪 Запуск тестов..."
	./tests
	
	@echo "📊 Сбор данных покрытия..."
	@lcov --rc branch_coverage=1 \
	     --capture \
	     --directory . \
	     --output-file coverage.info \
	     --no-external \
	     --ignore-errors inconsistent,mismatch,source,empty,negative,unused,corrupted
	
	# Проверяем, что coverage.info создан
	@if [ ! -f "coverage.info" ]; then \
		echo "❌ Ошибка: coverage.info не создан"; \
		exit 1; \
	fi
	
	@echo "🎯 Фильтрация данных..."
	@lcov --rc branch_coverage=1 \
	     --extract coverage.info \
	         "*/src/dungeon/core/Cave.cpp" \
	         "*/src/dungeon/core/Maze.cpp" \
	         "*/src/dungeon/pathfinding/MLAgent.cpp" \
	         "*/src/dungeon/pathfinding/MazeBfsPathfinder.cpp" \
	     --output-file coverage_filtered.info
	
	@echo "📈 Генерация отчёта..."
	@genhtml --rc branch_coverage=1 \
	        coverage_filtered.info \
	        --output-directory report
	
	@echo "✅ Отчёт готов: file://$(shell pwd)/report/index.html"
	
# gcov_report:
# 	@rm -rf report/ coverage.info coverage_filtered.info
# 	@cd gui/ && $(QMAKE_CMD) desktop.pro "CONFIG+=test" && make && mv tests ../
# 	./tests
# 	@if [ -z "$$(find gui/obj -name "*.gcno" | head -1)" ]; then \
# 		echo "❌ Ошибка: не найдено .gcno файлов"; \
# 		exit 1; \
# 	fi
# 	@lcov --rc branch_coverage=1 \
# 	     --capture \
# 	     --directory . \
# 	     --output-file coverage.info \
# 	     --no-external \
# 	     --gcov-tool \
# 	     --ignore-errors version,mismatch,inconsistent \
# 	     > /dev/null 2>&1
# 	@lcov --rc branch_coverage=1 \
# 	     --extract coverage.info \
# 	         "*/src/dungeon/core/Cave.cpp" \
# 	         "*/src/dungeon/core/Maze.cpp" \
# 	         "*/src/dungeon/pathfinding/MLAgent.cpp" \
# 	         "*/src/dungeon/pathfinding/MazeBfsPathfinder.cpp" \
# 	     --output coverage_filtered.info \
# 	     > /dev/null 2>&1
# 	@genhtml --rc branch_coverage=1 \
# 	        -o report \
# 	        coverage_filtered.info \
# 	        > /dev/null 2>&1
# 	@echo "✅ Отчёт готов: file://$(shell pwd)/report/index.html"
# 	@-make clean

run:
	./maze
.PHONY: run

install:
	mkdir -p ~/A1_Maze_CPP_by_HMD/
	install maze ~/A1_Maze_CPP_by_HMD/
.PHONY: install

uninstall:
	rm ~/A1_Maze_CPP_by_HMD/maze
	rmdir ~/A1_Maze_CPP_by_HMD/
.PHONY: uninstall

clean:
	rm -rf out/*.* gui/moc gui/obj gui/rcc gui/ui gui/.qmake.stash gui/Makefile \
	*.gcov *.info
.PHONY: clean

dvi:
	open readme.md
.PHONY: dvi

dist:
	@tar -czf A1_Maze_CPP.tar.gz --transform 's,^\./,A1_Maze_CPP/,' $(DIST_FILES)
.PHONY: dist

 clang:
	clang-format -i include/api/*.h include/common/*.h include/dungeon/core/*.h \
	include/dungeon/io/*.h include/dungeon/pathfinding/*.h include/gui/*.h gui/*.cpp \
	include/gui/*.h src/api/*.cpp src/common/*.cpp src/dungeon/core/*.cpp src/dungeon/io/*.cpp \
	src/dungeon/pathfinding/*.cpp main.cpp test/*.*
 .PHONY: clang

 clang_check:
	clang-format -n include/api/*.h include/common/*.h include/dungeon/core/*.h \
	include/dungeon/io/*.h include/dungeon/pathfinding/*.h include/gui/*.h gui/*.cpp \
	include/gui/*.h src/api/*.cpp src/common/*.cpp src/dungeon/core/*.cpp src/dungeon/io/*.cpp \
	src/dungeon/pathfinding/*.cpp main.cpp test/*.*
 .PHONY: clang_check

dependencies:
	sudo apt update
	sudo apt install nodejs npm
	sudo apt install qt6-declarative-dev
	sudo apt install libqt6openglwidgets6-dev
	sudo apt install libqt6opengl6-dev
#	git clone https://github.com/linux-test-project/lcov.git
#	cd lcov
#	sudo make install
.PHONY: dependencies

# VPATH указывает make, где искать файлы, если они не найдены в текущем каталоге
VPATH := src/api:src/common:src/dungeon/core:src/dungeon/io:src/dungeon/pathfinding:.
