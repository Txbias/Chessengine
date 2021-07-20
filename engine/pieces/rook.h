#pragma once

#include <vector>

#include "utils.h"
#include "move.h"

namespace Rook {

    typedef unsigned long U64;

    int getRookValue(const U64 pawns[2], int square, int team, int rookValue);

    std::vector<Move> getMoves(U64 rooks, U64 ownPieces, U64 enemyPieces);
    U64 getTargets(U64 rooks, U64 ownPieces, U64 enemyPieces, bool countBlocked=false);
}