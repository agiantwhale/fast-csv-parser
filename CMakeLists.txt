cmake_minimum_required(VERSION 2.8)
project(fast_csv_parser)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost COMPONENTS system thread filesystem REQUIRED)

if(NOT DEFINED TEST_CSV_PATH)
  set(TEST_CSV_PATH ${PROJECT_SOURCE_DIR}/csv/MOCK_DATA.csv)
endif()

configure_file(${PROJECT_SOURCE_DIR}/config.h.in ${PROJECT_BINARY_DIR}/config.h)
include_directories(${PROJECT_BINARY_DIR} ${PROJECT_SOURCE_DIR}/gtest)

if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif()

add_library(fastcsv CsvParser.cpp CsvParser.h)
add_executable(fastcsv_test test.cpp ${PROJECT_SOURCE_DIR}/gtest/gtest/gtest-all.cc)

if(Boost_FOUND)
  target_link_libraries(fastcsv ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY})
  target_link_libraries(fastcsv_test ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} fastcsv)
endif()

enable_testing()
add_test(NAME fastcsv_test COMMAND ./fastcsv_test)
