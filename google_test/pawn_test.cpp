#include "gtest/gtest.h"
#include "pieces/pawns.h"
#include "board.h"


TEST(PawnTest, BasePositionWhite) {

    U64 whitePawns = setRow(0UL, 1);

    std::vector<Move> moves = Pawn::getMoves(whitePawns, ~0UL, 0UL, 0UL, 0);

    ASSERT_EQ(moves.size(), 16);
}

TEST(PawnTest, BasePositionBlack) {
    U64 blackPawns = setRow(0UL, 6);

    std::vector<Move> moves = Pawn::getMoves(blackPawns, ~0UL, 0UL, 0UL, 1);
    ASSERT_EQ(moves.size(), 16);
}

TEST(PawnTest, WhiteAttackWest) {

    U64 whitePawns = 1UL << 10;
    U64 enemyPieces = 1UL << 17;
    U64 empty = ~(whitePawns | enemyPieces);

    std::vector<Move> moves = Pawn::getMoves(whitePawns, empty, enemyPieces, 0UL, 0);

    // 3 because pawn can dbl push
    ASSERT_EQ(moves.size(), 3);
}

TEST(PawnTest, WhiteAttackEast) {
    U64 whitePawns = 1UL << 18;
    U64 enemyPieces = 1UL << 27;
    U64 empty = ~(whitePawns | enemyPieces);

    std::vector<Move> moves = Pawn::getMoves(whitePawns, empty, enemyPieces, 0UL, 0);
    ASSERT_EQ(moves.size(), 2);
}

TEST(PawnTest, BlackAttackWest) {
    U64 blackPawns = 1UL << 33;
    U64 enemyPieces = 1UL << 24 | 1UL << 50 | 1UL << 1;
    U64 empty = ~(blackPawns | enemyPieces);

    std::vector<Move> moves = Pawn::getMoves(blackPawns, empty, enemyPieces, 0UL, 1);
    ASSERT_EQ(moves.size(), 2);
}

TEST(PawnTest, BlackAttackEast) {
    U64 blackPawns = 1UL << 14;
    U64 enemyPieces = 1UL << 7 | 1UL << 30 | 1UL << 6;
    U64 empty = ~(blackPawns | enemyPieces);

    std::vector<Move> moves = Pawn::getMoves(blackPawns, empty, enemyPieces, 0UL, 1);
    ASSERT_EQ(moves.size(), 1);
}

TEST(PawnTest, WhiteAttackBoth) {
    U64 whitePawns = 1UL << 50;
    U64 enemyPieces = 1UL << 57 | 1UL << 59 | 1UL << 2 | 1UL << 29;
    U64 empty = ~(whitePawns | enemyPieces);

    std::vector<Move> moves = Pawn::getMoves(whitePawns, empty, enemyPieces, 0UL, 0);
    ASSERT_EQ(moves.size(), 3);
}

TEST(PawnTest, BlackAttackBoth) {
    U64 blackPawns = 1UL << 19;
    U64 enemyPieces = 1UL << 10 | 1UL << 12;
    U64 empty = ~(blackPawns | enemyPieces);

    std::vector<Move> moves = Pawn::getMoves(blackPawns, empty, enemyPieces, 0UL, 1);
    ASSERT_EQ(moves.size(), 3);
}

TEST(PawnTest, EnPassantTestWhite) {
    Board board;

    std::vector<Move> moves = {
            Move(12, 12 + 16, FLAG_PAWN_DBL_PUSH),
            Move(48, 48 - 16, FLAG_PAWN_DBL_PUSH),
            Move(28, 28 + 8, FLAG_QUIET),
            Move(51, 51 - 16, FLAG_PAWN_DBL_PUSH),
    };

    for(Move move : moves) {
        board.executeMove(move);
    }

    Move epMove(0, 0, 0);
    bool foundEPMove = false;
    for(Move move : board.getAllMoves(0)) {
        if(move.getFlags() == FLAG_EP_CAPTURE) {
            ASSERT_FALSE(foundEPMove);
            foundEPMove = true;
            epMove = move;
        }
    }

    ASSERT_TRUE(foundEPMove);

    board.executeMove(epMove);

    ASSERT_TRUE(board.pawns[0] & (1UL << 43));
    ASSERT_FALSE(board.pawns[1] & (1UL << 35));
    ASSERT_FALSE(board.pawns[1] & (1UL << 35));
    ASSERT_EQ(getCardinality(board.pawns[1]), 7);

    board.undoLastMove();

    ASSERT_TRUE(board.pawns[0] & (1UL << 36));
    ASSERT_TRUE(board.pawns[1] & (1UL << 35));
}

