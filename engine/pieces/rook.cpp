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

U64 Rook::getTargets(U64 rooks, U64 ownPieces, U64 enemyPieces) {
    std::vector<bitShiftFunction> directions = {
            northOne, eastOne, westOne, southOne
    };

    return getSlidingTargets(rooks, ownPieces, enemyPieces, directions);
}