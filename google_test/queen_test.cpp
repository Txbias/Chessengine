#include "gtest/gtest.h"
#include "pieces/queen.h"

TEST(QueenTest, normalMove) {
    U64 queens = 1UL << 35;

    std::vector<Move> moves = Queen::getMoves(queens, queens, 0UL);

    ASSERT_EQ(moves.size(), 27);
}

TEST(QueenTest, attacks) {
    U64 queens = 1UL << 35;
    U64 enemyPieces = 1UL << 38 | 1UL << 3 | 1UL << 17 | 1UL << 20;

    std::vector<Move> moves = Queen::getMoves(queens, queens, enemyPieces);

    int captures = 0;
    for(Move move : moves) {
        if(move.isCapture()) captures++;
    }

    EXPECT_EQ(captures, 3);
    ASSERT_EQ(moves.size(), 13 + 12);
}

TEST(QueenTest, blocked) {
    U64 queens = 1UL << 35;
    U64 ownPieces = queens | 1UL << 37 | 1UL << 26;

    std::vector<Move> moves = Queen::getMoves(queens, ownPieces, 0UL);

    ASSERT_EQ(moves.size(), 21);
}

TEST(QueenTest, multipleQueens) {
    U64 queens = 1UL << 35 | 1UL << 37 | 1UL << 3 | 1UL << 56;

    std::vector<Move> moves = Queen::getMoves(queens, queens, 0UL);

    ASSERT_EQ(moves.size(), (27-5)+(10+11)+(10+7)+16);
}

TEST(QueenTest, targetMapNormal) {
    U64 queens = 1UL << 35;

    std::vector<Move> moves = Queen::getMoves(queens, queens, 0UL);
    U64 targets = Queen::getTargets(queens, queens, 0UL);

    EXPECT_EQ(moves.size(), getCardinality(targets));

    for(Move move : moves) {
        EXPECT_TRUE(targets & (1UL << move.getTo()));
    }
}

TEST(QueenTest, targetMapAttack) {
    U64 queens = 1UL << 35;
    U64 enemyPieces = 1UL << 38 | 1UL << 3 | 1UL << 17 | 1UL << 20;

    std::vector<Move> moves = Queen::getMoves(queens, queens, enemyPieces);
    U64 targets = Queen::getTargets(queens, queens, enemyPieces);

    EXPECT_EQ(getCardinality(targets), moves.size());

    for(Move move : moves) {
        EXPECT_TRUE(targets & (1UL << move.getTo()));
    }
}

TEST(QueenTest, targetMapBlocked) {
    U64 queens = 1UL << 35;
    U64 ownPieces = queens | 1UL << 37 | 1UL << 26;

    std::vector<Move> moves = Queen::getMoves(queens, ownPieces, 0UL);
    U64 targets = Queen::getTargets(queens, ownPieces, 0UL);

    EXPECT_EQ(getCardinality(targets), moves.size());

    for(Move move : moves) {
        EXPECT_TRUE(targets & (1UL << move.getTo()));
    }
}

TEST(QueenTest, targetMapMultipleQueens) {
    U64 queens = 1UL << 35 | 1UL << 37 | 1UL << 3 | 1UL << 56;
    std::vector<Move> moves = Queen::getMoves(queens, queens, 0UL);
    U64 targets = Queen::getTargets(queens, queens, 0UL);

    for(Move move : moves) {
        EXPECT_TRUE(targets & (1UL << move.getTo()));
    }
}

