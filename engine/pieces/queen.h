#pragma once

#include <vector>
#include <array>

#include "Move.h"
#include "utils.h"

namespace Queen {
    typedef U64(*bitShiftFunction)(U64);

    std::array<int, 64> pieceSquareTable();

    std::vector<Move> getMoves(U64 queens, U64 ownPieces, U64 enemyPieces);
    U64 getTargets(U64 queens, U64 ownPieces, U64 enemyPieces);

}