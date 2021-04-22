
#include "pawns.h"

typedef unsigned long U64;

U64 Pawn::noEaOne(U64 b) { return (b << 9) & notAFile; }

U64 Pawn::soEaOne(U64 b) { return (b >> 7) & notAFile; }

U64 Pawn::soWeOne(U64 b) { return (b >> 9) & notHFile; }

U64 Pawn::noWeOne(U64 b) { return (b << 7) & notHFile; }

U64 Pawn::whiteSinglePushTargets(U64 whitePawns, U64 empty) {
    return nortOne(whitePawns) & empty;
}

U64 Pawn::blackSinglePushTargets(U64 blackPawns, U64 empty) {
    return soutOne(blackPawns) & empty;
}

U64 Pawn::whiteDblPushTargets(U64 whitePawns, U64 empty) {
    U64 rank4 = 0;
    setRow(rank4, 3);

    U64 singlePush = whiteSinglePushTargets(whitePawns, empty);
    return nortOne(singlePush) & empty & rank4;
}

U64 Pawn::blackDoublePushTargets(U64 blackPawns, U64 empty) {
    U64 rank5 = 0;
    setRow(rank5, 4);
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
    setRow(rank4, 3);

    U64 emptyRank3 = soutOne(empty & rank4) & empty;
    return whitePawnsAble2Push(whitePawns, emptyRank3);
}

U64 Pawn::blackPawnsAble2DblPush(U64 blackPawns, U64 empty) {
    U64 rank5 = 0;
    setRow(rank5, 4);
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

U64 Pawn::whitePawnsAble2CaptureAny(U64 whitePawns, U64 blackPieces) {
    return whitePawns & blackPawnsAnyAttack(blackPieces);
}

U64 Pawn::blackPawnsAble2CaptureAny(U64 blackPawns, U64 whitePieces) {
    return blackPawns & whitePawnAnyAttack(whitePieces);
}

std::vector<Move> Pawn::getMoves(U64 pawns, U64 empty, U64 enemyPieces, int team) {
    std::vector<Move> moves;

    U64 able2DblPush;
    U64 able2Push;

    if(team == WHITE) {
        able2DblPush = whitePawnsAble2DblPush(pawns, empty);
        able2Push = whitePawnsAble2Push(pawns, empty);
    } else {
        able2DblPush = blackPawnsAble2DblPush(pawns, empty);
        able2Push = blackPawnsAble2Push(pawns, empty);
    }

    for (int i = 0; i < 8; i++) {
        if (able2DblPush & (1 << (i + 8))) {
            Move move(i + 8, i + 24, FLAG_PAWN_DBL_PUSH);
            moves.emplace_back(move);
        }
        if (able2Push & (1 << (i + 8))) {
            Move move(i + 8, i + 16, FLAG_QUIET);
            moves.emplace_back(move);
        }
    }

    U64 able2Capture;

    if(team == WHITE) {
        able2Capture = whitePawnsAble2CaptureAny(pawns, enemyPieces);
    } else {
        able2Capture = blackPawnsAble2CaptureAny(pawns, enemyPieces);
    }

    if(team == WHITE) {
        for (int i = 0; i < 64; i++) {
            if (able2Capture & (1 << i)) {
                Move move(i, whitePawnAnyAttack(1 << i), FLAG_CAPTURE);
                moves.emplace_back(move);
            }
        }
    } else {
        for(int i = 0; i < 64; i++) {
            if(able2Capture & (1 << i)) {
                Move move(i, blackPawnsAnyAttack(1 << i), FLAG_CAPTURE);
                moves.emplace_back(move);
            }
        }
    }

    return moves;
}