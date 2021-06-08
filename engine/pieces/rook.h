#pragma once

#include <vector>
#include <array>

#include "utils.h"
#include "Move.h"

namespace Rook {

    typedef unsigned long U64;

    std::array<int, 64> pieceSquareTable();

    std::vector<Move> getMoves(U64 rooks, U64 ownPieces, U64 enemyPieces);
    U64 getTargets(U64 rooks, U64 ownPieces, U64 enemyPieces, bool countBlocked=false);
}