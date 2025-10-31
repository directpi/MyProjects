PROJECT_NAME := 3D_VIEWER
TARGET := 3DViewer
VERSION = 2.0

#================================== COMPILER ==================================#
CC = gcc
CPP = g++
#==============================================================================#

#==================================== FLAGS ===================================#
CFLAGS = -Wall -Werror -Wextra -std=c11 -g
CPPFLAGS = -Wall -Wextra -Werror -g --std=c++17 -fdiagnostics-color=always
UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
    CFLAGS += -DLinux
    CHECKFLAGS = -lgtest -lstdc++ -lm 
# -lsubunit
    RM_COMMAND = rm -rf test*.gcda test*.gcno
endif

ifeq ($(UNAME), Darwin)
    CFLAGS += -DDarwin
    CHECKFLAGS = -I/opt/homebrew/include -L/opt/homebrew/lib -lgtest -lm
    RM_COMMAND = rm test*.gcda test*.gcno
endif
GCOVFLAGS = -coverage
VALGRINDFLAGS = --tool=memcheck --leak-check=yes
REPFLAGS = --html --html-details --html-theme=blue --exclude tests
#==============================================================================#

#=============================== DIRECTORY NAMES ==============================#
BUILD_DIR = build
TEST_DIR = tests
DOC_DIR = doc
LOGIC_DIR = logic
MODEL_DIR = 3d_models
DIST_DIR = dist

DOC_CONFIG = $(DOC_DIR)/doc_config
DOC_OUTPUT = $(DOC_DIR)/html/index.html
LATEX_DIR = $(DOC_DIR)/latex
#==============================================================================#

