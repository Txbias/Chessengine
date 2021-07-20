#pragma once

#include <vector>
#include <array>

#include "move.h"
#include "utils.h"


namespace Knight {
    typedef unsigned long U64;

    std::array<int, 64> pieceSquareTable();

    const U64 notAFile = 0xFEFEFEFEFEFEFEFE;
    const U64 notABFile = 0xFCFCFCFCFCFCFCFC;
    const U64 notHFile = 0x7F7F7F7F7F7F7F7F;
    const U64 notGHFile = 0x3F3F3F3F3F3F3F3F;
    constexpr U64 noNoEa(U64 b);
    constexpr U64 noEaEa(U64 b);
    constexpr U64 soEaEa(U64 b);
    constexpr U64 soSoEa(U64 b);
    constexpr U64 noNoWe(U64 b);
    constexpr U64 noWeWe(U64 b);
    constexpr U64 soWeWe(U64 b);
    constexpr U64 soSoWe(U64 b);

    U64 getTargets(U64 knights, U64 ownPieces);
    U64 getOriginKnights(U64 knights, U64 target, U64 ownPieces);

    std::vector<Move> getMoves(U64 knights, U64 enemyPieces, U64 ownPieces);
}
