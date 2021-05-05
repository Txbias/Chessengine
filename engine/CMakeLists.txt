project(Engine)

set(HEADER_FILES
        board.h logger.h Move.h utils.h pieces/king.h pieces/queen.h
        pieces/bishop.h pieces/rook.h pieces/knight.h pieces/pawns.h)

set(SOURCE_FILES
        board.cpp logger.cpp utils.cpp pieces/king.cpp pieces/queen.cpp
        pieces/bishop.cpp pieces/rook.cpp pieces/knight.cpp pieces/pawns.cpp)

add_library(engine STATIC ${SOURCE_FILES} ${HEADER_FILES})