#include "gtest/gtest.h"
#include "bishop.h"

class BishopTests : public ::testing::Test {
protected:
    void SetUp() override {
        initializeMagicBitboards();
    }
};

TEST_F(BishopTests, normalMoveFromWhiteSquare) {
    U64 bishops = 1UL << 35;

    std::vector<Move> moves = Bishop::getMoves(bishops, bishops, 0UL);

    ASSERT_EQ(moves.size(), 13);
}

TEST_F(BishopTests, normalMoveFromBlackSquare) {
    U64 bishops = 1UL << 36;

    std::vector<Move> moves = Bishop::getMoves(bishops, bishops, 0UL);

    ASSERT_EQ(moves.size(), 13);
}

TEST_F(BishopTests, inCorner) {
    U64 bishop = 1UL;

    std::vector<Move> moves = Bishop::getMoves(bishop, bishop, 0UL);

    ASSERT_EQ(moves.size(), 7);
}

TEST_F(BishopTests, attack) {
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

TEST_F(BishopTests, doubleAttack) {
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

TEST_F(BishopTests, blocked) {
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

TEST_F(BishopTests, blockedByBishop) {
    U64 bishops = 1UL << 2 | 1UL << 38;

    std::vector<Move> moves = Bishop::getMoves(bishops, bishops, 0UL);

    ASSERT_EQ(moves.size(), 5 + 8);
}

TEST_F(BishopTests, targetMapBlocked) {
    U64 bishop = 1UL << 35;
    U64 ownPieces = bishop | 1UL << 37 | 1UL << 26;

    U64 targets = Bishop::getTargets(bishop, ownPieces, 0UL);
    ASSERT_EQ(getCardinality(targets), 10);
}

TEST_F(BishopTests, targetMapBlockedByBishop) {
    U64 bishop = 1UL | 1UL << 27;

    U64 targets = Bishop::getTargets(bishop, bishop, 0UL);
    ASSERT_EQ(getCardinality(targets), 12);

}

TEST_F(BishopTests, targetMapBlockedByEnemy) {
    U64 bishop = 1UL;
    U64 enemies = 1UL << 18;

    U64 targets = Bishop::getTargets(bishop, bishop, enemies);
    ASSERT_EQ(getCardinality(targets), 2);
}