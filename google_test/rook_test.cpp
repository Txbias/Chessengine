#include "gtest/gtest.h"
#include "rook.h"

TEST(RookTest, normalMove) {

    U64 rooks = 1UL << 27;
    U64 enemyPieces = 0UL;

    std::vector<Move> moves = Rook::getMoves(rooks, rooks, enemyPieces);

    ASSERT_EQ(moves.size(), 14);
}

TEST(RookTest, attacks) {

    U64 rooks = 1UL << 35;
    U64 enemyPieces = 1UL << 36 | 1UL << 11 | 1UL << 42;

    std::vector<Move> moves = Rook::getMoves(rooks, rooks, enemyPieces);

    int captures = 0;
    for(Move move : moves) {
        if(move.isCapture()) {
            captures++;
        }
    }

    ASSERT_EQ(moves.size(), 10);
    ASSERT_EQ(captures, 2);
}

TEST(RookTest, blocked) {
    U64 rooks = 1UL << 35 | 1UL << 20;
    U64 ownPieces = rooks | 1UL << 36 | 1UL << 19;

    std::vector<Move> moves = Rook::getMoves(rooks, ownPieces, 0UL);

    ASSERT_EQ(moves.size(), 13);
}

TEST(RookTest, blockedByRook) {
    U64 rooks = 1UL << 34 | 1UL << 36 | 1UL << 10;

    std::vector<Move> moves = Rook::getMoves(rooks, rooks, 0UL);

    int captures = 0;
    for(Move move : moves) {
        if(move.isCapture()) {
            captures++;
        }
    }

    ASSERT_EQ(moves.size(), 8 + 11 + 10);
    ASSERT_EQ(captures, 0);
}