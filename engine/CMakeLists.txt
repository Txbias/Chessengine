project(Engine)

set(HEADER_FILES board.h logger.h Move.h utils.h position.h)
set(SOURCE_FILES board.cpp logger.cpp utils.cpp position.cpp)

add_library(engine STATIC ${SOURCE_FILES} ${HEADER_FILES})
add_subdirectory(pieces)