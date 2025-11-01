CC = gcc
CFLAGS = -Wall -Wextra -Werror -g -std=c11 
BACK_SOURCE = $(wildcard brick_game/tetris/*.c) # get list all *.c    $(filter-out brick_game/tetris/tetris.c, ...)
FRONT_SOURCE = $(wildcard gui/cli/*.c)
SOURCE_TEST = $(wildcard tests/*.c)
SOURCE_LIBRARY = $(filter-out s21_test.c, $(filter-out test%, $(SOURCE)))
EXECUTABLE = tetris
UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
	CFLAGS += -DLinux
	GFLAGS = -lcheck -lm -lsubunit
	RM_COMMAND = rm -rf test*.gcda test*.gcno
endif

ifeq ($(UNAME), Darwin)
	CFLAGS += -DDarwin
	GFLAGS = -I/opt/homebrew/include -L/opt/homebrew/lib -lcheck -lm
	RM_COMMAND = rm test*.gcda test*.gcno
endif

.PHONY: all install uninstall clean dvi dist test gcov_report style clang rebuild valgrind_test cppcheck_test

all: clean install
	./tetris

install: s21_tetris_back.a s21_tetris_front.a
	$(CC) $(CFLAGS) s21_tetris.c s21_tetris_back.a s21_tetris_front.a -o tetris -lncurses
	@rm s21_tetris_back.a s21_tetris_front.a

uninstall: clean
	@rm -rf tetris

s21_tetris_back.a:
	$(CC) $(CFLAGS) -c $(BACK_SOURCE)
	ar -rc s21_tetris_back.a s21_tetris_back.o
	ranlib s21_tetris_back.a
	# @rm -rf *.o

s21_tetris_front.a:
	$(CC) $(CFLAGS) -c $(FRONT_SOURCE)
	ar -rc s21_tetris_front.a s21_tetris_front.o
	ranlib s21_tetris_front.a
	@rm -rf *.o

test: clean s21_tetris_back.a
	@$(CC) $(CFLAGS) $(SOURCE_TEST) s21_tetris_back.a -o $(EXECUTABLE) $(GFLAGS)
	@./$(EXECUTABLE)

gcov_report: clean
	@$(CC) $(CFLAGS) $(SOURCE_TEST) $(BACK_SOURCE) -o $(EXECUTABLE) $(GFLAGS) --coverage 
	@./$(EXECUTABLE)
	@lcov -t "s21_tests" -o s21_tests.info -c -d . -rc lcov_branch_coverage=1 --exclude "$(PWD)/tests/*"
	@genhtml -o report s21_tests.info -rc lcov_branch_coverage=1
	@rm -rf *.gcno *.gcda *.gcov

style: clean
	@find . -type f \( -name "*.c" -o -name "*.h" \) -exec clang-format -n -style=file:../materials/linters/.clang-format {} \;

clang: clean
	@find . -type f \( -name "*.c" -o -name "*.h" \) -exec clang-format -i -style=file:../materials/linters/.clang-format {} \;

clean:
	@rm -f  *.out *.o *.a *.gcno *.gcda *.info $(EXECUTABLE) valgrind_out.txt s21_tetris
	@rm -rf report $(OBJ_DIR)

dist:
	tar -cvf tetris.tar s21_tetris.c s21_tetris.h brick_game/tetris/* common/* gui/cli/* Makefile 

dvi:
	@open docs/block_scheme_fsm.png

# rebuild: clean ... test

valgrind_test: rebuild
	valgrind -s -q --tool=memcheck --leak-check=yes --track-origins=yes --show-leak-kinds=all --verbose --log-file=valgrind_out.txt ./$(EXECUTABLE)

cppcheck_test:
	cppcheck --enable=all --suppress=missingIncludeSystem *.c *.h tests/*.c tests/*.h brick_game/tetris/*.c brick_game/tetris/*.h gui/cli/*.c gui/cli/*.h common/*
