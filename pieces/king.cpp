#include "king.h"

std::vector<Move> King::getMoves(U64 king, U64 ownPieces, U64 enemyPieces) {

    unsigned int from;

    for(unsigned int i = 0; i < 64; i++) {
        if(king & (1UL << i)) {
            from = i;
        }
    }

    U64 attackedFields = 0UL;

    attackedFields |= northOne(king);
    attackedFields |= southOne(king);
    attackedFields |= westOne(king);
    attackedFields |= eastOne(king);

    attackedFields |= northEast(king);
    attackedFields |= northWest(king);
    attackedFields |= southEast(king);
    attackedFields |= southWest(king);

    attackedFields &= ~ownPieces;

    std::vector<Move> moves;

    for(unsigned int i = 0; i < 64; i++) {
        if(!(attackedFields & (1UL << i))) {
            continue;
        }

        unsigned int flag = getFlag(i, enemyPieces);

        Move move(from, i, flag);
        moves.emplace_back(move);
    }

    return moves;
}