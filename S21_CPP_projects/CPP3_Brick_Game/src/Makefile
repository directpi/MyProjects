#================================== COMPILER ==================================#
CC = gcc
CPP = g++
#==============================================================================#

#==================================== FLAGS ===================================#
STANDART_C = -std=c11
STANDART_CPP = -std=c++17
CFLAGS = -Wall -Wextra -Werror -g --std=gnu11
CPPFLAGS = -Wall -Wextra -Werror -g --std=c++17 -fdiagnostics-color=always
UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
    GFLAGS = -lgtest -lstdc++ -lncurses
endif
ifeq ($(UNAME), Darwin)
    GFLAGS = -I/opt/homebrew/include -L/opt/homebrew/lib -lncurses -lgtest 
endif
#==============================================================================##

#================================ TARGET NAMES ================================#
CLANG_FILE = "../materials/linters/.clang-format"
EXECUTABLE = s_brickgame
CLI_TARGET = s_brickgame_cli
DESKTOP_TARGET = s_brickgame_desktop
TEST_EXEC = s_test
#==============================================================================#

#=============================== DIRECTORY NAMES ==============================#
BUILD_DIR = build
BUILD_DIR_CLI = $(BUILD_DIR)/cli
BUILD_DIR_DESKTOP = $(BUILD_DIR)/desktop
SNAKE_CONTROLLER_DIR = brick_game/snake/controller
SNAKE_MODEL_DIR = brick_game/snake/model
TETRIS_DIR = brick_game/tetris
CLI_DIR = gui/cli
DESKTOP_DIR = gui/desktop
COMMON_DIR = brick_game/common
TEST_DIR = tests
DVI_DIR = docs
DIST_DIR = dist/brick_game_v2.0
#==============================================================================#

