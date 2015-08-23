cmake_minimum_required(VERSION 2.8)
project(fast_csv_parser)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

if(NOT DEFINED TEST_CSV_PATH)
  set(TEST_CSV_PATH ${PROJECT_SOURCE_DIR}/csv/MOCK_DATA.csv)
endif()

configure_file(${PROJECT_SOURCE_DIR}/config.h.in ${PROJECT_BINARY_DIR}/config.h)
include_directories(${PROJECT_BINARY_DIR}
                    ${PROJECT_SOURCE_DIR}/gtest
                    ${PROJECT_SOURCE_DIR}/threadpool)

add_library(fastcsv CsvParser.cpp CsvParser.h)
add_executable(fastcsv_test test.cpp ${PROJECT_SOURCE_DIR}/gtest/gtest/gtest-all.cc)
target_link_libraries(fastcsv_test ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} fastcsv)

enable_testing()
add_test(NAME fastcsv_test COMMAND ./fastcsv_test)
