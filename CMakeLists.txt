cmake_minimum_required(VERSION 3.19)
project(Chessengine)

set(CMAKE_CXX_STANDARD 20)

add_executable(Chessengine main.cpp board.cpp board.h pieces/pawns.h utils.h Move.h utils.cpp pieces/pawns.cpp pieces/knight.cpp pieces/knight.h pieces/rook.cpp pieces/rook.h pieces/bishop.cpp pieces/bishop.h pieces/queen.cpp pieces/queen.h pieces/king.cpp pieces/king.h)