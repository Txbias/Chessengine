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

TEST(BoardTest, MateInOne) {
    Board board("5K2/p2r4/2r5/k5p1/8/8/8/3q4 b - - 12 90");

    Move move = board.getBestMove(1);

    std::cout << Move::toNotation(move) << std::endl;
    ASSERT_EQ(Move::toNotation(move), "c6c8");
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

    Move move2(52, 52-16, FLAG_QUIET);
    board.executeMove(move2);

    Move move3(8, 16, FLAG_QUIET);
    board.executeMove(move3);

    Move move4(59, 31, FLAG_QUIET);
    board.executeMove(move4);

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

TEST(BoardTest, Crash3) {
    Board board("5k2/2K5/5Q2/2p5/p1P5/P7/8/8 b - - 6 75");

    Move move = board.getBestMove(1);

    ASSERT_FALSE(move.getFrom() == 0 && move.getTo() == 0);
}

TEST(BoardTest, Crash4) {
    Board board("r1bqkb1r/ppp1pppp/2n2P2/8/3p4/2N2N2/PPPP1PPP/R1BQKB1R b KQkq - 0 5");

    board.getBestMove(1);
}

TEST(BoardTest, MovesKingInCheck) {
    Board board("2Q2b1r/4p1p1/pk6/1p5p/1Pp4P/4PB2/P1P3P1/2K5 b - - 0 30");

    Move move = board.getBestMove(1);
    ASSERT_FALSE(Move::toNotation(move) == "b6c7");
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

TEST(BoardTest, RepetitionDraw) {
    Board board("2k5/5pp1/3Q4/2K5/2P4p/4qP2/P7/8 w - - 3 47");

    board.executeMove(Move(34, 25, FLAG_QUIET));
    board.executeMove(Move(20, 4, FLAG_QUIET));
    board.executeMove(Move(25, 34, FLAG_QUIET));
    board.executeMove(Move(4, 20, FLAG_QUIET));
    board.executeMove(Move(34, 25, FLAG_QUIET));
    board.executeMove(Move(20, 4, FLAG_QUIET));
    board.executeMove(Move(25, 34, FLAG_QUIET));
    ASSERT_FALSE(board.threeFoldRepetition);
    board.executeMove(Move(4, 20, FLAG_QUIET));

    ASSERT_TRUE(board.threeFoldRepetition);
    board.undoLastMove();

    ASSERT_FALSE(board.threeFoldRepetition);
}

TEST(BoardTest, StaleMate) {
    Board board("7r/p2n1k1p/2pb3p/1r1n4/2K3p1/8/3q4/8 w - - 2 30");

    ASSERT_TRUE(board.isStaleMate(0));
}

TEST(BoardTest, NotStaleMate) {
    Board board("7r/p2n1k1p/2pb3p/1r1n4/2K3p1/8/7q/8 b - - 1 29");

    ASSERT_FALSE(board.isStaleMate(0));
}

TEST(BoardTest, AvoidStalemate) {
    Board board("7r/p2n1k1p/2pb3p/1r1n4/2K3p1/8/7q/8 b - - 1 29");

    Move move = board.getBestMove(1);
    ASSERT_NE(Move::toNotation(move), "h2d2");
}

TEST(BoardTest, FullMoveCount) {
    Board board;

    for(int i = 0; i < 5; i++) {
        Move move = board.getAllMoves(i % 2)[0];
        board.executeMove(move);
    }

    ASSERT_EQ(board.amountFullMoves, 3);
}

TEST(BoardTest, InCheck) {
    Board board("1r1qr3/2p2pp1/5k1p/3p4/P1pB2Q1/7P/1PP2PP1/4R1K1 b - - 7 25");

    Move move = board.getBestMove(1);

    ASSERT_FALSE(Move::toNotation(move) == "e8e1");
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

    ASSERT_TRUE(durationInSeconds <= 15);
}

TEST(FenTest, FullMoveCount) {
    Board board("8/5k2/3p4/1p1Pp2p/pP2Pp1P/P4P1K/8/8 b - - 99 50");

    ASSERT_EQ(board.amountFullMoves, 50);

    Board board2("8/5k2/3p4/1p1Pp2p/pP2Pp1P/P4P1K/8/8 b - - 99 9");

    ASSERT_EQ(board2.amountFullMoves, 9);
}

TEST(FenGenerationTest, StartingPosition) {
    Board board;

    ASSERT_EQ(board.getFENString(),
              "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

TEST(FenGenerationTest, AfterOneMove) {
    Board board;

    board.executeMove(Move(12, 12 + 16, FLAG_PAWN_DBL_PUSH));
    ASSERT_EQ(board.getFENString(),
              "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
}

TEST(FenGenerationTest, castling) {
    std::string fen = "rnbk2nr/pp1p1ppp/1p1bp3/8/8/B7/P1PPPPPP/q2QKBNR b K - 0 1";
    Board board(fen);

    ASSERT_EQ(board.getFENString(), fen);
}

TEST(CheckMate, CanBeBlocked) {
    Board board("rnb1kbnr/pp1ppppp/8/q1p5/3P4/8/PPP1PPPP/RNBQKBNR w KQkq - 1 3");

    ASSERT_FALSE(board.checkMate(0));
}

TEST(CheckMate, mate) {
    Board board("rnbqkbnr/ppppp2p/8/5ppQ/8/3PP3/PPP2PPP/RNB1KBNR b KQkq - 1 3");

    ASSERT_TRUE(board.checkMate(1));
}

TEST(CheckMate, mate2) {
    Board board("5K2/p2r4/2r5/k5p1/8/8/8/3q4 w - - 12 90");
    board.executeMove(Move::fromNotation("c6c8"));

    ASSERT_TRUE(board.checkMate(0));
}

TEST(CheckMate, mate3) {
    Board board("7r/p4k1p/2pb3p/3nn3/1r1K2p1/8/7q/8 w - - 4 31");

    ASSERT_TRUE(board.checkMate(0));
}

TEST(HashingTest, shouldNotChange) {
    Board board;

    ASSERT_EQ(board.getPositionHash(), board.getPositionHash());
}

TEST(HashingTest, shouldChange) {
    Board board;

    unsigned long hashBefore = board.getPositionHash();

    board.executeUserMove(Move::fromNotation("e2e4"));

    ASSERT_NE(hashBefore, board.getPositionHash());
}

TEST(HashingTest, undoMove) {
    Board board;

    unsigned long hashBefore = board.getPositionHash();

    board.executeUserMove(Move::fromNotation("e2e4"));
    board.undoLastMove();

    ASSERT_EQ(hashBefore, board.getPositionHash());
}