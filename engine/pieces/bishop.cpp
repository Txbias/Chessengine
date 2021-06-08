#include "bishop.h"

std::array<int, 64> Bishop::pieceSquareTable() {

    constexpr const std::array<int, 64> table {
            -20,-10,-10,-10,-10,-10,-10,-20,
            -10,  0,  0,  0,  0,  0,  0,-10,
            -10,  0,  5, 10, 10,  5,  0,-10,
            -10,  5,  5, 10, 10,  5,  5,-10,
            -10,  0, 10, 10, 10, 10,  0,-10,
            -10, 10, 10, 10, 10, 10, 10,-10,
            -10,  5,  0,  0,  0,  0,  5,-10,
            -20,-10,-10,-10,-10,-10,-10,-20,
    };

    return table;
}


std::vector<Move> Bishop::getMoves(U64 bishops, U64 ownPieces, U64 enemyPieces) {
    std::vector<Move> moves;

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
    std::vector<bitShiftFunction> directions = {
            northEast, northWest, southEast, southWest
    };

    return getSlidingTargets(bishops, ownPieces, enemyPieces, directions, countBlocked);
}
