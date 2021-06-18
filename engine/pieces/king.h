#pragma once

#include <vector>
#include <array>

#include "Move.h"
#include "utils.h"

namespace King {

    std::array<int, 64> pieceSquareTable(U64 queens[2]);


    std::vector<Move> getMoves(U64 king, U64 ownPieces, U64 enemyPieces);
    std::vector<Move> getCastlingMoves(U64 king, U64 rooks, U64 occupied,
                                       U64 enemyAttackMap, int team,
                                       const bool kingMoved[2], bool rookMoved[2][2]);
    bool canCastleKingSide(U64 king, U64 rooks, U64 occupied, U64 enemyAttackMap,
                           int team, const bool kingMoved[2], bool rookMoved[2][2]);
    bool canCastleQueenSide(U64 king, U64 rooks, U64 occupied, U64 enemyAttackMap,
                            int team, const bool kingMoved[2], bool rookMoved[2][2]);
    U64 getTargets(U64 king, U64 ownPieces);
}