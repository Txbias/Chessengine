#include "rook.h"

typedef unsigned long U64;


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
