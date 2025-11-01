CC = gcc
CPP = g++
STANDART_C=-std=c11
STANDART_CPP=-std=c++17
CFLAGS = -Wall -Wextra -Werror -g
SOURCE = $(wildcard *.tpp) #*.cpp 
SOURCE_TEST = $(wildcard tests/*.cpp)
BACK_SOURCE = $(wildcard brick_game/tetris/*.c) # get list all *.c    $(filter-out brick_game/tetris/tetris.c, ...)
SNAKE_SOURCE = $(wildcard brick_game/snake/model/*.cpp brick_game/snake/controller/*.cpp)
RACING_SOURCE = $(wildcard brick_game/racing/*.cpp)
FRONT_SOURCE = $(wildcard gui/cli/*.cpp)
EXECUTABLE = test
UNAME := $(shell uname)
CLANG_FILE= "../materials/linters/.clang-format"
TARGET_DIRS= . tests/ brickgame/common/ brickgame/snake/model brickgame/snake/controller  brickgame/tetris/ gui/cli/ #gui/desktop/


ifeq ($(UNAME), Linux)
	GFLAGS = -lgtest -lstdc++ -lncurses
endif

ifeq ($(UNAME), Darwin)
	GFLAGS = -I/opt/homebrew/include -L/opt/homebrew/lib -lncurses -lgtest 
endif

# .PHONY all install uninstall clean dvi  dist tests cli desktop launch_cli gcov_report clang cppcheck_test valgrind_test rebuild  

all: gcov_report install
# all, install, uninstall, clean, dvi, dist, tests

install: uninstall cli 
# desktop

uninstall:
	@rm -rf build

cli: s21_tetris_back.a s21_snake.a s21_racing.a s21_gui.a
	@mkdir -p build
	$(CPP) $(STANDART_CPP) s21_gui.a s21_racing.a s21_snake.a s21_tetris_back.a -o build/brickgame -lncurses -lcurl
	@rm s21_tetris_back.a s21_snake.a s21_racing.a s21_gui.a

s21_tetris_back.a:
	$(CC)  $(STANDART_C) -c $(BACK_SOURCE) $(CFLAGS)
	ar -rc s21_tetris_back.a s21_tetris_back.o
	ranlib s21_tetris_back.a
	# @rm -rf *.o

s21_snake.a:
	$(CPP) $(CFLAGS) $(STANDART_CPP) -c $(SNAKE_SOURCE)
	ar -rc s21_snake.a s21_model.o s21_controller.o
	ranlib s21_snake.a
	@rm -rf *.o

s21_racing.a:
	$(CPP) $(CFLAGS) $(STANDART_CPP) -c $(RACING_SOURCE)
	ar -rc s21_racing.a s21_racing_rest_client.o
	ranlib s21_racing.a
	@rm -rf *.o

s21_gui.a:
	$(CPP) $(CFLAGS) $(STANDART_CPP) -c $(FRONT_SOURCE)
	ar -rc s21_gui.a s21_gui.o s21_brickgame_main.o
	ranlib s21_gui.a
	@rm -rf *.o
	
test: clean
	$(CPP) $(CFLAGS) $(SOURCE_TEST) -o test $(GFLAGS)
	./test
	@rm test

gcov_report: clean 
	@$(CPP) $(CFLAGS) $(SOURCE_TEST) -o $(EXECUTABLE) $(GFLAGS) --coverage 
	@./$(EXECUTABLE)
	@lcov --no-external -t "$(EXECUTABLE)" -o $(EXECUTABLE).info -c -d . -rc lcov_branch_coverage=1 --exclude "$(PWD)/tests/*"
	@genhtml -o report $(EXECUTABLE).info -rc lcov_branch_coverage=1
	@rm -rf *.gcno *.gcda *.gcov 

style:
	@for TARGET_DIR in $(TARGET_DIRS); do cp $(CLANG_FILE) $$TARGET_DIR; done
	@for TARGET_DIR in $(TARGET_DIRS); do clang-format -i $$TARGET_DIR/*.tpp; done
	@for TARGET_DIR in $(TARGET_DIRS); do clang-format -i $$TARGET_DIR/*.hpp; done
	@for TARGET_DIR in $(TARGET_DIRS); do rm $$TARGET_DIR/.clang-format; done

clang:
	@for TARGET_DIR in $(TARGET_DIRS); do cp $(CLANG_FILE) $$TARGET_DIR; done
	@for TARGET_DIR in $(TARGET_DIRS); do clang-format -n $$TARGET_DIR/*.с; done
	@for TARGET_DIR in $(TARGET_DIRS); do clang-format -n $$TARGET_DIR/*.h; done
	@for TARGET_DIR in $(TARGET_DIRS); do clang-format -n $$TARGET_DIR/*.сpp; done
	@for TARGET_DIR in $(TARGET_DIRS); do clang-format -n $$TARGET_DIR/*.tpp; done
	@for TARGET_DIR in $(TARGET_DIRS); do clang-format -n $$TARGET_DIR/*.hpp; done
	@for TARGET_DIR in $(TARGET_DIRS); do rm $$TARGET_DIR/.clang-format; done

clean:
	@rm -rf report build
	@rm -rf *.o *.a *.gcno *.gcda *.info $(EXECUTABLE) valgrind_out.txt a.out test *.info s21_brickgame

valgrind_test: 
# clean
# $(CPP) $(CFLAGS) $(SOURCE_TEST) -o test $(GFLAGS)
	valgrind -s -q --tool=memcheck --leak-check=yes --track-origins=yes --show-leak-kinds=all --verbose --log-file=valgrind_out.txt ./s21_brickgame
	cat valgrind_out.txt
	@rm test valgrind_out.txt

cppcheck_test:
	@for TARGET_DIR in $(TARGET_DIRS); do cppcheck --enable=all --suppress=missingIncludeSystem $$TARGET_DIR/*.tpp; done
	@for TARGET_DIR in $(TARGET_DIRS); do cppcheck --enable=all --suppress=missingIncludeSystem $$TARGET_DIR/*.hpp; done 
# CPP = gcPP
# CFLAGS = -Wall -Wextra -Werror -g -std=c11 
# BACK_SOURCE = $(wildcard brick_game/tetris/*.c) # get list all *.c    $(filter-out brick_game/tetris/tetris.c, ...)
# FRONT_SOURCE = $(wildcard gui/cli/*.c)
# SOURCE_TEST = $(wildcard tests/*.c)
# SOURCE_LIBRARY = $(filter-out s21_test.c, $(filter-out test%, $(SOURCE)))
# EXECUTABLE = tetris
# UNAME := $(shell uname)

# ifeq ($(UNAME), Linux)
# 	CFLAGS += -DLinux
# 	GFLAGS = -lcheck -lm -lsubunit
# 	RM_COMMAND = rm -rf test*.gcda test*.gcno
# endif

# ifeq ($(UNAME), Darwin)
# 	CFLAGS += -DDarwin
# 	GFLAGS = -I/opt/homebrew/include -L/opt/homebrew/lib -lcheck -lm
# 	RM_COMMAND = rm test*.gcda test*.gcno
# endif

# .PHONY: all install uninstall clean dvi dist test gcov_report style clang rebuild valgrind_test cppcheck_test

# all: clean install
# 	./tetris

# install: s21_tetris_back.a s21_tetris_front.a
# 	$(CC) $(CFLAGS) s21_tetris.c s21_tetris_back.a s21_tetris_front.a -o tetris -lncurses
# 	@rm s21_tetris_back.a s21_tetris_front.a

# uninstall: clean
# 	@rm -rf tetris

# s21_tetris_back.a:
# 	$(CC) $(CFLAGS) -c $(BACK_SOURCE)
# 	ar -rc s21_tetris_back.a s21_tetris_back.o
# 	ranlib s21_tetris_back.a
# 	# @rm -rf *.o

# s21_tetris_front.a:
# 	$(CC) $(CFLAGS) -c $(FRONT_SOURCE)
# 	ar -rc s21_tetris_front.a s21_tetris_front.o
# 	ranlib s21_tetris_front.a
# 	@rm -rf *.o

# test: clean s21_tetris_back.a
# 	@$(CC) $(CFLAGS) $(SOURCE_TEST) s21_tetris_back.a -o $(EXECUTABLE) $(GFLAGS)
# 	@./$(EXECUTABLE)

# gcov_report: clean
# 	@$(CC) $(CFLAGS) $(SOURCE_TEST) $(BACK_SOURCE) -o $(EXECUTABLE) $(GFLAGS) --coverage 
# 	@./$(EXECUTABLE)
# 	@lcov -t "s21_tests" -o s21_tests.info -c -d . -rc lcov_branch_coverage=1 --exclude "$(PWD)/tests/*"
# 	@genhtml -o report s21_tests.info -rc lcov_branch_coverage=1
# 	@rm -rf *.gcno *.gcda *.gcov

# style: clean
# 	@find . -type f \( -name "*.c" -o -name "*.h" \) -exec clang-format -n -style=file:../materials/linters/.clang-format {} \;

# clang: clean
# 	@find . -type f \( -name "*.c" -o -name "*.h" \) -exec clang-format -i -style=file:../materials/linters/.clang-format {} \;

# clean:
# 	@rm -f  *.out *.o *.a *.gcno *.gcda *.info $(EXECUTABLE) valgrind_out.txt
# 	@rm -rf report $(OBJ_DIR)

# dist:
# 	tar -cvf tetris.tar s21_tetris.c s21_tetris.h brick_game/tetris/* common/* gui/cli/* Makefile 

# dvi:
# 	@open docs/block_scheme_fsm.png

# # rebuild: clean ... test

# valgrind_test: rebuild
# 	valgrind -s -q --tool=memcheck --leak-check=yes --track-origins=yes --show-leak-kinds=all --verbose --log-file=valgrind_out.txt ./$(EXECUTABLE)

# cppcheck_test:
# 	cppcheck --enable=all --suppress=missingIncludeSystem *.c *.h tests/*.c tests/*.h brick_game/tetris/*.c brick_game/tetris/*.h gui/cli/*.c gui/cli/*.h common/*