TEST(PawnTest, EnPassantTestBlack) {
    Board board;

    std::vector<Move> moves = {
            Move(8, 8 + 16, FLAG_PAWN_DBL_PUSH),
            Move(51, 51 - 16, FLAG_PAWN_DBL_PUSH),
            Move(24, 24 + 8, FLAG_QUIET),
            Move(35, 35 - 8, FLAG_QUIET),
            Move(12, 12 + 16, FLAG_PAWN_DBL_PUSH),
    };

    for(Move move : moves) {
        board.executeMove(move);
    }

    Move epMove(0, 0, 0);
    bool foundEPMove = false;
    for(Move move : board.getAllMoves(1)) {
        if(move.getFlags() == FLAG_EP_CAPTURE) {
            ASSERT_FALSE(foundEPMove);
            foundEPMove = true;
            epMove = move;
        }
    }

    ASSERT_TRUE(foundEPMove);

    board.executeMove(epMove);

    ASSERT_TRUE(board.pawns[1] & (1UL << 20));
    ASSERT_FALSE(board.pawns[0] & (1UL << 28));
    ASSERT_FALSE(board.pawns[1] & (1UL << 28));

    ASSERT_EQ(getCardinality(board.pawns[0]), 7);
    ASSERT_EQ(getCardinality(board.pawns[1]), 8);

    board.undoLastMove();

    ASSERT_TRUE(board.pawns[0] & (1UL << 28));
    ASSERT_TRUE(board.pawns[1] & (1UL << 27));
}

TEST(PawnTest, EnPassantMultiplePawnsAttackWhite) {

    U64 pawns = 1UL << 32 | 1UL << 34 | 1UL << 12;
    U64 enemyPieces = 1UL << 33 | 1UL << 56 | 1UL << 14;
    U64 empty = ~(pawns | enemyPieces);
    U64 enPassantTarget = 1UL << 41;

    std::vector<Move> pawnMoves = Pawn::getMoves(pawns, empty, enemyPieces,
                                                 enPassantTarget, 0);

    int countEPMoves = 0;
    for(Move move : pawnMoves) {
        if(move.getFlags() == FLAG_EP_CAPTURE) {
            countEPMoves++;
            ASSERT_EQ(move.getTo(), 41);
            ASSERT_TRUE(move.getFrom() == 32 || move.getFrom() == 34);
        }
    }

    ASSERT_EQ(countEPMoves, 2);
}

TEST(PawnTest, EnPassantMoveGeneration) {

    U64 pawns = 1UL << 36 | 1UL << 41 | 1UL << 8;
    U64 enemyPieces = 1UL << 37 | 1UL << 54 | 1UL << 62;
    U64 empty = ~(pawns | enemyPieces);
    U64 enPassantTarget = 1UL << 45;

    std::vector<Move> pawnMoves = Pawn::getMoves(pawns, empty, enemyPieces,
                                                 enPassantTarget, 0);

    bool foundEPMove = false;
    for(Move move : pawnMoves) {
        if(move.getFlags() == FLAG_EP_CAPTURE) {
            ASSERT_FALSE(foundEPMove);
            foundEPMove = true;
            ASSERT_EQ(move.getTo(), 45);
            ASSERT_EQ(move.getFrom(), 36);
        }
    }
}

TEST(PawnTest, EnPassantMoveExecution) {

    Board board;

    std::vector<Move> moves = {
            Move(48, 48 - 16, FLAG_PAWN_DBL_PUSH),
            Move(9, 9 + 16, FLAG_PAWN_DBL_PUSH),
            Move(32, 25, FLAG_CAPTURE),
            Move(8, 8 + 16, FLAG_PAWN_DBL_PUSH),
            Move(25, 16, FLAG_EP_CAPTURE),
    };

    std::string stateBeforeEP;
    for(Move move : moves) {
        if(move.getFlags() == FLAG_EP_CAPTURE) {
            stateBeforeEP = board.getBoardPrintable();
        }
        board.executeMove(move);
    }


    ASSERT_EQ(getCardinality(board.getOccupied()), 30);
    ASSERT_EQ(getCardinality(board.pawns[0]), 6);
    ASSERT_EQ(getCardinality(board.pawns[1]), 8);
    ASSERT_TRUE(board.pawns[1] & (1UL << 16));
    ASSERT_FALSE(board.pawns[0] & (1UL << 24));

    board.undoLastMove();

    ASSERT_EQ(board.getBoardPrintable(), stateBeforeEP);
    ASSERT_EQ(getCardinality(board.getOccupied()), 31);
    ASSERT_EQ(getCardinality(board.pawns[0]), 7);
    ASSERT_EQ(getCardinality(board.pawns[1]), 8);

    ASSERT_TRUE(board.pawns[1] & (1UL << 25));
    ASSERT_FALSE(board.pawns[1] & (1UL << 16));
    ASSERT_TRUE(board.pawns[0] & (1UL << 24));

}