CC = g++
CFLAGS = -Wall -Wextra -Werror -g --std=c++17
SOURCE = $(wildcard *.tpp) #*.cpp 
SOURCE_TEST = $(wildcard tests/*.cpp)
EXECUTABLE = test
UNAME := $(shell uname)
CLANG_FILE= "../materials/linters/.clang-format"
TARGET_DIRS= . tests/ s21_array/ s21_list/ s21_map/ s21_multiset/ s21_queue/ s21_set/ s21_stack/ s21_vector/


ifeq ($(UNAME), Linux)
	GFLAGS = -lgtest -lstdc++ 
endif

ifeq ($(UNAME), Darwin)
	GFLAGS = -I/opt/homebrew/include -L/opt/homebrew/lib -lgtest 
endif

all: clean test

test: clean
	$(CC) $(CFLAGS) $(SOURCE_TEST) -o test $(GFLAGS)
	./test
	@rm test

gcov_report: clean 
	@$(CC) $(CFLAGS) $(SOURCE_TEST) -o $(EXECUTABLE) $(GFLAGS) --coverage 
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
	@for TARGET_DIR in $(TARGET_DIRS); do clang-format -n $$TARGET_DIR/*.tpp; done
	@for TARGET_DIR in $(TARGET_DIRS); do clang-format -n $$TARGET_DIR/*.hpp; done
	@for TARGET_DIR in $(TARGET_DIRS); do rm $$TARGET_DIR/.clang-format; done

clean:
	@rm -rf report
	@rm -rf *.o *.a *.gcno *.gcda *.info $(EXECUTABLE) valgrind_out.txt a.out a.out.dSYM test *.info test.dSYM main main.dSYM

valgrind_test: clean
	$(CC) $(CFLAGS) $(SOURCE_TEST) -o test $(GFLAGS)
	valgrind -s -q --tool=memcheck --leak-check=yes --track-origins=yes --show-leak-kinds=all --verbose --log-file=valgrind_out.txt ./test
	cat valgrind_out.txt
	@rm test valgrind_out.txt

cppcheck_test:
	@for TARGET_DIR in $(TARGET_DIRS); do cppcheck --enable=all --suppress=missingIncludeSystem $$TARGET_DIR/*.tpp; done
	@for TARGET_DIR in $(TARGET_DIRS); do cppcheck --enable=all --suppress=missingIncludeSystem $$TARGET_DIR/*.hpp; done 