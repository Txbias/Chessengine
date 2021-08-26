#pragma once

#include <vector>
#include <array>

#include "move.h"
#include "utils.h"


namespace Pawn {

    std::array<int, 64> pieceSquareTable();

    U64 wPassedPawns(U64 pawns[2]);
    U64 bPassedPawns(U64 pawns[2]);
    U64 passedPawns(U64 pawns[2], int team);

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

    U64 getMoveTargets(U64 pawns, U64 empty, int team);

    U64 whitePawnsAble2CaptureAny(U64 whitePawns, U64 blackPieces);

    U64 blackPawnsAble2CaptureAny(U64 blackPawns, U64 whitePieces);

    // Helper functions for generating moves for black or white pawns
    std::vector<Move> getMovesWhite(U64 pawns, U64 empty, U64 enemyPieces, U64 enPassantTarget);
    std::vector<Move> getMovesBlack(U64 pawns, U64 empty, U64 enemyPieces, U64 enPassantTarget);

    /**
     * Calculates all moves possible for the given configuration of pieces
     * @param pawns Bitboard with all own pawns
     * @param empty Bitboard that has a 1 for all empty squares
     * @param enemyPieces Bitboard of all enemy pieces
     * @param enPassantTarget Bitboard with the current en passant target square marked
     * @param team the acting team
     * @return All possible and legal pawn moves
     */
    std::vector<Move> getMoves(U64 pawns, U64 empty, U64 enemyPieces,
                               U64 enPassantTarget, int team);
}