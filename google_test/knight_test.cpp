#include "gtest/gtest.h"
#include "knight.h"

TEST(KnightTest, normalMove) {
    U64 knights = 1UL << 35;

    std::vector<Move> moves = Knight::getMoves(knights, 0UL, knights);

    ASSERT_EQ(moves.size(), 8);
}

TEST(KnightTest, fromCorner) {
    U64 knights = 1UL << 0;
    U64 ownPieces = knights | 1UL << 1;

    std::vector<Move> moves = Knight::getMoves(knights, 0UL, ownPieces);

    ASSERT_EQ(moves.size(), 2);
}

TEST(KnightTest, multipleKnights) {
    U64 knights = 1UL << 35 | 1UL << 25;

    std::vector<Move> moves = Knight::getMoves(knights, 0UL, knights);

    ASSERT_EQ(moves.size(), 7 + 5);
}

TEST(KnightTest, blocked) {
    U64 knights = 1UL << 35;
    U64 ownPieces = knights | 1UL << 25 | 1UL << 36 | 1UL << 34 | 1UL << 50;

    std::vector<Move> moves = Knight::getMoves(knights, 0UL, ownPieces);

    ASSERT_EQ(moves.size(), 6);
}

TEST(KnightTest, attack) {
    U64 knights = 1UL << 35;
    U64 enemyPieces = 1UL << 25 | 1UL << 50;

    std::vector<Move> moves = Knight::getMoves(knights, enemyPieces, knights);
    int captures = 0;
    for(Move move : moves) {
        if(move.isCapture()) captures++;
    }

    ASSERT_EQ(moves.size(), 8);
    ASSERT_EQ(captures, 2);
}