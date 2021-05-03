#pragma once

#include <vector>

#include "Move.h"
#include "utils.h"

namespace Queen {

    std::vector<Move> getMoves(U64 queens, U64 ownPieces, U64 enemyPieces);

}