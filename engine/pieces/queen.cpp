#include "queen.h"

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