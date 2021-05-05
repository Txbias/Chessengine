
#include "pawns.h"

typedef unsigned long U64;

U64 Pawn::noEaOne(U64 b) { return (b << 9) & notAFile; }

U64 Pawn::soEaOne(U64 b) { return (b >> 7) & notAFile; }

U64 Pawn::soWeOne(U64 b) { return (b >> 9) & notHFile; }

U64 Pawn::noWeOne(U64 b) { return (b << 7) & notHFile; }

U64 nortOne(U64 bitboard) {
    return bitboard << 8;
}

U64 soutOne(U64 bitboard) {
    return bitboard >> 8;
}

U64 Pawn::whiteSinglePushTargets(U64 whitePawns, U64 empty) {
    return nortOne(whitePawns) & empty;
}

U64 Pawn::blackSinglePushTargets(U64 blackPawns, U64 empty) {
    return soutOne(blackPawns) & empty;
}

U64 Pawn::whiteDblPushTargets(U64 whitePawns, U64 empty) {
    U64 rank4 = 0;
    rank4 = setRow(rank4, 3);

    U64 singlePush = whiteSinglePushTargets(whitePawns, empty);
    return nortOne(singlePush) & empty & rank4;
}

U64 Pawn::blackDoublePushTargets(U64 blackPawns, U64 empty) {
    U64 rank5 = 0;
    rank5 = setRow(rank5, 4);
    U64 singlePush = blackSinglePushTargets(blackPawns, empty);
    return soutOne(singlePush) & empty & rank5;
}

U64 Pawn::whitePawnsAble2Push(U64 whitePawns, U64 empty) {
    return soutOne(empty) & whitePawns;
}

U64 Pawn::blackPawnsAble2Push(U64 blackPawns, U64 empty) {
    return nortOne(empty) & blackPawns;
}

U64 Pawn::whitePawnsAble2DblPush(U64 whitePawns, U64 empty) {
    U64 rank4 = 0;
    rank4 = setRow(rank4, 3);

    U64 emptyRank3 = soutOne(empty & rank4) & empty;
    return whitePawnsAble2Push(whitePawns, emptyRank3);
}

U64 Pawn::blackPawnsAble2DblPush(U64 blackPawns, U64 empty) {
    U64 rank5 = 0;
    rank5 = setRow(rank5, 4);
    U64 emptyRank6 = nortOne(empty & rank5) & empty;
    return blackPawnsAble2Push(blackPawns, emptyRank6);
}

U64 Pawn::whitePawnsEastAttacks(U64 pawns) {
    return noEaOne(pawns);
}

U64 Pawn::blackPawnsEastAttacks(U64 pawns) {
    return soEaOne(pawns);
}

U64 Pawn::whitePawnsWestAttacks(U64 pawns) {
    return noWeOne(pawns);
}

U64 Pawn::blackPawnsWestAttacks(U64 pawns) {
    return soWeOne(pawns);
}

U64 Pawn::whitePawnAnyAttack(U64 pawns) {
    return whitePawnsEastAttacks(pawns) | whitePawnsWestAttacks(pawns);
}

U64 Pawn::blackPawnsAnyAttack(U64 pawns) {
    return blackPawnsEastAttacks(pawns) | blackPawnsWestAttacks(pawns);
}

U64 Pawn::getAttackTargets(U64 pawns, int team) {
    return team == WHITE ? whitePawnAnyAttack(pawns) : blackPawnsAnyAttack(pawns);
}

U64 Pawn::whitePawnsAble2CaptureAny(U64 whitePawns, U64 blackPieces) {
    return whitePawns & blackPawnsAnyAttack(blackPieces);
}

U64 Pawn::blackPawnsAble2CaptureAny(U64 blackPawns, U64 whitePieces) {
    return blackPawns & whitePawnAnyAttack(whitePieces);
}

std::vector<Move> Pawn::getMovesWhite(U64 pawns, U64 empty, U64 enemyPieces) {
    std::vector<Move> moves;

    U64 able2DblPush = whitePawnsAble2DblPush(pawns, empty);
    U64 able2Push = whitePawnsAble2Push(pawns, empty);

    for(unsigned int i = 0; i < 8; i++) {
        if(able2DblPush & (1UL << (i + 8))) {
            Move move(i + 8, i + 24, FLAG_PAWN_DBL_PUSH);
            moves.emplace_back(move);
        }
    }

    for(unsigned int i = 0; i < 64; i++) {
        if(able2Push & (1UL << i)) {
            Move move(i, i + 8, FLAG_QUIET);
            moves.emplace_back(move);
        }
    }

    U64 able2Capture = whitePawnsAble2CaptureAny(pawns, enemyPieces);

    for(unsigned int i = 0; i < 64; i++) {
        if(able2Capture & (1UL << i)) {
            U64 west = whitePawnsWestAttacks(1UL << i);
            for(unsigned int k = 0; k < 64; k++) {
                if(west & (1UL << k) & enemyPieces) {
                    Move move(i, k, FLAG_CAPTURE);
                    moves.emplace_back(move);
                    break;
                }
            }

            U64 east = whitePawnsEastAttacks(1UL << i);
            for(unsigned int k = 0; k < 64; k++) {
                if(east & (1UL << k) & enemyPieces) {
                    Move move(i, k, FLAG_CAPTURE);
                    moves.emplace_back(move);
                    break;
                }
            }
        }
    }

    return moves;
}

std::vector<Move> Pawn::getMovesBlack(U64 pawns, U64 empty, U64 enemyPieces) {
    std::vector<Move> moves;

    U64 able2DblPush = blackPawnsAble2DblPush(pawns, empty);
    U64 able2Push= blackPawnsAble2Push(pawns, empty);

    for(unsigned int i = 0; i < 8; i++) {
        if (able2DblPush & (1UL << (i + 48))) {
            Move move(i + 48, i + 32, FLAG_PAWN_DBL_PUSH);
            moves.emplace_back(move);
        }
    }

    for(unsigned int i = 0; i < 64; i++) {
        if(able2Push & (1UL << i)) {
            Move move(i, i - 8, FLAG_QUIET);
            moves.emplace_back(move);
        }
    }

    U64 able2Capture = blackPawnsAble2CaptureAny(pawns, enemyPieces);

    for(unsigned int i = 0; i < 64; i++) {
       if(able2Capture & (1UL << i)) {
           U64 west = blackPawnsWestAttacks(1UL << i);
           for(unsigned int k = 0; k < 64; k++) {
               if(west & (1UL << k) & enemyPieces) {
                   Move move(i, k, FLAG_CAPTURE);
                   moves.emplace_back(move);
                   break;
               }
           }

           U64 east = blackPawnsEastAttacks(1UL << i);
           for(unsigned int k = 0; k < 64; k++) {
               if(east & (1UL << k) & enemyPieces) {
                   Move move(i, k, FLAG_CAPTURE);
                   moves.emplace_back(move);
                   break;
               }
           }
       }
    }

    return moves;
}

std::vector<Move> Pawn::getMoves(U64 pawns, U64 empty, U64 enemyPieces, int team) {
    if(team == WHITE) {
        return getMovesWhite(pawns, empty, enemyPieces);
    } else {
        return getMovesBlack(pawns, empty, enemyPieces);
    }
}