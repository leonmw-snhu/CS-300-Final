# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Iinclude -Isrc
COVERAGE_FLAGS := --coverage

# Directories
SRC_DIR := src
INCLUDE_DIR := include
BUILD_DIR := build
BIN_DIR := bin
TEST_DIR := tests

# Output binaries
CSV_BIN := $(BIN_DIR)/csv_tool
TEST_BIN := $(BIN_DIR)/test_runner

# Google Test (adjust paths as needed)
GTEST_INC := -I/opt/homebrew/include
GTEST_LIB := -L/opt/homebrew/opt/googletest/lib -lgtest -lgtest_main -pthread

# Files
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
SRC_OBJ := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC_FILES))

TEST_SRC := $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJ := $(patsubst $(TEST_DIR)/%.cpp, $(BUILD_DIR)/%.test.o, $(TEST_SRC))

# Default target
all: $(CSV_BIN)

# Build CSV application
$(CSV_BIN): $(SRC_OBJ)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(COVERAGE_FLAGS) $^ -o $@

# Compile source objects
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(COVERAGE_FLAGS) -c $< -o $@

# Build test runner
test_runner: $(TEST_OBJ) $(BUILD_DIR)/CSVparser.o
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(COVERAGE_FLAGS) $(GTEST_INC) $^ -o $(TEST_BIN) $(GTEST_LIB)

# Compile test sources
$(BUILD_DIR)/%.test.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(COVERAGE_FLAGS) $(GTEST_INC) -c $< -o $@

# Run the tests
run_tests: test_runner
	@echo "🧪 Running tests..."
	@./$(TEST_BIN)

# Generate sample test data
generate_csv:
	@mkdir -p tests
	@echo "CourseID,CourseName" > tests/test.csv
	@echo "CS101,Intro to Computer Science" >> tests/test.csv
	@echo "CS102,Data Structures" >> tests/test.csv
	@echo "ID,Name,Age" > tests/simple.csv
	@echo "1,Alice,20" >> tests/simple.csv
	@echo "2,Bob,22" >> tests/simple.csv

	@echo "CourseID,CourseName" > tests/header_only.csv

	@echo "CourseID;CourseName" > tests/semicolon.csv
	@echo "CS101;Intro to Computer Science" >> tests/semicolon.csv

# Run with coverage report
coverage: run_tests
	@echo "📊 Generating code coverage..."
	@lcov --capture --directory . --output-file coverage.info
	@genhtml coverage.info --output-directory coverage_report
	@echo "✅ Report generated in ./coverage_report/index.html"

# Clean
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR) *.csv *.gcda *.gcno *.gcov coverage.info coverage_report

.PHONY: all clean test_runner run_tests coverage generate_csv