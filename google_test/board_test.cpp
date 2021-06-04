#include "gtest/gtest.h"
#include "board.h"

#include <chrono>

TEST(BoardTest, MoveExecution) {
    Board board;

    std::vector<Move> moves = board.getAllMoves(0);
    for(Move move : moves) {
        int cardinalityBefore = getCardinality(board.getOccupied());
        board.executeMove(move);

        ASSERT_FALSE(move.isCapture());
        ASSERT_EQ(getCardinality(board.getOccupied()), cardinalityBefore);
        board.undoLastMove();
    }
}

TEST(BoardTest, MoveExecution2) {
    Board board;

    std::vector<Move> moves = board.getAllMoves(0);
    for(Move move : moves) {
        std::string boardStateBefore = board.getBoardPrintable();
        board.executeMove(move);

        std::vector<Move> newMoves = board.getAllMoves(1);
        for(Move newMove : newMoves) {
            std::string stateBefore = board.getBoardPrintable();
            board.executeMove(newMove);
            board.undoLastMove();
            ASSERT_EQ(stateBefore, board.getBoardPrintable());
        }

        board.undoLastMove();

        ASSERT_EQ(boardStateBefore, board.getBoardPrintable());
    }
}

void testMoveExecution(Board &board, int depth) {

    if(depth == 0) {
        return;
    }

    std::vector<Move> moves = board.getAllMoves(depth % 2);
    for(Move move : moves) {
        std::string stateBefore = board.getBoardPrintable();
        board.executeMove(move);
        testMoveExecution(board, depth - 1);

        board.undoLastMove();
        ASSERT_EQ(stateBefore, board.getBoardPrintable());
    }

}

TEST(BoardTest, MoveExecution3) {
    Board board;
    testMoveExecution(board, 4);
}


TEST(BoardTest, InCheckTest)  {
    Board board;

    Move move(13, 13+16, FLAG_QUIET);
    board.executeMove(move);

    board.printBoard();

    Move move2(52, 52-16, FLAG_QUIET);
    board.executeMove(move2);

    board.printBoard();

    Move move3(8, 16, FLAG_QUIET);
    board.executeMove(move3);

    board.printBoard();

    Move move4(59, 31, FLAG_QUIET);
    board.executeMove(move4);

    board.printBoard();

    ASSERT_TRUE(board.inCheck(0));
}

TEST(BoardTest, EvaluationTest) {
    Board board;

    ASSERT_EQ(board.valuePosition(0), 0);
    ASSERT_EQ(board.valuePosition(1), 0);

    Move move(8, 16, FLAG_QUIET);
    board.executeMove(move);

    ASSERT_EQ(board.valuePosition(0), -10);
    ASSERT_EQ(board.valuePosition(1), 10);
}

TEST(BoardTest, Crash) {
    Board board("rnbqk2r/pppp1pp1/4p2p/8/4P1n1/2PBBN2/PPP1QPPP/R3K2R b KQkq - 3 7");

    Move move = board.getBestMove(1);
    ASSERT_FALSE(move.getFrom() == 0 && move.getTo() == 0);
}

TEST(BoardTest, Crash2) {
    Board board("5Q2/6R1/2pNK3/2P5/pP4k1/3n4/8/8 b - - 6 50");

    Move move = board.getBestMove(1);
    ASSERT_FALSE(move.getFrom() == 0 && move.getTo() == 0);
}

TEST(BoardTest, CountMovesStartingPosition) {
    Board board;

    ASSERT_EQ(board.countMoves(0), 20);
    ASSERT_EQ(board.countMoves(1), 20);
}

TEST(BoardTest, CountMovesTest) {

    Board board;

    std::vector<Move> moves = board.getAllMoves(0);
    for(Move move : moves) {
        board.executeMove(move);

        int countMovesBlack = board.countMoves(1);
        ASSERT_EQ(countMovesBlack, board.getAllMoves(1).size());

        board.undoLastMove();
    }

    moves = board.getAllMoves(1);
    for(Move move : moves) {
        board.executeMove(move);

        int countWhiteMoves = board.countMoves(0);
        ASSERT_EQ(countWhiteMoves, board.getAllMoves(0).size());

        board.undoLastMove();
    }
}

TEST(FenTest, StartingPosition) {
    Board fenBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    Board board;

    ASSERT_EQ(fenBoard.getBoardPrintable(), board.getBoardPrintable());
}

TEST(FenTest, AfterFewMoves) {
    Board board;

    Move move1(12, 12+16, FLAG_PAWN_DBL_PUSH);
    board.executeMove(move1);

    Move move2(50, 50-16, FLAG_PAWN_DBL_PUSH);
    board.executeMove(move2);

    Move move3(6, 21, FLAG_QUIET);
    board.executeMove(move3);

    Board fenBoard("rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2");

    ASSERT_EQ(board.getBoardPrintable(), fenBoard.getBoardPrintable());
}

TEST(FenTest, EnPassant) {
    Board board("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");

    ASSERT_EQ(board.enPassantSquare, 20);
    ASSERT_EQ(board.enPassantTarget, 1UL << 20);
}

TEST(FenTest, CastlingRights) {
    Board board("rnbk2nr/pp1p1ppp/1p1bp3/8/8/B7/P1PPPPPP/q2QKBNR b K - 1 7");

    ASSERT_FALSE(board.kingMoved[0]);
    ASSERT_TRUE(board.kingMoved[1]);

    ASSERT_FALSE(board.rookMoved[0][0]);
    ASSERT_TRUE(board.rookMoved[0][1]);
    ASSERT_TRUE(board.rookMoved[1][0]);
    ASSERT_TRUE(board.rookMoved[1][1]);
}

TEST(FenTest, infiniteLoop) {
    Board board("rnbk2nr/pp1p1ppp/1p1bp3/8/8/B7/P1PPPPPP/q2QKBNR b K - 1 7");

    ASSERT_TRUE(board.queens[0] & (1UL << 3));
    ASSERT_FALSE(board.rooks[0] & (1UL << 3));

    auto begin = std::chrono::high_resolution_clock::now();
    board.getBestMove(1);
    auto end = std::chrono::high_resolution_clock::now();

    long durationInSeconds = std::chrono::duration_cast<std::chrono::seconds>(end - begin).count();

    ASSERT_TRUE(durationInSeconds <= 5);
}