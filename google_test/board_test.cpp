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