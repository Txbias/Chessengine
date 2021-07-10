#include "gtest/gtest.h"
#include "rook.h"
#include "utils.h"

class RookTests : public ::testing::Test {
protected:
    void SetUp() override {
        initializeMagicBitboards();
    }
};

TEST_F(RookTests, normalMove) {

    U64 rooks = 1UL << 27;
    U64 enemyPieces = 0UL;

    std::vector<Move> moves = Rook::getMoves(rooks, rooks, enemyPieces);

    ASSERT_EQ(moves.size(), 14);
}

TEST_F(RookTests, attacks) {

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

TEST_F(RookTests, blocked) {
    U64 rooks = 1UL << 35 | 1UL << 20;
    U64 ownPieces = rooks | 1UL << 36 | 1UL << 19;

    std::vector<Move> moves = Rook::getMoves(rooks, ownPieces, 0UL);

    ASSERT_EQ(moves.size(), 13);
}

TEST_F(RookTests, blockedByRook) {
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

TEST_F(RookTests, targetMap) {
    U64 rooks = 1UL;

    U64 targets = Rook::getTargets(rooks, rooks, 0UL);
    ASSERT_EQ(getCardinality(targets), 14);
}

TEST_F(RookTests, targetMapBlocked) {
    U64 rooks = 1UL | 1UL << 5;

    U64 targets = Rook::getTargets(rooks, rooks, 0UL);
    ASSERT_EQ(getCardinality(targets), 20);

}

TEST_F(RookTests, targetMapBlockedByEnemy) {
    U64 rooks = 1UL;
    U64 enemies = 1UL << 3;

    U64 targets = Rook::getTargets(rooks, rooks, enemies);
    ASSERT_EQ(getCardinality(targets), 10);
}

TEST_F(RookTests, targetMapBlockedByEnemy2) {
    U64 rooks = 1UL << 35;
    U64 enemies = 1UL << 38 | 1UL << 1;

    U64 targets = Rook::getTargets(rooks, rooks, enemies);
    ASSERT_EQ(Rook::getMoves(rooks, rooks, enemies).size(), getCardinality(targets));
}