#========================= LIST OF FILES AND DIRS =============================#
TETRIS_SRC = $(wildcard $(TETRIS_DIR)/*.c)
SNAKE_SRC = $(wildcard $(SNAKE_CONTROLLER_DIR)/*.cpp) $(wildcard $(SNAKE_MODEL_DIR)/*.cpp)
CLI_SRC_CPP = $(wildcard $(CLI_DIR)/*.cpp)
SOURCE_TEST = $(wildcard $(TEST_DIR)/*.cpp)

TETRIS_OBJS = $(patsubst $(TETRIS_DIR)/%.c, $(BUILD_DIR_CLI)/%.o, $(TETRIS_SRC))
SNAKE_OBJS = $(patsubst %.cpp, $(BUILD_DIR_CLI)/%.o, $(notdir $(SNAKE_SRC)))
CLI_SRC_CPP_OBJ = $(patsubst $(CLI_DIR)/%.cpp, $(BUILD_DIR_CLI)/%.o, $(CLI_SRC_CPP))

CLI_C_OBJ = $(TETRIS_OBJS)
CLI_CPP_OBJ = $(SNAKE_OBJS) $(CLI_SRC_CPP_OBJ)

DOC_CONFIG = $(DVI_DIR)/doc_config
DOC_OUTPUT = $(DVI_DIR)/html/index.html
LATEX_DIR = $(DVI_DIR)/latex
#==============================================================================#


#================================= MAIN TARGETS ===============================#
.PHONY: all install uninstall clean dvi dist tests cli desktop run_cli run_desktop gcov_report clang style cppcheck_test valgrind_test valgrind_cli rebuild

all: install gcov_report 

install: uninstall desktop cli 

uninstall:
	@rm -rf build


cli: $(BUILD_DIR) $(BUILD_DIR_CLI) $(CLI_CPP_OBJ) $(CLI_C_OBJ)
	$(CPP) $(CPPFLAGS) $(CLI_CPP_OBJ) $(CLI_C_OBJ) $(GFLAGS) -o $(BUILD_DIR_CLI)/$(CLI_TARGET)
	@rm $(CLI_C_OBJ) $(CLI_CPP_OBJ)
	@echo "\033[1;35mcli mode has been built in build/cli\033[0m"
	@echo "\033[1;35mFor run cli game type:\033[0m" make run_cli

$(BUILD_DIR_CLI)/%.o: $(TETRIS_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR_CLI)/%.o: $(SNAKE_MODEL_DIR)/%.cpp
	$(CPP) $(CPPFLAGS) -c $< -o $@

$(BUILD_DIR_CLI)/%.o: $(SNAKE_CONTROLLER_DIR)/%.cpp
	$(CPP) $(CPPFLAGS) -c $< -o $@

$(BUILD_DIR_CLI)/%.o: $(CLI_DIR)/%.cpp 
	$(CPP) $(CPPFLAGS) -c $< -o $@

desktop: $(BUILD_DIR) $(BUILD_DIR_DESKTOP)
	@qmake $(DESKTOP_DIR)/UI_Snake.pro -o $(BUILD_DIR_DESKTOP)
	@make -C $(BUILD_DIR_DESKTOP)
	@echo "\033[1;35mdesktop mode has been built in build/desktop\033[0m"
	@echo "\033[1;35mFor run desktop game type:\033[0m" make run_desktop

run_cli:
	@if [ -f $(BUILD_DIR_CLI)/$(CLI_TARGET) ]; then \
		$(BUILD_DIR_CLI)/$(CLI_TARGET); \
	else \
		$(MAKE) cli; \
		$(BUILD_DIR_CLI)/$(CLI_TARGET); \
	fi

run_desktop:
	@if [ -f $(BUILD_DIR_DESKTOP)/$(DESKTOP_TARGET) ]; then \
		$(BUILD_DIR_DESKTOP)/$(DESKTOP_TARGET); \
	else \
		$(MAKE) cli; \
		$(BUILD_DIR_DESKTOP)/$(DESKTOP_TARGET); \
	fi

test:
	$(CPP) $(CPPFLAGS) $(SOURCE_TEST) $(SNAKE_MODEL_DIR)/s_model.cpp $(SNAKE_CONTROLLER_DIR)/s_controller.cpp $(TETRIS_DIR)/s_tetris_back.c -o $(TEST_EXEC) $(GFLAGS)
	./$(TEST_EXEC)
	@rm $(TEST_EXEC)

gcov_report: clean
	@echo "generating gcov report..." 
	@$(CPP) $(CPPFLAGS) $(SOURCE_TEST) $(SNAKE_MODEL_DIR)/s_model.cpp $(SNAKE_CONTROLLER_DIR)/s_controller.cpp $(TETRIS_DIR)/s_tetris_back.c -o $(TEST_EXEC) $(GFLAGS) --coverage 
	@./$(TEST_EXEC)
	@lcov --no-external -t "$(TEST_EXEC)" -o $(TEST_EXEC).info -c -d . --ignore-errors unused \
			--exclude "$(PWD)/tests/*" \
			--exclude "$(PWD)/brick_game/tetris/*" \
			--exclude "$(PWD)/brick_game/snake/controller/*" \
			--exclude "$(PWD)/gui/cli/*" \
			--exclude "$(PWD)/gui/desktop/*" 
	@genhtml -o report $(TEST_EXEC).info -rc lcov_branch_coverage=1
	@open report/index.html
	@rm -rf *.gcno *.gcda *.gcov 

style:
	@cp $(CLANG_FILE) .
	@find . -type f \( -name "*.c" -o -name "*.h" -o -name "*.cpp" \) -exec clang-format -i {} \;
	@rm .clang-format
	@echo CLANG FORMATTED

clang:
	@cp $(CLANG_FILE) .
	@find . -type f \( -name "*.c" -o -name "*.h" -o -name "*.cpp" \) -exec clang-format -n {} \;
	@rm .clang-format
	@echo CLANG CHECKED. OK

clean:
	@rm -rf report build dist docs/html docs/latex tetris_record_score.txt snake_record_score.txt
	@rm -rf *.o *.a *.gcno *.gcda *.info $(EXECUTABLE) $(TEST_EXEC) valgrind_out.txt
	@echo "cleaning..."

dvi: $(DOC_CONFIG)
	@doxygen $(DOC_CONFIG)

ifeq ($(UNAME), Linux)
	xdg-open $(DVI_DIR)/html/index.html
endif
ifeq ($(UNAME), Darwin)
	open $(DVI_DIR)/html/index.html
endif

dist: clean
	@rm -rf $(DIST_DIR)
	@echo "Creating a distribution..."
	@mkdir -p $(DIST_DIR)
	@echo "Distribution packaging..."
	@tar --exclude='./tests' --exclude='./dist' -czf $(DIST_DIR)/brick_game_v2.0.tar.gz -C ./ .
	@echo "Distribution created"
	
valgrind_cli:
	@if [ -f $(BUILD_DIR_CLI)/$(CLI_TARGET) ]; then \
		valgrind -s -q --tool=memcheck --leak-check=yes --track-origins=yes --show-leak-kinds=all --verbose --log-file=valgrind_out.txt $(BUILD_DIR_CLI)/$(CLI_TARGET); \
	else \
		$(MAKE) cli; \
		valgrind -s -q --tool=memcheck --leak-check=yes --track-origins=yes --show-leak-kinds=all --verbose --log-file=valgrind_out.txt $(BUILD_DIR_CLI)/$(CLI_TARGET); \
	fi
	@cat valgrind_out.txt
	@rm valgrind_out.txt

valgrind_test:
	$(CPP) $(CPPFLAGS) $(SOURCE_TEST) $(SNAKE_MODEL_DIR)/s_model.cpp $(SNAKE_CONTROLLER_DIR)/s_controller.cpp $(TETRIS_DIR)/s_tetris_back.c -o $(TEST_EXEC) $(GFLAGS)
	valgrind -s -q --tool=memcheck --leak-check=yes --track-origins=yes --show-leak-kinds=all --verbose --log-file=valgrind_out.txt ./$(TEST_EXEC)
	@cat valgrind_out.txt
	@rm test valgrind_out.txt

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem --check-level=exhaustive $(SNAKE_MODEL_DIR)/* $(SNAKE_CONTROLLER_DIR)/* $(TETRIS_DIR)/* $(CLI_DIR)/* $(DESKTOP_DIR)/* $(COMMON_DIR)/* $(TEST_DIR)/*

#============================== CREATE DIRECTORIES ============================#
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
$(BUILD_DIR_CLI):
	mkdir -p $(BUILD_DIR_CLI)
$(BUILD_DIR_DESKTOP):
	mkdir -p $(BUILD_DIR_DESKTOP)
#==============================================================================#