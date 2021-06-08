#include "queen.h"


std::array<int, 64> Queen::pieceSquareTable() {

    constexpr const std::array<int, 64> table = {
            -20,-10,-10, -5, -5,-10,-10,-20,
            -10,  0,  0,  0,  0,  0,  0,-10,
            -10,  0,  5,  5,  5,  5,  0,-10,
            -5,  0,  5,  5,  5,  5,  0, -5,
            0,  0,  5,  5,  5,  5,  0, -5,
            -10,  5,  5,  5,  5,  5,  0,-10,
            -10,  0,  5,  0,  0,  0,  0,-10,
            -20,-10,-10, -5, -5,-10,-10,-20,
    };

    return table;
}

std::vector<Move> Queen::getMoves(U64 queens, U64 ownPieces, U64 enemyPieces) {

    std::vector<Move> moves;

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

    std::vector<bitShiftFunction> directions = {
            northOne, southOne, eastOne, westOne,
            northEast, northWest, southEast, southWest
    };

    return getSlidingTargets(queens, ownPieces, enemyPieces, directions, countBlocked);
}