# >> INIT SECTION >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
CC = g++
STD = c++20
STYLE = Google
LIBNAME = 3d_viewer
REPORT_NAME = report

FLAGS = -std=$(STD) -Wall -Werror -Wextra -I.
TEST_FLAGS = -lgtest_main -lgtest
COV_FLAGS = -fprofile-arcs -ftest-coverage
CLANG = clang-format -style=$(STYLE)
VALGRIND = valgrind --vgdb=no --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --read-var-info=yes --log-file=$(VALG_FILE)
LIBS = cmake valgrind check libgtest-dev gcovr lcov doxygen build-essential qt6-base-dev

## FILES EXTENSIONS
GCOV_FILES = *.gcov *.gcna *.gcda *.gcno
DOXYFILE = Doxyfile

## DIRECTORIES
SRC_DIR = .
BUILD_DIR = build
DIST_DIR = dist
OBJECTS_DIR = $(BUILD_DIR)/objects
MODEL_DIR = model
VIEW_DIR = view
CONTROLLER_DIR = controller
TESTS_DIR = tests
HEADER_DIR = include
LOG_DIR = logs
COV_DIR = $(BUILD_DIR)/coverage
VALG_FILE = $(LOG_DIR)/RESULT_VALGRIND.txt

## COLORS
RED = \033[31m
GREEN = \033[32m
YELLOW = \033[33m
BLUE = \033[34m
RESET_COLOR = \033[0m
# << INIT SECTION ENDS <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

# >> BUILD SECTION >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
## MODEL BUILDING
MODEL_SOURCES = $(shell find $(MODEL_DIR) -name "*.cpp")
MODEL_OBJECTS = $(patsubst $(MODEL_DIR)/%.cpp, $(OBJECTS_DIR)/%.o, $(MODEL_SOURCES))
MODEL_HEADERS = $(shell find $(MODEL_DIR) -name "*.h")

## VIEW BUILDING
VIEW_SOURCES = $(shell find $(VIEW_DIR) -name "*.cpp")
VIEW_OBJECTS = $(patsubst $(VIEW_DIR)/%.cpp, $(OBJECTS_DIR)/%.o, $(VIEW_SOURCES))
VIEW_HEADERS = $(shell find $(VIEW_DIR) -name "*.h")

## CONTROLLER BUILDING
CONTROLLER_SOURCES = $(shell find $(CONTROLLER_DIR) -name "*.cpp")
CONTROLLER_OBJECTS = $(patsubst $(CONTROLLER_DIR)/%.cpp, $(OBJECTS_DIR)/%.o, $(CONTROLLER_SOURCES))
CONTROLLER_HEADERS = $(shell find $(CONTROLLER_DIR) -name "*.h")

## TESTS BUILDING
TESTS_SOURCES = $(shell find $(TESTS_DIR) -name "*.cpp")
TESTS_OBJECTS = $(patsubst $(TESTS_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(TESTS_SOURCES))
TESTS_HEADERS = $(shell find $(TESTS_DIR) -name "*.h")

INCLUDE_HEADERS = $(shell find include -name "*.h")

MODULES_SOURCES = $(MODEL_SOURCES) $(VIEW_SOURCES) $(CONTROLLER_SOURCES)
MODULES_OBJECTS = $(MODEL_OBJECTS) $(VIEW_OBJECTS) $(CONTROLLER_OBJECTS)
MODULES_HEADERS = $(MODEL_HEADERS) $(VIEW_HEADERS) $(CONTROLLER_HEADERS)

## MODEL COMPILE RULE
$(OBJECTS_DIR)/%.o: $(MODEL_DIR)/%.cpp
	@$(CC) $(FLAGS) -c $< -o $@

## VIEW COMPILE RULE
$(OBJECTS_DIR)/%.o: $(VIEW_DIR)/%.cpp
	@$(CC) $(FLAGS) -c $< -o $@

## CONTROLLER COMPILE RULE
$(OBJECTS_DIR)/%.o: $(CONTROLLER_DIR)/%.cpp
	@$(CC) $(FLAGS) -c $< -o $@

## TESTS COMPILE RULE
$(BUILD_DIR)/%.o: $(TESTS_DIR)/%.cpp
	@$(CC) $(FLAGS) -c $< -o $@
# << BUILD SECTION ENDS <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

# >> MAIN SECTION >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
.PHONY: build_dir all clean test gcov_report dvi dist gui

all: install test

rebuild: uninstall install

## INSTALL PROGRAM
# install: build_dir $(MODULES_OBJECTS)
# 	$(CC) $(FLAGS) $(MODULES_OBJECTS) $(TEST_FLAGS) -o $(BUILD_DIR)/$(LIBNAME)
# 	@echo "$(GREEN)Installation successful:$(RESET_COLOR) $(BLUE)$(BUILD_DIR)/$(LIBNAME)$(RESET_COLOR)"
install: build_dir
	cd build && cmake .. && make

## UNINSTALL PROGRAM
uninstall:
	@echo "$(GREEN)Removing program...$(RESET_COLOR)"
	@if [ "$(shell ls -A $(BUILD_DIR))" ]; then \
		rm -rf $(BUILD_DIR)*; \
		echo "$(GREEN)Uninstallation successful:$(RESET_COLOR) $(BLUE)$(BUILD_DIR)$(RESET_COLOR)"; \
	else \
		echo "$(YELLOW)No files found in:$(RESET_COLOR) $(BLUE)$(BUILD_DIR)$(RESET_COLOR)"; \
	fi

	rm $(LOG_DIR)/tetris_high_score.txt

## RUN PROGRAM


gui:
	./$(BUILD_DIR)/$(LIBNAME)_gui

# The dist target
# Creates distribution packages (typically tarballs or archives) containing all necessary files for app installation
# Automates the packaging process, reducing manual intervention and potential errors
# Verifies that all non-source files in the distribution are up-to-date
## MAKE DISTRIBUTIVE
dist: clean_dist
	@echo "$(GREEN)Creating distributive...$(RESET_COLOR)"
	@if [ ! -d "$(DIST_DIR)" ]; then \
		mkdir -p "$(DIST_DIR)"; \
	fi;
	cp -r $(MODEL_DIR) $(VIEW_DIR) $(CONTROLLER_DIR) $(HEADER_DIR) $(DIST_DIR)
	cp Makefile Doxyfile mainpage.dox $(DIST_DIR)
	tar -czf $(LIBNAME).tar.gz -C $(DIST_DIR) .
	@echo "$(GREEN)Distributive successfully created:$(RESET_COLOR) $(BLUE)$(LIBNAME).tar.gz$(RESET_COLOR)"
	@rm -rf $(DIST_DIR)

## CLEAN DISTRIBUTIVE
clean_dist:
	@echo "$(GREEN)Removing distributive...$(RESET_COLOR)"
	@if [ -d "$(DIST_DIR)" ]; then \
		rm -rf $(DIST_DIR); \
		echo "$(GREEN)Removed directory:$(RESET_COLOR) $(BLUE)$(DIST_DIR)$(RESET_COLOR)"; \
	else \
		echo "$(YELLOW)No directory found:$(RESET_COLOR) $(DIST_DIR)"; \
	fi
	@if [ -f "$(LIBNAME).tar.gz" ]; then \
		rm -f $(LIBNAME).tar.gz; \
		echo "$(GREEN)Removed file:$(RESET_COLOR) $(BLUE)$(LIBNAME).tar.gz$(RESET_COLOR)"; \
	else \
		echo "$(YELLOW)No file found:$(RESET_COLOR) $(BLUE)$(LIBNAME).tar.gz$(RESET_COLOR)"; \
	fi
	@echo "$(GREEN)Distributive cleanup complete!$(RESET_COLOR)"

# The dvi target
# Converts documentation source files into DVI (Device Independent) format
# Helps maintain up-to-date documentation alongside the software
## MAKE DOCUMENTATION
dvi:
	@echo "$(GREEN)Generating documentation...$(RESET_COLOR)"
	@if [ -f $(DOXYFILE) ]; then \
		doxygen $(DOXYFILE); \
		echo "$(GREEN)Documentation generated:$(RESET_COLOR) $(BLUE)docs/html$(RESET_COLOR)"; \
	else \
		echo "$(RED)Error: Doxyfile not found. Run 'doxygen -g' to generate it.$(RESET_COLOR)"; \
	fi

## CLEAN DOCUMENTATION
clean_dvi:
	@echo "$(GREEN)Removing documentation...$(RESET_COLOR)"
	@if [ -d "latex" ]; then \
		rm -rf latex; \
		echo "$(GREEN)Removed directory:$(RESET_COLOR) $(BLUE)latex$(RESET_COLOR)"; \
	else \
		echo "$(YELLOW)No directory found:$(RESET_COLOR) $(BLUE)latex$(RESET_COLOR)"; \
	fi
	@echo "$(GREEN)Documentation cleanup complete!$(RESET_COLOR)"

clean: clean_dist clean_dvi
	rm -rf $(BUILD_DIR) html
	@echo "$(GREEN)Project cleanup complete!$(RESET_COLOR)"
# << MAIN SECTION ENDS <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

# >> TESTS SECTION >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
## RUN TESTS
# filter-out $(OBJECTS_DIR)/brick_game.o, $(MODEL_OBJECTS) utility 
# adds $(MODEL_OBJECTS) w/o brick_game.o file with main()
test: build_dir $(TESTS_OBJECTS) $(MODEL_OBJECTS)
	$(CC) $(FLAGS) $(TESTS_OBJECTS) $(MODEL_OBJECTS) $(TEST_FLAGS) -o $(BUILD_DIR)/test
	./$(BUILD_DIR)/test

## RUN TESTS WITH VALGRIND
valgrind: build_dir $(TESTS_OBJECTS) $(MODEL_OBJECTS)
	$(CC) $(FLAGS) $(TESTS_OBJECTS) $(MODEL_OBJECTS) $(TEST_FLAGS) -o $(BUILD_DIR)/test
	$(VALGRIND) ./$(BUILD_DIR)/test
	cat $(VALG_FILE) 2>&1 | grep lost

## MAKE GCOV REPORT
# specific variable for gcov report target
test_cov: FLAGS:=$(FLAGS) $(COV_FLAGS)

test_cov: build_dir $(TESTS_OBJECTS) $(MODEL_OBJECTS)
	$(CC) $(FLAGS) $(TESTS_OBJECTS) $(MODEL_OBJECTS) $(TEST_FLAGS) -o $(BUILD_DIR)/test_cov 
	@./$(BUILD_DIR)/test_cov

gcov_report: clean test_cov
	@echo "$(GREEN)Generating LCOV coverage report...$(RESET_COLOR)"
	@lcov --capture --directory $(OBJECTS_DIR) --output-file $(BUILD_DIR)/coverage.info
	lcov --remove $(BUILD_DIR)/coverage.info "/usr/*" -o $(BUILD_DIR)/coverage.info
	@echo "$(GREEN)Generating HTML coverage report...$(RESET_COLOR)"
	@genhtml $(BUILD_DIR)/coverage.info --output-directory $(COV_DIR)
	@echo "$(GREEN)Coverage report generated in $(COV_DIR)/index.html$(RESET_COLOR)"
	$(foreach ext, $(GCOV_FILES), find $(BUILD_DIR)/ -type f -name "$(ext)" -delete;)
	$(foreach ext, $(GCOV_FILES), rm -f $(BUILD_DIR)/$(ext);)
	rm -f $(BUILD_DIR)/coverage.info

# << TESTS SECTION ENDS <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

# >> UTILS SECTION >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
build_dir:
	@mkdir -p $(BUILD_DIR) $(OBJECTS_DIR)
	@mkdir -p $(OBJECTS_DIR)/model
	@mkdir -p $(OBJECTS_DIR)/view
	@mkdir -p $(OBJECTS_DIR)/controller
	@mkdir -p $(BUILD_DIR)/logs
	@mkdir -p $(COV_DIR)

style_check:
	clang-format -style=$(STYLE) -n $(MODULES_SOURCES) $(MODULES_HEADERS) $(TESTS_SOURCES) $(TESTS_HEADERS)

style_format:
	clang-format -style=$(STYLE) -i $(MODULES_SOURCES) $(MODULES_HEADERS) $(TESTS_SOURCES) $(TESTS_HEADERS)

install_libs:
	sudo apt-get install $(LIBS)
# << UTILS SECTION ENDS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
