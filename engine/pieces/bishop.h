#pragma once

#include <vector>
#include <array>

#include "utils.h"
#include "Move.h"


namespace Bishop {

    std::array<int, 64> pieceSquareTable();

    std::vector<Move> getMoves(U64 bishops, U64 ownPieces, U64 enemyPieces);
    U64 getTargets(U64 bishops, U64 ownPieces, U64 enemyPieces, bool countBlocked=false);
}