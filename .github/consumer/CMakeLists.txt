# A minimal project that consumes an installed QuickFAST, used by CI to check
# that the exported package works.
cmake_minimum_required(VERSION 3.16)
project(quickfast_consumer LANGUAGES CXX)
find_package(QuickFAST REQUIRED)
add_executable(quickfast_consumer main.cpp)
target_link_libraries(quickfast_consumer PRIVATE QuickFAST::QuickFAST)
