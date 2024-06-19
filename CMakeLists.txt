cmake_minimum_required(VERSION 3.22.1)
project(AlphaDeepChess VERSION 0.1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(src)
include_directories(src/board)
include_directories(src/moveGenerator)

add_executable(AlphaDeepChess 
src/main.cpp
src/uci.cpp
src/board/board.cpp
src/moveGenerator/moveGenerator.cpp
)

target_compile_options(AlphaDeepChess PRIVATE -g -Wall)