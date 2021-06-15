#pragma once

#include <vector>
#include <stack>
#include <cctype>
#include <map>

#include "Move.h"
#include "position.h"
#include "pawns.h"
#include "knight.h"
#include "rook.h"
#include "bishop.h"
#include "queen.h"
#include "king.h"

#define ENEMY(team) (team == 0  ? 1 : 0)

class Board {

    typedef unsigned long U64;

private:
    unsigned long occupied = 0;

    const int WHITE = 0;
    const int BLACK = 1;

    int actingTeam = 0;

    void initializePieces();

    unsigned int getPieceType(unsigned int targetSquare, int team);
    unsigned long* getTargetPieces(unsigned int targetSquare, int team);
    int getTeam(unsigned int square);
    U64 getTargetMap(int team, bool includeKing=true, bool countBlocked=false);
    int alphaBeta(int alpha, int beta, int depthLeft, int team, Move &bestMove);
    int quiesce(int alpha, int beta, int depth);
public:

    Board();
    Board(const std::string& fen);

    std::stack<Move> moves;
    std::map<Position, int> positions;

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

    const int VALUE_MOBILITY = 10;
    const int VALUE_PAWN = 100;
    const int VALUE_KNIGHT = 320;
    const int VALUE_BISHOP = 330;
    const int VALUE_ROOK = 500;
    const int VALUE_QUEEN = 900;
    const int VALUE_KING = 20000;

    int amountFullMoves = 1;

    U64 enPassantTarget = 0UL;
    unsigned int enPassantSquare = 0;

    std::vector<Move> getAllMoves(int team);
    Move getBestMove(int team);
    Position getCurrentPosition();

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

    inline unsigned long getOccupied() const {
        return occupied;
    }
};