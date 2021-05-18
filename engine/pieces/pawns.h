#pragma once

#include <vector>
#include <array>

#include "Move.h"
#include "utils.h"


namespace Pawn {
    typedef unsigned long U64;

    const int WHITE = 0;
    const U64 notAFile = 0xfefefefefefefefe; // ~0x0101010101010101
    const U64 notHFile = 0x7f7f7f7f7f7f7f7f; // ~0x8080808080808080


    std::array<int, 64> pieceSquareTable();

    U64 noEaOne(U64 b);

    U64 soEaOne(U64 b);

    U64 soWeOne(U64 b);

    U64 noWeOne(U64 b);

    U64 whiteSinglePushTargets(U64 whitePawns, U64 empty);

    U64 blackSinglePushTargets(U64 blackPawns, U64 empty);

    U64 whiteDblPushTargets(U64 whitePawns, U64 empty);

    U64 blackDoublePushTargets(U64 blackPawns, U64 empty);

    U64 whitePawnsAble2Push(U64 whitePawns, U64 empty);

    U64 blackPawnsAble2Push(U64 blackPawns, U64 empty);

    U64 whitePawnsAble2DblPush(U64 whitePawns, U64 empty);

    U64 blackPawnsAble2DblPush(U64 blackPawns, U64 empty);

    U64 whitePawnsEastAttacks(U64 pawns);

    U64 blackPawnsEastAttacks(U64 pawns);

    U64 whitePawnsWestAttacks(U64 pawns);

    U64 blackPawnsWestAttacks(U64 pawns);

    U64 whitePawnAnyAttack(U64 pawns);

    U64 blackPawnsAnyAttack(U64 pawns);

    U64 getAttackTargets(U64 pawns, int team);

    U64 whitePawnsAble2CaptureAny(U64 whitePawns, U64 blackPieces);

    U64 blackPawnsAble2CaptureAny(U64 blackPawns, U64 whitePieces);

    std::vector<Move> getMovesWhite(U64 pawns, U64 empty, U64 enemyPieces);
    std::vector<Move> getMovesBlack(U64 pawns, U64 empty, U64 enemyPieces);

    std::vector<Move> getMoves(U64 pawns, U64 empty, U64 enemyPieces, int team);
}