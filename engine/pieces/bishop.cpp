#include "bishop.h"

std::vector<Move> Bishop::getMoves(U64 bishops, U64 ownPieces, U64 enemyPieces) {
    std::vector<Move> moves;
    moves.reserve(28);

    if(bishops) do {
        int square = bitScanForward(bishops);
        U64 bitboard = 1UL << square;

        U64 targets = getTargets(bitboard, ownPieces, enemyPieces);

        while(targets) {
            int destinationSquare = bitScanForward(targets);

            unsigned int flag = FLAG_QUIET;
            if(enemyPieces & (1UL << destinationSquare)) {
                flag = FLAG_CAPTURE;
            }

            moves.emplace_back(Move(square, destinationSquare, flag));

            // delete used bit
            targets &= targets - 1;
        }
    } while(bishops &= bishops - 1);


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
