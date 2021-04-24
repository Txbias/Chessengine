#pragma once

#include <vector>

#include "../Move.h"
#include "../utils.h"

namespace Knight {
    typedef unsigned long U64;

    const U64 notAFile = ~setFile(0UL, 0);
    const U64 notABFile = ~setFile(~notAFile, 1);
    const U64 notHFile = ~setFile(0UL, 7);
    const U64 notGHFile = ~setFile(~notHFile, 6);
    U64 noNoEa(U64 b);
    U64 noEaEa(U64 b);
    U64 soEaEa(U64 b);
    U64 soSoEa(U64 b);
    U64 noNoWe(U64 b);
    U64 noWeWe(U64 b);
    U64 soWeWe(U64 b);
    U64 soSoWe(U64 b);

    std::string getBits(U64 bitboard);

    U64 getKnightTargets(U64 knights, U64 ownPieces);
    U64 getOriginKnights(U64 knights, U64 target, U64 ownPieces);

    std::vector<Move> getMoves(U64 knights, U64 enemyPieces, U64 ownPieces);

}
