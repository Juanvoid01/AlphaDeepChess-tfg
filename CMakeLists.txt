cmake_minimum_required(VERSION 3.22.1)
project(AlphaDeepChess VERSION 0.1.0)

include_directories(src)

add_executable(AlphaDeepChess 
src/main.cpp
)

target_compile_options(AlphaDeepChess PRIVATE -g -Wall)