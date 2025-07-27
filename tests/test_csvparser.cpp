
#include "CSVparser.hpp"
#include <gtest/gtest.h>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>

const std::string TEST_CSV_PATH = "./tests/test.csv";
const std::string TEST_CSV_EMPTY = "./tests/empty.csv";
const std::string TEST_CSV_HEADER_ONLY = "./tests/header_only.csv";
const std::string TEST_CSV_SEMICOLON = "./tests/semicolon.csv";

class ParserTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::ofstream file(TEST_CSV_PATH);
        file << "ID,Name,Age\n1,Alice,30\n2,Bob,25";
        file.close();

        std::ofstream empty(TEST_CSV_EMPTY);
        empty.close();

        std::ofstream headerOnly(TEST_CSV_HEADER_ONLY);
        headerOnly << "ID,Name,Age\n";
        headerOnly.close();

        std::ofstream semicolon(TEST_CSV_SEMICOLON);
        semicolon << "ID;Name;Age\nCS101;Intro to Computer Science;3";
        semicolon.close();
    }

    void TearDown() override {
        remove(TEST_CSV_PATH.c_str());
        remove(TEST_CSV_EMPTY.c_str());
        remove(TEST_CSV_HEADER_ONLY.c_str());
        remove(TEST_CSV_SEMICOLON.c_str());
    }
};

TEST(ParserTest, CanLoadCSV) {
    csv::Parser parser(TEST_CSV_PATH);
    ASSERT_EQ(parser.rowCount(), 2);
}

TEST(ParserTest, HeaderIsCorrect) {
    csv::Parser parser(TEST_CSV_PATH);
    std::vector<std::string> expected = {"CourseID", "CourseName"};
    ASSERT_EQ(parser.getHeader(), expected);
}

TEST(ParserTest, AccessFirstRow) {
    csv::Parser parser(TEST_CSV_PATH);
    csv::Row row = parser.getRow(0);
    EXPECT_EQ(row[0], "CS101");
    EXPECT_EQ(row[1], "Intro to Computer Science");
}

TEST(ParserTest, CanAddAndDeleteRow) {
    csv::Parser parser(TEST_CSV_PATH);
    std::vector<std::string> newRow = {"3", "Charlie", "28"};
    parser.addRow(1, newRow);
    EXPECT_EQ(parser.rowCount(), 3);
    EXPECT_TRUE(parser.deleteRow(1));
    EXPECT_EQ(parser.rowCount(), 2);
}

TEST(ParserTest, FileNameIsStored) {
    csv::Parser parser(TEST_CSV_PATH);
    EXPECT_EQ(parser.getFileName(), TEST_CSV_PATH);
}

TEST(ParserTest, HandlesEmptyFileGracefully) {
    try {
        csv::Parser parser(TEST_CSV_EMPTY);
        FAIL() << "Expected exception for empty file";
    } catch (const std::exception& e) {
        SUCCEED();
    }
}

TEST(ParserTest, GetRowOutOfBounds) {
    csv::Parser parser(TEST_CSV_PATH);
    try {
        parser.getRow(100);
        FAIL() << "Expected exception for out-of-bounds access";
    } catch (...) {
        SUCCEED();
    }
}

TEST(ParserTest, AddRowAtPosition) {
    csv::Parser parser(TEST_CSV_PATH);
    std::vector<std::string> row = {"99", "Test", "40"};
    EXPECT_TRUE(parser.addRow(0, row));
    csv::Row newRow = parser.getRow(0);
    EXPECT_EQ(newRow[0], "99");
}

TEST(ParserTest, DeleteRowOutOfBoundsFails) {
    csv::Parser parser(TEST_CSV_PATH);
    EXPECT_FALSE(parser.deleteRow(99));
}

TEST(ParserTest, HeaderOnlyFile) {
    csv::Parser parser(TEST_CSV_HEADER_ONLY);
    EXPECT_EQ(parser.rowCount(), 0);
    EXPECT_EQ(parser.getHeaderElement(1), "CourseName");
}

TEST(ParserTest, CustomDelimiter) {
    csv::Parser parser(TEST_CSV_SEMICOLON, csv::eFILE, ';');
    csv::Row row = parser.getRow(0);
    EXPECT_EQ(row[0], "CS101;Intro to Computer Science");
}

int main(int argc, char **argv) {
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::cout << "\n🕒 Test run started at: " << std::ctime(&now) << std::endl;
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
