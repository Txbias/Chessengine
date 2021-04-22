#pragma once

#include <vector>

#include "Move.h"
#include "pieces/pawns.h"
#include "pieces/knight.h"

class Board {

    typedef unsigned long U64;

private:
    unsigned long occupied = 0;
    unsigned long white = 0;
    unsigned long black = 0;

    const int WHITE = 0;
    const int BLACK = 1;

    void initializePieces();
public:

    Board();

    unsigned long pawns[2];
    unsigned long knights[2];
    unsigned long bishops[2];
    unsigned long rooks[2];
    unsigned long queens[2];
    unsigned long kings[2];

    const int VALUE_PAWN = 100;
    const int VALUE_KNIGHT = 350;
    const int VALUE_BISHOP = 350;
    const int VALUE_ROOK = 525;
    const int VALUE_QUEEN = 1000;
    const int VALUE_KING = 10000;

    std::vector<Move> getAllMoves(int team);
    int valuePosition(int team);

    inline unsigned long getWhite() const {
        return white;
    }

    inline unsigned long getBlack() const {
        return black;
    }

    inline unsigned long getOccupied() const {
        return occupied;
    }
};