#============================ COLOR SETTINGS ==================================#
COLOR_RESET = \033[0m
COLOR_YELLOW = \033[1m\033[33m\033[3m
#==============================================================================#

#========================= LIST OF FILES AND DIRS =============================#
SOURCES_LOGIC = $(wildcard $(LOGIC_DIR)/*.cpp)
SOURCES_TEST = $(wildcard $(TEST_DIR)/*.cpp)
OBJECTS_LOGIC = $(patsubst $(LOGIC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES_LOGIC))
OBJECTS_TEST = $(patsubst $(TEST_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES_TEST))
#==============================================================================#

#================================ TARGET NAMES ================================#
EXECUTABLE = s21_test
#==============================================================================#

all: clean install

install:
	@echo  "\033[1;33mВыполнение сборки - этап I\033[0m"
	@mkdir -p $(BUILD_DIR)
	@echo  "\033[1;33mВыполнение сборки - этап II\033[0m"
	@qmake6 -o $(BUILD_DIR)/Makefile viewer/viewer.pro
	@echo  "\033[1;33mВыполнение сборки - этап III\033[0m"
	@make -C $(BUILD_DIR)
	@echo  "\033[1;33mВыполнение сборки - этап IV\033[0m"
	@make -C $(BUILD_DIR) clean
	@echo  "\033[1;32mВыполнение сборки - этап V\033[0m"
	@rm -rf $(BUILD_DIR)/.qmake.stash $(BUILD_DIR)/Makefile
	@echo  "\033[1;35mДля запуска Viewer введите:\033[0m" ./build/viewer
#@echo  "\033[1;35mДля запуска тестов добавьте архив files.zip в:\033[0m" src/3d_models и распакуйте его

uninstall:
	@rm -rf $(BUILD_DIR)

dvi: clean docs
	@command -v pdflatex >/dev/null 2>&1 || { echo >&2 "pdflatex не найден. Установите LaTeX."; exit 1; }
	@cd $(DOC_DIR) && pdflatex description.tex
	@mkdir -p $(DOC_DIR)/dvi
	@mv $(DOC_DIR)/description.pdf $(DOC_DIR)/dvi/description.pdf
	@echo  "${COLOR_YELLOW}Описание сгенерировано, проверьте файл:${COLOR_RESET}" dvi/description.pdf
	@cd $(LATEX_DIR) && make
	@mv $(LATEX_DIR)/refman.pdf $(DOC_DIR)/dvi/passport.pdf
	@echo  "${COLOR_YELLOW}Документация (dvi by latex) сгенерирована, проверьте файл:${COLOR_RESET}" dvi/passport.pdf

dist: 
	@rm -rf $(DIST_DIR)
	@echo "Creating a distribution..."
	@mkdir -p $(DIST_DIR)
	@echo "Distribution packaging..."
	@tar --exclude='./tests' --exclude='./dist' -czf $(DIST_DIR)/$(PROJECT_NAME)-$(VERSION).tgz -C ./ .
	@echo "Distribution created"

tests: clean
	@echo  "\033[1;33мСборка тестов\033[0m"
	@mkdir -p $(BUILD_DIR)
	$(CPP) $(CPPFLAGS) $(SOURCES_TEST) $(SOURCES_LOGIC) -o $(EXECUTABLE) $(CHECKFLAGS)
	@echo "\nrunning tests..."
	@./$(EXECUTABLE)
	@echo "\ntests finished..."
	@rm -rf $(EXECUTABLE)

gcov_report:
	@echo "generating gcov report..." 
	@$(CPP) $(CPPFLAGS) $(SOURCES_TEST) $(SOURCES_LOGIC) -o $(EXECUTABLE) $(CHECKFLAGS) $(GCOVFLAGS)
	@./$(EXECUTABLE)
	@lcov --no-external -t "s21_tests" -o s21_tests.info -c -d . -rc lcov_branch_coverage=0 --exclude "$(PWD)/tests/*"
	@genhtml -o report s21_tests.info -rc lcov_branch_coverage=0
	@rm -f *.gcno *.gcda *.info $(EXECUTABLE)
	@open report/index.html

valgrind_test:
	$(CPP) $(CPPFLAGS) $(SOURCES_TEST) $(SOURCES_LOGIC) -o $(EXECUTABLE) $(CHECKFLAGS)
	valgrind -s -q --tool=memcheck --leak-check=yes --track-origins=yes --show-leak-kinds=all --verbose --log-file=valgrind_out.txt ./$(EXECUTABLE)
	@cat valgrind_out.txt
	@rm -rf $(EXECUTABLE) valgrind_out.txt

cppcheck_test:
	cppcheck --enable=all --suppress=missingIncludeSystem *.cpp *.hpp tests/*.cpp tests/*.hpp logic/*.cpp logic/*.hpp

docs: $(DOC_CONFIG)
	@echo "${COLOR_YELLOW}Генерация документации...${COLOR_RESET}"
	@doxygen $(DOC_CONFIG)
	@echo  "${COLOR_YELLOW}Документация (html) сгенерирована. Проверьте файл:${COLOR_RESET}" $(DOC_OUTPUT)

style:
	@echo CLANG CHECKING...
	@cp ../materials/linters/.clang-format .
	@find . -type f \( -name "*.c" -o -name "*.h" -o -name "*.cpp" \) -exec clang-format -n {} \;
	@rm .clang-format

style_replace:
	@cp ../materials/linters/.clang-format .
	@find . -type f \( -name "*.c" -o -name "*.h" -o -name "*.cpp" \) -exec clang-format -i {} \;
	@rm .clang-format
	@echo CLANG FORMATTED

clean:
	@rm -rf $(BUILD_DIR) $(DIST_DIR) $(DOC_DIR)/dvi report/ $(EXECUTABLE) $(DOC_DIR)/html $(DOC_DIR)/latex $(DOC_DIR)/doc doc/*.aux doc/*.log doc/*.pdf valgrind_out.txt *.bmp *.jpeg *.gif

dependencies:
	sudo apt update
	sudo apt install cmake gcc clang gdb build-essential libgtkmm-4.0-dev libglew-dev libglm-dev libtinyobjloader-dev libgtest-dev doxygen qt6-base-dev qtcreator

.PHONY: all install uninstall clean dvi dist tests gcov_report valgrind_test cppcheck_test style docs style_replace dependencies
