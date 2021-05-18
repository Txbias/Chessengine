#include "gtest/gtest.h"
#include "board.h"

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

    ASSERT_EQ(board.valuePosition(0), 0);
    ASSERT_EQ(board.valuePosition(1), 0);

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