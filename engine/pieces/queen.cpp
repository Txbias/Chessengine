#include "queen.h"

std::vector<Move> Queen::getMoves(U64 queens, U64 ownPieces, U64 enemyPieces) {

    std::vector<Move> moves;
    moves.reserve(40);

    if(queens) do {
        int square = bitScanForward(queens);
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
    } while(queens &= queens - 1);

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