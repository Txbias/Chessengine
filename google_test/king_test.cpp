#include "gtest/gtest.h"
#include "pieces/king.h"
#include "board.h"
#include "utils.h"

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

TEST(KingTest, castlingMoveExecution) {
    Board board;

    int cardinality = getCardinality(board.getOccupied());

    Move move(6, 21, FLAG_QUIET);
    board.executeMove(move);
    ASSERT_EQ(cardinality, getCardinality(board.getOccupied()));

    Move move2(62, 44, FLAG_QUIET);
    board.executeMove(move2);
    ASSERT_EQ(cardinality, getCardinality(board.getOccupied()));

    Move move3(14, 14+8, FLAG_QUIET);
    board.executeMove(move3);
    ASSERT_EQ(cardinality, getCardinality(board.getOccupied()));


    Move move4(54, 54-8, FLAG_QUIET);
    board.executeMove(move4);
    ASSERT_EQ(cardinality, getCardinality(board.getOccupied()));

    Move move5(5, 14, FLAG_QUIET);
    board.executeMove(move5);
    ASSERT_EQ(cardinality, getCardinality(board.getOccupied()));

    Move move6(61, 54, FLAG_QUIET);
    board.executeMove(move6);
    ASSERT_EQ(cardinality, getCardinality(board.getOccupied()));

    Move move7(4, 6, FLAG_KING_CASTLE);
    board.executeMove(move7);
    ASSERT_EQ(cardinality, getCardinality(board.getOccupied()));

    Move move8(60, 62, FLAG_KING_CASTLE);
    board.executeMove(move8);
    ASSERT_EQ(cardinality, getCardinality(board.getOccupied()));

    ASSERT_TRUE(board.kings[0] & (1UL << 6));
    ASSERT_TRUE(board.kings[1] & (1UL << 62));

    ASSERT_TRUE(board.rooks[0] & (1UL << 5));
    ASSERT_TRUE(board.kings[1] & (1UL << 62));

    ASSERT_EQ(getCardinality(board.kings[0]), 1);
    ASSERT_EQ(getCardinality(board.kings[1]), 1);


    Move move9(8, 16, FLAG_QUIET);
    board.executeMove(move9);

    Move move10(48, 48-16, FLAG_PAWN_DBL_PUSH);
    board.executeMove(move10);

    Move move11(5, 4, FLAG_QUIET);
    board.executeMove(move11);

    Move move12(61, 60, FLAG_QUIET);
    board.executeMove(move12);

    for(int i = 0; i < 4; i++) {
        board.undoLastMove();
    }

    ASSERT_TRUE(board.kings[0] & (1UL << 6));
    ASSERT_TRUE(board.kings[1] & (1UL << 62));

    ASSERT_TRUE(board.rooks[0] & (1UL << 5));
    ASSERT_TRUE(board.kings[1] & (1UL << 62));

    ASSERT_EQ(getCardinality(board.kings[0]), 1);
    ASSERT_EQ(getCardinality(board.kings[1]), 1);

    while(!board.moves.empty()) {
        board.undoLastMove();
        ASSERT_EQ(cardinality, getCardinality(board.getOccupied()));
        ASSERT_EQ(getCardinality(board.kings[0]), 1);
        ASSERT_EQ(getCardinality(board.kings[1]), 1);
    }
}