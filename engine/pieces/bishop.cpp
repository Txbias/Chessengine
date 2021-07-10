#include "bishop.h"

std::vector<Move> Bishop::getMoves(U64 bishops, U64 ownPieces, U64 enemyPieces) {
    std::vector<Move> moves;
    moves.reserve(28);

    std::vector<Move> movesNorthEast = getSlidingMovesNorthEast(bishops, ownPieces, enemyPieces);
    std::vector<Move> movesNorthWest = getSlidingMovesNorthWest(bishops, ownPieces, enemyPieces);
    std::vector<Move> movesSouthEast = getSlidingMovesSouthEast(bishops, ownPieces, enemyPieces);
    std::vector<Move> movesSouthWest = getSlidingMovesSouthWest(bishops, ownPieces, enemyPieces);

    moves.insert(moves.end(), std::begin(movesNorthEast), std::end(movesNorthEast));
    moves.insert(moves.end(), std::begin(movesNorthWest), std::end(movesNorthWest));
    moves.insert(moves.end(), std::begin(movesSouthEast), std::end(movesSouthEast));
    moves.insert(moves.end(), std::begin(movesSouthWest), std::end(movesSouthWest));

    return moves;
}

U64 Bishop::getTargets(U64 bishops, U64 ownPieces, U64 enemyPieces, bool countBlocked) {
    U64 targets = 0UL;
    U64 occupied = ownPieces | enemyPieces;

    if(bishops) do {
        int square = bitScanForward(bishops);

        U64 blockers = getBishopMasks(square);
        blockers &= occupied;

        U64 key = (blockers * getBishopMagic(square)) >> (64 - getBishopIndexBit(square));
        targets |= getBishopAttacks(square, key);
    } while(bishops &= bishops - 1);

    if(!countBlocked) {
        targets &= ~ownPieces;
    }

    return targets;
}
