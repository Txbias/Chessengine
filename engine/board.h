#pragma once

#include <vector>
#include <stack>
#include <cctype>
#include <map>
#include <mutex>

#include "Move.h"
#include "thread_pool.h"
#include "pawns.h"
#include "knight.h"
#include "rook.h"
#include "bishop.h"
#include "queen.h"
#include "king.h"

#define PENALTY_BAD_DRAW -5000

void initializeHashKeys();

class Board {

    typedef unsigned long U64;

private:
    unsigned long occupied = 0;

    int SEARCH_DEPTH = 3;

    static const int WHITE = 0;
    static const int BLACK = 1;

    void initializePieces();

    unsigned int getPieceType(unsigned int targetSquare, int team);
    unsigned long* getTargetPieces(unsigned int targetSquare, int team);
    int getTeam(unsigned int square);
    U64 getTargetMap(int team, bool includeKing=true, bool countBlocked=false);
    int alphaBeta(int alpha, int beta, int depthLeft, int team, Move &bestMove);
    int pvSearch(int alpha, int beta, int depthLeft, int team, Move &bestMove);
    int zwSearch(int beta, int depthLeft);

    int quiesce(int alpha, int beta, int depth);
public:

    Board();
    Board(const std::string& fen);

    std::stack<Move> moves;
    std::map<U64, int> positions;

    unsigned long pieces[2];
    unsigned long pawns[2];
    unsigned long knights[2];
    unsigned long bishops[2];
    unsigned long rooks[2];
    unsigned long queens[2];
    unsigned long kings[2];

    bool kingMoved[2];
    bool rookMoved[2][2];
    bool threeFoldRepetition = false;

    int VALUE_MOBILITY = 10;
    int VALUE_PAWN = 100;
    int VALUE_KNIGHT = 320;
    int VALUE_BISHOP = 330;
    int VALUE_ROOK = 500;
    int VALUE_QUEEN = 900;
    int VALUE_KING = 20000;

    int amountFullMoves = 1;
    int actingTeam = 0;

    U64 enPassantTarget = 0UL;
    unsigned int enPassantSquare = 0;

    U64 getPositionHash();

    std::vector<Move> getAllMoves(int team);
    Move getBestMove(int team);

    bool inCheck(int team);
    bool checkMate(int team);
    bool isStaleMate(int team);

    void executeMove(Move move);
    void executeUserMove(Move move);
    void undoLastMove();
    void printBoard();
    std::string getBoardPrintable();
    std::string getFENString();

    int valuePosition(int team);
    int countMoves(int team);
    int countDoublePawns(int team);

    static int valueMove(Board boardCopy, const Move &move, int team);

    inline unsigned long getOccupied() const {
        return occupied;
    }

    inline void setSearchDepth(int depth) {
        SEARCH_DEPTH = depth;
    }

    inline int getSearchDepth() const {
        return SEARCH_DEPTH;
    }
};