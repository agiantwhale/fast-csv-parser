/*
 * =====================================================================================
 *
 *       Filename:  test.cpp
 *
 *    Description:  Runs unit tests
 *
 *        Version:  1.0
 *        Created:  2015/08/21 23시 32분 39초
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Il Jae Lee (ij), iljae@umich.edu
 *   Organization:
 *
 * =====================================================================================
 */
#define CSV_PATH "/Users/iljae/Development/fast-csv-parser/csv/MOCK_DATA.csv"

#include "CsvParser.h"
#include <gtest/gtest.h>

const char * GetNextField(char *& cursor);
void PopulateRow(Row &row,char *start);

TEST(GetNextField,ProcessesACsvFile) {
  char csv_row_string[]="First, Second,,Empty, Last\n";
  char * cursor= &csv_row_string[0];
  ASSERT_STREQ(GetNextField(cursor), "First");
  ASSERT_STREQ(GetNextField(cursor), "Second");
  ASSERT_STREQ(GetNextField(cursor), "");
  ASSERT_STREQ(GetNextField(cursor), "Empty");
  ASSERT_STREQ(GetNextField(cursor), "Last");
  ASSERT_EQ(GetNextField(cursor), nullptr);
}

TEST(PopulateRow,ProcessesACsvFile) {
  char csv_row_string[]="First, Second,,Empty, \" Last , Comma\"\n";
  Row row;
  PopulateRow(row,&csv_row_string[0]);
  ASSERT_EQ(row.size(),5);
  ASSERT_STREQ(row[0], "First");
  ASSERT_STREQ(row[1], "Second");
  ASSERT_STREQ(row[2], "");
  ASSERT_STREQ(row[3], "Empty");
  ASSERT_STREQ(row[4], "\" Last , Comma\"");
}

TEST(CsvReaderTest,OpensACsvFile) {
  std::string csv_file_path(CSV_PATH);
  CsvParser valid_parser(csv_file_path);
  ASSERT_TRUE(valid_parser.Init());

  csv_file_path+="_temp";
  CsvParser invalid_parser(csv_file_path);
  ASSERT_FALSE(invalid_parser.Init());
}

TEST(CsvReaderTest,ReadLinesSingleThreaded) {
  std::string csv_file_path(CSV_PATH);
  CsvParser parser(csv_file_path);
  ASSERT_TRUE(parser.Init());

  int lines_count=0;
  Row line;
  while(parser.Read(line)) {
    lines_count++;
  }

  // 1000 + 1 because of the header line
  ASSERT_EQ(lines_count,1001);
}

TEST(CsvReaderTest,ReadLinesMultiThreaded) {
  std::string csv_file_path(CSV_PATH);
  CsvParser parser(csv_file_path);
  ASSERT_TRUE(parser.Init());

  int lines_count=0;
  Rows lines(4);
  while(parser.Read(lines)) {
    lines_count+=lines.valid_rows();
  }

  // 1000 + 1 because of the header line
  ASSERT_EQ(lines_count,1001);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
