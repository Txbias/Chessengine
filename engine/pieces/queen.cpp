#include "queen.h"

std::vector<Move> Queen::getMoves(U64 queens, U64 ownPieces, U64 enemyPieces) {

    std::vector<Move> moves;
    moves.reserve(40);

    std::vector<Move> movesNorth = getSlidingMovesNorth(queens, ownPieces, enemyPieces);
    std::vector<Move> movesEast = getSlidingMovesEast(queens, ownPieces, enemyPieces);
    std::vector<Move> movesSouth = getSlidingMovesSouth(queens, ownPieces, enemyPieces);
    std::vector<Move> movesWest = getSlidingMovesWest(queens, ownPieces, enemyPieces);

    std::vector<Move> movesNorthEast = getSlidingMovesNorthEast(queens, ownPieces, enemyPieces);
    std::vector<Move> movesSouthEast = getSlidingMovesSouthEast(queens, ownPieces, enemyPieces);
    std::vector<Move> movesSouthWest = getSlidingMovesSouthWest(queens, ownPieces, enemyPieces);
    std::vector<Move> movesNorthWest = getSlidingMovesNorthWest(queens, ownPieces, enemyPieces);

    moves.insert(moves.end(), std::begin(movesNorth), std::end(movesNorth));
    moves.insert(moves.end(), std::begin(movesEast), std::end(movesEast));
    moves.insert(moves.end(), std::begin(movesSouth), std::end(movesSouth));
    moves.insert(moves.end(), std::begin(movesWest), std::end(movesWest));

    moves.insert(moves.end(), std::begin(movesNorthEast), std::end(movesNorthEast));
    moves.insert(moves.end(), std::begin(movesSouthEast), std::end(movesSouthEast));
    moves.insert(moves.end(), std::begin(movesSouthWest), std::end(movesSouthWest));
    moves.insert(moves.end(), std::begin(movesNorthWest), std::end(movesNorthWest));

    return moves;
}

U64 Queen::getTargets(U64 queens, U64 ownPieces, U64 enemyPieces, bool countBlocked) {
    U64 occupied = ownPieces | enemyPieces;
    U64 targets = 0UL;

    if(queens) do {
        int square = bitScanForward(queens);

        // diagonal blockers
        U64 diagonalBlockers = getBishopMasks(square);
        diagonalBlockers &= occupied;

        // straight blockers
        U64 straightBlockers = getRookMasks(square);
        straightBlockers &= occupied;

        U64 key = (diagonalBlockers * getBishopMagic(square)) >> (64 - getBishopIndexBit(square));
        targets |= getBishopAttacks(square, key);

        key = (straightBlockers * getRookMagic(square)) >> (64 - getRookIndexBit(square));
        targets |= getRookAttacks(square, key);

    } while(queens &= queens - 1);

    if(!countBlocked) {
        targets &= ~ownPieces;
    }

    return targets;
}