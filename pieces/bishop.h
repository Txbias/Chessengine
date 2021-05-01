#pragma once

#include <vector>

#include "../utils.h"
#include "../Move.h"


namespace Bishop {

    std::vector<Move> getMoves(U64 bishops, U64 ownPieces, U64 enemyPieces);

}