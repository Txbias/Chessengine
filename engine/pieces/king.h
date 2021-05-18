#pragma once

#include <vector>
#include <array>

#include "Move.h"
#include "utils.h"

namespace King {



    /*int pieceSquareTableEndGame[64] {
            -50,-40,-30,-20,-20,-30,-40,-50,
            -30,-20,-10,  0,  0,-10,-20,-30,
            -30,-10, 20, 30, 30, 20,-10,-30,
            -30,-10, 30, 40, 40, 30,-10,-30,
            -30,-10, 30, 40, 40, 30,-10,-30,
            -30,-10, 20, 30, 30, 20,-10,-30,
            -30,-30,  0,  0,  0,  0,-30,-30,
            -50,-30,-30,-30,-30,-30,-30,-50,
    };*/

    std::array<int, 64> pieceSquareTableMiddleGame();


    std::vector<Move> getMoves(U64 king, U64 ownPieces, U64 enemyPieces);
    std::vector<Move> getCastlingMoves(U64 occupied, U64 enemyAttackMap, int team,
                                       bool kingMoved[2], bool rookMoved[2][2]);
    bool canCastleKingSide(U64 occupied, U64 enemyAttackMap, int team,
                        const bool kingMoved[2], bool rookMoved[2][2]);
    bool canCastleQueenSide(U64 occupied, U64 enemyAttackMap, int team,
                        const bool kingMoved[2], bool rookMoved[2][2]);
    U64 getTargets(U64 king, U64 ownPieces);
}