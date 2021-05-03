#include "gtest/gtest.h"
#include "pieces/pawns.h"


TEST(PawnTest, BasePositionWhite) {

    U64 whitePawns = setRow(0UL, 1);

    std::vector<Move> moves = Pawn::getMoves(whitePawns, ~0UL, 0UL, 0);

    ASSERT_EQ(moves.size(), 16);
}

TEST(PawnTest, BasePositionBlack) {
    U64 blackPawns = setRow(0UL, 6);

    std::vector<Move> moves = Pawn::getMoves(blackPawns, ~0UL, 0UL, 1);
    ASSERT_EQ(moves.size(), 16);
}

TEST(PawnTest, WhiteAttackWest) {

    U64 whitePawns = 1UL << 10;
    U64 enemyPieces = 1UL << 17;
    U64 empty = ~(whitePawns | enemyPieces);

    std::vector<Move> moves = Pawn::getMoves(whitePawns, empty, enemyPieces, 0);

    // 3 because pawn can dbl push
    ASSERT_EQ(moves.size(), 3);
}

TEST(PawnTest, WhiteAttackEast) {
    U64 whitePawns = 1UL << 18;
    U64 enemyPieces = 1UL << 27;
    U64 empty = ~(whitePawns | enemyPieces);

    std::vector<Move> moves = Pawn::getMoves(whitePawns, empty, enemyPieces, 0);
    ASSERT_EQ(moves.size(), 2);
}

TEST(PawnTest, BlackAttackWest) {
    U64 blackPawns = 1UL << 33;
    U64 enemyPieces = 1UL << 24 | 1UL << 50 | 1UL << 1;
    U64 empty = ~(blackPawns | enemyPieces);

    std::vector<Move> moves = Pawn::getMoves(blackPawns, empty, enemyPieces, 1);
    ASSERT_EQ(moves.size(), 2);
}

TEST(PawnTest, BlackAttackEast) {
    U64 blackPawns = 1UL << 14;
    U64 enemyPieces = 1UL << 7 | 1UL << 30 | 1UL << 6;
    U64 empty = ~(blackPawns | enemyPieces);

    std::vector<Move> moves = Pawn::getMoves(blackPawns, empty, enemyPieces, 1);
    ASSERT_EQ(moves.size(), 1);
}

TEST(PawnTest, WhiteAttackBoth) {
    U64 whitePawns = 1UL << 50;
    U64 enemyPieces = 1UL << 57 | 1UL << 59 | 1UL << 2 | 1UL << 29;
    U64 empty = ~(whitePawns | enemyPieces);

    std::vector<Move> moves = Pawn::getMoves(whitePawns, empty, enemyPieces, 0);
    ASSERT_EQ(moves.size(), 3);
}

TEST(PawnTest, BlackAttackBoth) {
    U64 blackPawns = 1UL << 19;
    U64 enemyPieces = 1UL << 10 | 1UL << 12;
    U64 empty = ~(blackPawns | enemyPieces);

    std::vector<Move> moves = Pawn::getMoves(blackPawns, empty, enemyPieces, 1);
    ASSERT_EQ(moves.size(), 3);
}
