#include "rook.h"

typedef unsigned long U64;

std::array<int, 64> Rook::pieceSquareTable() {
    constexpr std::array<int, 64> table = {
            0,  0,  0,  0,  0,  0,  0,  0,
            5, 10, 10, 10, 10, 10, 10,  5,
            -5,  0,  0,  0,  0,  0,  0, -5,
            -5,  0,  0,  0,  0,  0,  0, -5,
            -5,  0,  0,  0,  0,  0,  0, -5,
            -5,  0,  0,  0,  0,  0,  0, -5,
            -5,  0,  0,  0,  0,  0,  0, -5,
            0,  0,  5,  10,  10,  5,  0,  0
    };

    return table;
}

int Rook::getRookValue(const U64 pawns[2], int square, int team) {
    const static int BASE_VALUE = 500;
    int value = BASE_VALUE;

    int countPawnsEnemy = getCardinality(pawns[ENEMY(team)]);
    value += (8 - countPawnsEnemy) * 5;

    U64 wFileFill = fileFill(pawns[0]);
    U64 bFileFill = fileFill(pawns[1]);
    U64 fileFill = wFileFill | bFileFill;

    U64 rook = 1UL << square;

    if(!(rook & fileFill)) {
        value += 25;
    } else if(!(rook & wFileFill) || !(rook & bFileFill)) {
        value += 12;
    } else {
        value -= 12;
    }

    return value;
}

std::vector<Move> Rook::getMoves(U64 rooks, U64 ownPieces, U64 enemyPieces) {

    std::vector<Move> moves;

    std::vector<Move> movesNorth = getSlidingMovesNorth(rooks, ownPieces, enemyPieces);
    std::vector<Move> movesSouth = getSlidingMovesSouth(rooks, ownPieces, enemyPieces);
    std::vector<Move> movesEast = getSlidingMovesEast(rooks, ownPieces, enemyPieces);
    std::vector<Move> movesWest = getSlidingMovesWest(rooks, ownPieces, enemyPieces);

    moves.insert(moves.end(), std::begin(movesNorth), std::end(movesNorth));
    moves.insert(moves.end(), std::begin(movesSouth), std::end(movesSouth));
    moves.insert(moves.end(), std::begin(movesEast), std::end(movesEast));
    moves.insert(moves.end(), std::begin(movesWest), std::end(movesWest));

    return moves;
}

U64 Rook::getTargets(U64 rooks, U64 ownPieces, U64 enemyPieces, bool countBlocked) {
    std::vector<bitShiftFunction> directions = {
            northOne, eastOne, westOne, southOne
    };

    return getSlidingTargets(rooks, ownPieces, enemyPieces, directions, countBlocked);
}