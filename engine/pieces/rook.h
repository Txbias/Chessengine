#pragma once

#include <vector>

#include "utils.h"
#include "Move.h"

namespace Rook {

    typedef unsigned long U64;

    std::vector<Move> getMoves(U64 rooks, U64 ownPieces, U64 enemyPieces);

}