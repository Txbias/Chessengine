project(Engine)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
set(HEADER_FILES board.h logger.h move.h utils.h thread_pool.h transposition_table.h search.h constants.h)
set(SOURCE_FILES board.cpp logger.cpp utils.cpp transposition_table.cpp search.cpp)

add_library(engine STATIC ${SOURCE_FILES} ${HEADER_FILES})
add_subdirectory(pieces)
target_link_libraries(engine ${CMAKE_THREAD_LIBS_INIT})