project(Google_tests)
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(Google_Tests_run perft_test.cpp pawn_test.cpp rook_test.cpp bishop_test.cpp
        knight_test.cpp board_test.cpp queen_test.cpp king_test.cpp thread_pool_test.cpp
        transposition_table_test.cpp)

target_link_libraries(Google_Tests_run engine)

target_link_libraries(Google_Tests_run gtest gtest_main)