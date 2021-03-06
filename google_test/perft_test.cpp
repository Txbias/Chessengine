#include "gtest/gtest.h"
#include "board.h"
#include "utils.h"

typedef unsigned long long ull;

ull perft(Board& board, int depth, ull &captures, ull &checks, ull &checkMates);

TEST(PerftTest, DEPTH_0) {
    Board board;

    ull captures = 0;
    ull checks = 0;
    ull checkMates = 0;
    ull nodes = perft(board, 0, captures, checks, checkMates);

    ASSERT_EQ(nodes, 1);
    ASSERT_EQ(captures, 0);
    ASSERT_EQ(checks, 0);
    ASSERT_EQ(checkMates, 0);
}

TEST(PerftTest, DEPTH_1) {
    Board board;

    ull captures = 0;
    ull checks = 0;
    ull checkMates = 0;
    ull nodes = perft(board, 1, captures, checks, checkMates);

    ASSERT_EQ(nodes, 20);
    ASSERT_EQ(captures, 0);
    ASSERT_EQ(checks, 0);
    ASSERT_EQ(checkMates, 0);
}

TEST(PerftTest, DEPTH_2) {
    Board board;

    ull captures = 0;
    ull checks = 0;
    ull checkMates = 0;
    ull nodes = perft(board, 2, captures, checks, checkMates);
    ASSERT_EQ(nodes, 400);
    ASSERT_EQ(captures, 0);
    ASSERT_EQ(checks, 0);
}

TEST(PerftTest, DEPTH_3) {
    Board board;

    ull captures = 0;
    ull checks = 0;
    ull checkMates = 0;
    ull nodes = perft(board, 3, captures, checks, checkMates);
    ASSERT_EQ(captures, 34);
    ASSERT_EQ(checks, 12);
    ASSERT_EQ(nodes, 8902);
    ASSERT_EQ(checkMates, 0);
}

TEST(PerftTest, DEPTH_4) {
    Board board;

    ull captures = 0;
    ull checks = 0;
    ull checkMates = 0;
    ull nodes = perft(board, 4, captures, checks, checkMates);

    EXPECT_EQ(captures, 1576);
    EXPECT_EQ(checks, 469);
    ASSERT_EQ(nodes, 197281);
    ASSERT_EQ(checkMates, 8);
}


ull perft(Board& board, int depth, ull &captures, ull &checks, ull &checkMates) {

    if(depth == 0) {
        return 1ULL;
    }

    ull nodes = 0;

    std::vector<Move> moves = board.getAllMoves(depth % 2);
    for(Move move : moves) {
        board.executeMove(move);
        if(!board.inCheck(depth % 2)) {
            if(move.isCapture() && depth == 1) {
                captures++;
            }

            if(board.checkMate(ENEMY(depth % 2)) && depth == 1) {
                checkMates++;
            }

            if(board.inCheck(ENEMY(depth % 2)) && depth == 1) {
                checks++;
            }
            nodes += perft(board, depth - 1, captures, checks, checkMates);
        }
        board.undoLastMove();
    }

    return nodes;
}

