#include "gtest/gtest.h"
#include "pieces/king.h"

TEST(KingTest, castlingWhite) {

    U64 occupied = 1UL << 4 | 1UL << 25;
    U64 enemyAttackMap = 1UL << 50 | 1UL << 40;
    int team = 0;
    bool kingMoved[2] = {false, true};
    bool rookMoved[2][2] = {{false, false}, {true, true}};

    std::vector<Move> moves = King::getCastlingMoves(occupied, enemyAttackMap, team, kingMoved,
                                             rookMoved);

    ASSERT_EQ(moves.size(), 2);
    for(Move move : moves) {
        ASSERT_TRUE(move.getFrom() == 4);
        ASSERT_TRUE(move.getTo() == 2 || move.getTo() == 6);
    }
}

TEST(KingTest, castlingBlack) {

    U64 occupied = 1UL << 60 | 1UL << 56 | 1UL << 63;
    U64 enemyAttackMap = 1UL << 50 | 1UL << 40;
    int team = 1;
    bool kingMoved[2] = {true, false};
    bool rookMoved[2][2] = {{true, true}, {false, false}};

    std::vector<Move> moves = King::getCastlingMoves(occupied, enemyAttackMap, team, kingMoved,
                                                     rookMoved);

    ASSERT_EQ(moves.size(), 2);
    for(Move move : moves) {
        ASSERT_TRUE(move.getFrom() == 60);
        ASSERT_TRUE(move.getTo() == 58 || move.getTo() == 62);
    }
}

TEST(KingTest, castlingWhiteBlocked) {

    U64 occupied = 1UL << 4 | 1UL << 25 | 1UL << 5;
    U64 enemyAttackMap = 1UL << 2 | 1UL << 40;
    int team = 0;
    bool kingMoved[2] = {false, true};
    bool rookMoved[2][2] = {{false, false}, {true, true}};

    std::vector<Move> moves = King::getCastlingMoves(occupied, enemyAttackMap, team, kingMoved,
                                                     rookMoved);

    ASSERT_EQ(moves.size(), 0);
}

TEST(KingTest, castlingBlackBlocked) {
    U64 occupied = 1UL << 59 | 1UL << 25 | 1UL << 1;
    U64 enemyAttackMap = 1UL << 2 | 1UL << 61;
    int team = 1;
    bool kingMoved[2] = {true, false};
    bool rookMoved[2][2] = {{false, false}, {false, false}};

    std::vector<Move> moves = King::getCastlingMoves(occupied, enemyAttackMap, team, kingMoved,
                                                     rookMoved);

    ASSERT_EQ(moves.size(), 0);
}

TEST(KingTest, castlingWhiteKingMoved) {
    U64 occupied = 1UL << 4 | 1UL << 25 | 1UL << 10;
    U64 enemyAttackMap = 1UL << 45 | 1UL << 40;
    int team = 0;
    bool kingMoved[2] = {true, true};
    bool rookMoved[2][2] = {{false, false}, {true, true}};

    std::vector<Move> moves = King::getCastlingMoves(occupied, enemyAttackMap, team, kingMoved,
                                                     rookMoved);

    ASSERT_EQ(moves.size(), 0);
}

TEST(KingTest, castlingBlackKingMoved) {
    U64 occupied = 1UL << 4 | 1UL << 25 | 1UL << 10;
    U64 enemyAttackMap = 1UL << 45 | 1UL << 40;
    int team = 1;
    bool kingMoved[2] = {false, true};
    bool rookMoved[2][2] = {{false, false}, {false, false}};

    std::vector<Move> moves = King::getCastlingMoves(occupied, enemyAttackMap, team, kingMoved,
                                                     rookMoved);

    ASSERT_EQ(moves.size(), 0);
}
