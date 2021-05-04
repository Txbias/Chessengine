#include "gtest/gtest.h"
#include "pieces/bishop.h"

TEST(BishopTest, normalMoveFromWhiteSquare) {
    U64 bishops = 1UL << 35;

    std::vector<Move> moves = Bishop::getMoves(bishops, bishops, 0UL);

    ASSERT_EQ(moves.size(), 13);
}

TEST(BishopTest, normalMoveFromBlackSquare) {
    U64 bishops = 1UL << 36;

    std::vector<Move> moves = Bishop::getMoves(bishops, bishops, 0UL);

    ASSERT_EQ(moves.size(), 13);
}

TEST(BishopTest, inCorner) {
    U64 bishop = 1UL;

    std::vector<Move> moves = Bishop::getMoves(bishop, bishop, 0UL);

    ASSERT_EQ(moves.size(), 7);
}

TEST(BishopTest, attack) {
    U64 bishops = 1UL << 6;
    U64 enemyPieces = 1UL << 15 | 1UL << 34;

    std::vector<Move> moves = Bishop::getMoves(bishops, bishops, enemyPieces);
    int captures = 0;
    for(Move move : moves) {
        if(move.isCapture()) captures++;
    }

    ASSERT_EQ(moves.size(), 5);
    ASSERT_EQ(captures, 2);
}

TEST(BishopTest, doubleAttack) {
    U64 bishops = 1UL << 2 | 1UL << 38;
    U64 enemyPieces = 1UL << 20;

    std::vector<Move> moves = Bishop::getMoves(bishops, bishops, enemyPieces);

    int captures = 0;
    for(Move move : moves) {
        if(move.isCapture()) captures++;
    }

    ASSERT_EQ(moves.size(), 4 + 7);
    ASSERT_EQ(captures, 2);
}

TEST(BishopTest, blocked) {
    U64 bishops = 1UL << 10 | 1UL << 11;
    U64 ownPieces = bishops | 1UL << 1 | 1UL << 18 | 1UL << 55;

    std::vector<Move> moves = Bishop::getMoves(bishops, ownPieces, 0UL);
    int captures = 0;
    for(Move move : moves) {
        if(move.isCapture()) captures++;
    }

    ASSERT_EQ(moves.size(), 7 + 6);
    ASSERT_EQ(captures, 0);
}

TEST(BishopTest, blockedByBishop) {
    U64 bishops = 1UL << 2 | 1UL << 38;

    std::vector<Move> moves = Bishop::getMoves(bishops, bishops, 0UL);

    ASSERT_EQ(moves.size(), 5 + 8);
}