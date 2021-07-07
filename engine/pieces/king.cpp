#include "king.h"

std::vector<Move> King::getMoves(U64 king, U64 ownPieces, U64 enemyPieces) {

    unsigned int from = bitScanForward(king);
    U64 attackedFields = getTargets(king, ownPieces);

    std::vector<int> setBits = getSetBits(attackedFields);
    std::vector<Move> moves;
    moves.reserve(setBits.size());

    for(int i : setBits) {
        unsigned int flag = getFlag(i, enemyPieces);

        Move move(from, i, flag);
        moves.emplace_back(move);
    }

    return moves;
}

std::vector<Move> King::getCastlingMoves(U64 king, U64 rooks, U64 occupied, U64 enemyAttackMap,
                                         int team, const bool kingMoved[2], bool rookMoved[2][2]) {

    std::vector<Move> moves;

    if(canCastleKingSide(king, rooks, occupied, enemyAttackMap, team, kingMoved, rookMoved)) {
        if(team == 0) {
            Move move(4, 6, FLAG_KING_CASTLE);
            moves.emplace_back(move);
        } else if(team == 1) {
            Move move(60, 62, FLAG_KING_CASTLE);
            moves.emplace_back(move);
        }
    }

    if(canCastleQueenSide(king, rooks, occupied, enemyAttackMap, team, kingMoved, rookMoved)) {
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

bool King::canCastleKingSide(U64 king, U64 rooks, U64 occupied, U64 enemyAttackMap,
                             int team, const bool kingMoved[2], bool rookMoved[2][2]) {

    if(kingMoved[team]) return false;
    if(rookMoved[team][0]) return false;

    if(team == 0) {
        if((!(king & (1UL << 4)) )|| (!(rooks & (1UL << 7)))) {
            return false;
        }
    } else {
        if((!(king & (1UL << 60))) || (!(rooks & (1UL << 63)))) {
            return false;
        }
    }

    static U64 castlingSquares;
    static U64 targetSquares;

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

bool King::canCastleQueenSide(U64 king, U64 rooks, U64 occupied, U64 enemyAttackMap,
                              int team, const bool kingMoved[2], bool rookMoved[2][2]) {

    if(kingMoved[team]) return false;
    if(rookMoved[team][1]) return false;

    if(team == 0) {
        if((!(king & (1UL << 4))) || (!(rooks & (1UL << 0)))) {
            return false;
        }
    } else {
        if((!(king & (1UL << 60))) || (!(rooks & (1UL << 56)))) {
            return false;
        }
    }

    static U64 castlingSquares;
    static U64 targetSquares;

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
    U64 attackedFields = king;

    attackedFields |= westOne(king);
    attackedFields |= eastOne(king);
    attackedFields |= northOne(attackedFields);
    attackedFields |= southOne(attackedFields);

    attackedFields &= ~ownPieces;

    return attackedFields;
}