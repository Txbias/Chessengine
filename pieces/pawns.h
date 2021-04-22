#pragma once

#include "../utils.h"

namespace Pawn {
    typedef unsigned long U64;

    const int WHITE = 0;
    const int BLACK = 1;
    const U64 notAFile = 0xfefefefefefefefe; // ~0x0101010101010101
    const U64 notHFile = 0x7f7f7f7f7f7f7f7f; // ~0x8080808080808080

    U64 eastOne(U64 b) { return (b << 1) & notAFile; }

    U64 noEaOne(U64 b) { return (b << 9) & notAFile; }

    U64 soEaOne(U64 b) { return (b >> 7) & notAFile; }

    U64 westOne(U64 b) { return (b >> 1) & notHFile; }

    U64 soWeOne(U64 b) { return (b >> 9) & notHFile; }

    U64 noWeOne(U64 b) { return (b << 7) & notHFile; }

    U64 whiteSinglePushTargets(U64 whitePawns, U64 empty) {
        return nortOne(whitePawns) & empty;
    }

    U64 blackSinglePushTargets(U64 blackPawns, U64 empty) {
        return soutOne(blackPawns) & empty;
    }

    U64 whiteDblPushTargets(U64 whitePawns, U64 empty) {
        U64 rank4 = 0;
        setRow(rank4, 3);

        U64 singlePush = whiteSinglePushTargets(whitePawns, empty);
        return nortOne(singlePush) & empty & rank4;
    }

    U64 blackDoublePushTargets(U64 blackPawns, U64 empty) {
        U64 rank5 = 0;
        setRow(rank5, 4);
        U64 singlePush = blackSinglePushTargets(blackPawns, empty);
        return soutOne(singlePush) & empty & rank5;
    }

    U64 whitePawnsAble2Push(U64 whitePawns, U64 empty) {
        return soutOne(empty) & whitePawns;
    }

    U64 blackPawnsAble2Push(U64 blackPawns, U64 empty) {
        return nortOne(empty) & blackPawns;
    }

    U64 whitePawnsAble2DblPush(U64 whitePawns, U64 empty) {
        U64 rank4 = 0;
        setRow(rank4, 3);

        U64 emptyRank3 = soutOne(empty & rank4) & empty;
        return whitePawnsAble2Push(whitePawns, emptyRank3);
    }

    U64 blackPawnsAble2DblPush(U64 blackPawns, U64 empty) {
        U64 rank5 = 0;
        setRow(rank5, 4);
        U64 emptyRank6 = nortOne(empty & rank5) & empty;
        return blackPawnsAble2Push(blackPawns, emptyRank6);
    }

    U64 whitePawnsEastAttacks(U64 pawns) {
        return noEaOne(pawns);
    }

    U64 blackPawnsEastAttacks(U64 pawns) {
        return soEaOne(pawns);
    }

    U64 whitePawnsWestAttacks(U64 pawns) {
        return noWeOne(pawns);
    }

    U64 blackPawnsWestAttacks(U64 pawns) {
        return soWeOne(pawns);
    }

    U64 whitePawnAnyAttack(U64 pawns) {
        return whitePawnsEastAttacks(pawns) | whitePawnsWestAttacks(pawns);
    }

    U64 blackPawnsAnyAttack(U64 pawns) {
        return blackPawnsEastAttacks(pawns) | blackPawnsWestAttacks(pawns);
    }

    U64 whitePawnsAble2CaptureAny(U64 whitePawns, U64 blackPieces) {
        return whitePawns & blackPawnsAnyAttack(blackPieces);
    }

    U64 blackPawnsAble2CaptureAny(U64 blackPawns, U64 whitePieces) {
        return blackPawns & whitePawnAnyAttack(whitePieces);
    }

    std::vector<Move> getMoves(U64 pawns, U64 empty, U64 enemyPieces, int team) {
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
}