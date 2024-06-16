cmake_minimum_required(VERSION 3.22.1)
project(AlphaDeepChess VERSION 0.1.0)

include_directories(src)
include_directories(src/board)

add_executable(AlphaDeepChess 
src/main.cpp
src/uci.cpp
src/board/board.cpp
)

target_compile_options(AlphaDeepChess PRIVATE -g -Wall)