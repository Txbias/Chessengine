#include "rook.h"

typedef unsigned long U64;

int Rook::getRookValue(const U64 pawns[2], int square, int team, int rookValue) {
    int value = rookValue;

    int countPawnsEnemy = getCardinality(pawns[ENEMY(team)]);
    value += (8 - countPawnsEnemy) * 5;

    U64 wFileFill = fileFill(pawns[0]);
    U64 bFileFill = fileFill(pawns[1]);
    U64 fileFill = wFileFill | bFileFill;

    U64 rook = 1UL << square;

    if(!(rook & fileFill)) {
        value += 15;
    } else if(!(rook & wFileFill) || !(rook & bFileFill)) {
        value += 6;
    } else {
        value -= 6;
    }

    return value;
}

std::vector<Move> Rook::getMoves(U64 rooks, U64 ownPieces, U64 enemyPieces) {

    std::vector<Move> moves;
    moves.reserve(28);

    if(rooks) do {
        int square = bitScanForward(rooks);
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
    } while (rooks &= rooks - 1);


    return moves;
}

U64 Rook::getTargets(U64 rooks, U64 ownPieces, U64 enemyPieces, bool countBlocked) {
    U64 targets = 0UL;
    U64 occupied = ownPieces | enemyPieces;

    if(rooks) do {
        int square = bitScanForward(rooks);

        U64 blockers = getRookMasks(square);
        blockers &= occupied;

        U64 key = (blockers * getRookMagic(square)) >> (64 - getRookIndexBit(square));
        targets |= getRookAttacks(square, key);
    } while(rooks &= rooks - 1);

    if(!countBlocked) {
        targets &= ~ownPieces;
    }

    return targets;
}