CC = g++
CFLAGS = -Wall -Wextra -Werror -std=c++17 -g
OBJ_DIR = objects/
SOURCE = $(wildcard *.cpp ) # get list all *.cpp common/*.cpp
OBJECT = $(patsubst %.cpp, %.o, $(SOURCE))  # replace .cpp -> .o (making object files)
SOURCE_TEST = $(wildcard tests/*.cpp)
SOURCE_LIBRARY = $(filter-out test.cpp, $(filter-out test%, $(SOURCE)))
OBJECT_LIBRARY = $(patsubst %.cpp, %.o, $(SOURCE_LIBRARY))
EXECUTABLE = test
UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
	CFLAGS += -DLinux
	GFLAGS = -lgtest -lstdc++ 
	RM_COMMAND = rm -rf test*.gcda test*.gcno
endif

ifeq ($(UNAME), Darwin)
	CFLAGS += -DDarwin
	GFLAGS = -I/opt/homebrew/include -L/opt/homebrew/-lgtest -L/opt/homebrew/-lgmock
	RM_COMMAND = rm test*.gcda test*.gcno
endif

.PHONY: all  matrix_oop.a test gcov_report style clang clean rebuild valgrind_test cppcheck_test

all: clean  matrix_oop.a #test gcov_report

matrix_oop.a: clean $(OBJECT_LIBRARY)
	@ar rc  matrix_oop.a $(OBJECT_LIBRARY)
	@ranlib  matrix_oop.a
	@rm -rf *.o *.o #common/*.o


test: clean  matrix_oop.a
	@$(CC) $(CFLAGS)   $(SOURCE_TEST) -o $(EXECUTABLE) $(GFLAGS) -L. -l:matrix_oop.a
	@./$(EXECUTABLE)

gcov_report: clean
	@$(CC) $(CFLAGS) $(SOURCE) $(SOURCE_TEST) -o $(EXECUTABLE) $(GFLAGS) --coverage 
	@./$(EXECUTABLE)
	@lcov --no-external -t "tests" -o tests.info -c -d . -rc lcov_branch_coverage=1 --exclude "$(PWD)/tests/*"
	@genhtml -o report tests.info -rc lcov_branch_coverage=1
	@rm -rf *.gcno *.gcda *.gcov

style: clean
	@find . -type f \( -name "*.cpp" -o -name "*.h" \) -exec clang-format -n -style=file:../materials/linters/.clang-format {} \;

clang:
	@find . -type f \( -name "*.cpp" -o -name "*.h" \) -exec clang-format -i -style=file:../materials/linters/.clang-format {} \;


clean:
	@rm -f *.o *.a *.gcno *.gcda *.info $(EXECUTABLE) valgrind_out.txt main
	@rm -rf report $(OBJ_DIR)

rebuild: clean test #matrix_oop.a

valgrind_test: rebuild
	valgrind -s -q --tool=memcheck --leak-check=yes --track-origins=yes --show-leak-kinds=all --verbose --log-file=valgrind_out.txt ./$(EXECUTABLE)

cppcheck_test:
	cppcheck --language=c++ --std=c++17 --enable=all --suppress=missingIncludeSystem *.cpp *.h tests/*.cpp 
