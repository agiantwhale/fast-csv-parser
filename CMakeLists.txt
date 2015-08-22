cmake_minimum_required(VERSION 3.3)
project(fast_csv_parser)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost COMPONENTS thread filesystem REQUIRED)

if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS} $ENV{GTEST_DIR}/include)
endif()

add_library(fastcsv CsvParser.cpp CsvParser.h)
add_executable(fastcsv_test test.cpp)

enable_testing()
add_test(NAME fastcsv_test COMMAND ./fastcsv_test)

if(Boost_FOUND)
  target_link_libraries(fastcsv ${Boost_THREAD_LIBRARY})
endif()
target_link_libraries(fastcsv_test fastcsv $ENV{GTEST_DIR}/lib/libgtest.a)
