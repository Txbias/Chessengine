#pragma once

#include <vector>

#include "Move.h"
#include "utils.h"

namespace King {

    std::vector<Move> getMoves(U64 king, U64 ownPieces, U64 enemyPieces);
    U64 getTargets(U64 king, U64 ownPieces);
}