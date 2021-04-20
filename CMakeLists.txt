cmake_minimum_required(VERSION 3.19)
project(Chessengine)

set(CMAKE_CXX_STANDARD 20)

add_executable(Chessengine main.cpp pieces/Piece.h board.cpp board.h pieces/pawns.h utils.h Move.h)