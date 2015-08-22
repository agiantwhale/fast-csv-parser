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

TEST(CsvReaderTest,OpensACsvFile) {
  {
    std::string csvFilePath=CSV_PATH;
    CsvParser parser(csvFilePath);
    ASSERT_TRUE(parser.Init());
  }

  {
    std::string csvFilePath=CSV_PATH;
    csvFilePath+="_temp";
    CsvParser parser(csvFilePath);
    ASSERT_FALSE(parser.Init());
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
