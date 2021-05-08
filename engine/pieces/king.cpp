#include "king.h"

std::vector<Move> King::getMoves(U64 king, U64 ownPieces, U64 enemyPieces) {

    unsigned int from;

    for(unsigned int i = 0; i < 64; i++) {
        if(king & (1UL << i)) {
            from = i;
        }
    }

    U64 attackedFields = getTargets(king, ownPieces);

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

std::vector<Move> King::getCastlingMoves(U64 occupied, U64 enemyAttackMap, int team,
                                         bool kingMoved[2], bool rookMoved[2][2]) {

    std::vector<Move> moves;

    if(canCastleKingSide(occupied, enemyAttackMap, team, kingMoved, rookMoved)) {
        if(team == 0) {
            Move move(4, 6, FLAG_KING_CASTLE);
            moves.emplace_back(move);
        } else if(team == 1) {
            Move move(60, 62, FLAG_KING_CASTLE);
            moves.emplace_back(move);
        }
    }

    if(canCastleQueenSide(occupied, enemyAttackMap, team, kingMoved, rookMoved)) {
        if(team == 0) {
            Move move(4, 2, FLAG_QUEEN_CASTLE);
            moves.emplace_back(move);
        } else if(team == 1) {
            Move move(60, 58, FLAG_QUEEN_CASTLE);
            moves.emplace_back(move);
        }
    }

    return moves;
}

bool King::canCastleKingSide(U64 occupied, U64 enemyAttackMap, int team,
                             const bool kingMoved[2], bool rookMoved[2][2]) {

    if(kingMoved[team]) return false;
    if(rookMoved[team][1]) return false;

    U64 castlingSquares;
    U64 targetSquares;

    if(team == 0) {
        targetSquares = 1UL << 5 | 1UL << 6;
        castlingSquares = 1UL << 4 | 1UL << 5 | 1UL << 6;
    } else {
        targetSquares = 1UL << 61 | 1UL << 62;
        castlingSquares = 1UL << 60 | 1UL << 61 | 1UL << 62;
    }

    if(enemyAttackMap & castlingSquares) {
        // King can't pass trough check
        return false;
    }

    if(occupied & targetSquares) {
        // Pieces between king and rook
        return false;
    }

    return true;
}

bool King::canCastleQueenSide(U64 occupied, U64 enemyAttackMap, int team,
                              const bool kingMoved[2], bool rookMoved[2][2]) {

    if(kingMoved[team]) return false;
    if(rookMoved[team][1]) return false;

    U64 castlingSquares;
    U64 targetSquares;

    if(team == 0) {
        targetSquares = 1UL << 1 | 1UL << 2 | 1UL << 3;
        castlingSquares = 1UL << 2 | 1UL << 3 | 1UL << 4;
    } else {
        targetSquares = 1UL << 57 | 1UL << 58 | 1UL << 59;
        castlingSquares = 1UL << 58 | 1UL << 59 | 1UL << 60;
    }

    if(enemyAttackMap & castlingSquares) {
        // King can't pass trough check
        return false;
    }

    if(occupied & targetSquares) {
        // Pieces between king and rook
        return false;
    }

    return true;
}

U64 King::getTargets(U64 king, U64 ownPieces) {
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

    return attackedFields;
}