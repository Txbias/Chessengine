set(HEADER_FILES king.h queen.h bishop.h rook.h knight.h pawns.h)
set(SOURCE_FILES king.cpp queen.cpp bishop.cpp rook.cpp knight.cpp pawns.cpp)

target_sources(engine PRIVATE ${HEADER_FILES} ${SOURCE_FILES})
target_include_directories(engine PUBLIC ${CMAKE_CURRENT_LIST_DIR})