cmake_minimum_required(VERSION 3.19)
project(ChessEngine)

set(CMAKE_CXX_STANDARD 20)
set(SOURCE_FILES main.cpp)

add_executable(ChessEngine_run ${SOURCE_FILES})

include_directories(engine)
add_subdirectory(engine)

target_link_libraries(ChessEngine_run engine)

add_subdirectory(google_test)


#add_executable(Chessengine main.cpp board.cpp board.h pieces/pawns.h utils.h Move.h utils.cpp pieces/pawns.cpp pieces/knight.cpp pieces/knight.h pieces/rook.cpp pieces/rook.h pieces/bishop.cpp pieces/bishop.h pieces/queen.cpp pieces/queen.h pieces/king.cpp pieces/king.h)