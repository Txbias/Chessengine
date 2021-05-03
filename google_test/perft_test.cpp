#include "gtest/gtest.h"
#include "board.h"

typedef unsigned long long ull;

ull perft(Board& board, int depth);

TEST(PerftTest, DEPTH_0) {
    Board board;

    ull nodes = perft(board, 0);
    ASSERT_EQ(nodes, 1);
}

TEST(PerftTest, DEPTH_1) {
    Board board;

    ull nodes = perft(board, 1);

    ASSERT_EQ(nodes, 20);
}

TEST(PerftTest, DEPTH_2) {
    Board board;

    ull nodes = perft(board,2);
    ASSERT_EQ(nodes, 400);
}

TEST(PerftTest, DEPTH_3) {
    Board board;

    ull nodes = perft(board, 3);
    ASSERT_EQ(nodes, 8902);
}


ull perft(Board& board, int depth) {

    if(depth == 0) {
        return 1ULL;
    }

    ull nodes = 0;

    std::vector<Move> moves = board.getAllMoves(depth % 2);
    for(Move move : moves) {
        board.executeMove(move);
        if(!board.inCheck(depth % 2)) {
            nodes += perft(board, depth - 1);
        }
        board.undoLastMove();
    }

    return nodes;
}

