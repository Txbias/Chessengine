
#include "pawns.h"

typedef unsigned long U64;

std::array<int, 64> Pawn::pieceSquareTable() {

    constexpr const std::array<int, 64> table = {
            0,  0,  0,  0,  0,  0,  0,  0,
            50, 50, 50, 50, 50, 50, 50, 50,
            10, 10, 20, 30, 30, 20, 10, 10,
            5,  5, 10, 25, 25, 10,  5,  5,
            0,  0,  0, 20, 20,  0,  0,  0,
            5, -5,-10,  0,  0,-10, -5,  5,
            5, 10, 10,-20,-20, 10, 10,  5,
            0,  0,  0,  0,  0,  0,  0,  0
    };

    return table;
}

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

U64 Pawn::passedPawns(U64 pawns[2], int team) {
    return team == 0 ? wPassedPawns(pawns) : bPassedPawns(pawns);
}

U64 Pawn::wPassedPawns(U64 pawns[2]) {
    U64 allFrontSpans = bFrontSpans(pawns[1]);
    allFrontSpans |= eastOne(allFrontSpans)
                  |  westOne(allFrontSpans);
    return pawns[0] & ~allFrontSpans;
}

U64 Pawn::bPassedPawns(U64 pawns[2]) {
    U64 allFrontSpans = wFrontSpans(pawns[0]);
    allFrontSpans |= eastOne(allFrontSpans)
                  |  westOne(allFrontSpans);
    return pawns[1] & ~allFrontSpans;
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

U64 Pawn::getMoveTargets(U64 pawns, U64 empty, int team) {

    U64 moveTargets = 0UL;
    if(team == WHITE) {
        moveTargets |= whiteSinglePushTargets(pawns, empty);
        moveTargets |= whiteDblPushTargets(pawns, empty);
    } else {
        moveTargets |= blackSinglePushTargets(pawns, empty);
        moveTargets |= blackDoublePushTargets(pawns, empty);
    }

    return moveTargets;
}

U64 Pawn::whitePawnsAble2CaptureAny(U64 whitePawns, U64 blackPieces) {
    return whitePawns & blackPawnsAnyAttack(blackPieces);
}

U64 Pawn::blackPawnsAble2CaptureAny(U64 blackPawns, U64 whitePieces) {
    return blackPawns & whitePawnAnyAttack(whitePieces);
}

std::vector<Move> Pawn::getMovesWhite(U64 pawns, U64 empty, U64 enemyPieces, U64 enPassantTarget) {
    std::vector<Move> moves(30); // Max pawns moves possible
    int index = 0;

    U64 able2DblPush = whitePawnsAble2DblPush(pawns, empty);
    U64 able2Push = whitePawnsAble2Push(pawns, empty);

    if(able2DblPush) do {
        int square = bitScanForward(able2DblPush);
        Move move(square, square + 16, FLAG_PAWN_DBL_PUSH);
        moves[index++] = move;
    } while(able2DblPush &= able2DblPush - 1);

    if(able2Push) do {
        int square = bitScanForward(able2Push);
        if(square >= 48) {
            // Promotion
            Move move(square, square + 8, FLAG_QUEEN_PROMOTION);
            moves[index++] = move;
        } else {
            Move move(square, square + 8, FLAG_QUIET);
            moves[index++] = move;
        }
    } while(able2Push &= able2Push - 1);

    // Captures
    U64 able2Capture = whitePawnsAble2CaptureAny(pawns, enemyPieces);
    if(able2Capture) do {
        int square = bitScanForward(able2Capture);
        U64 bitboard = 1UL << square;
        U64 west = whitePawnsWestAttacks(bitboard);
        U64 east = whitePawnsEastAttacks(bitboard);

        if(west & enemyPieces) {
            int targetSquare = bitScanForward(west);

            Move move(square, targetSquare, FLAG_CAPTURE);
            if(targetSquare >= 56) {
                // Pawn promotion capture
                move.setFlags(FLAG_QUEEN_PROMOTION_CAPTURE);
            }
            moves[index++] = move;
        }

        if(east & enemyPieces) {
            int targetSquare = bitScanForward(east);

            Move move(square, targetSquare, FLAG_CAPTURE);
            if(targetSquare >= 56) {
                move.setFlags(FLAG_QUEEN_PROMOTION_CAPTURE);
            }
            moves[index++] = move;
        }
    } while(able2Capture &= able2Capture - 1);

    // En passant moves
    U64 enPassant = whitePawnAnyAttack(pawns) & enPassantTarget;
    if(enPassant) {
        unsigned int to = bitScanForward(enPassant);
        U64 fromBitboard = pawns & blackPawnsAnyAttack(enPassantTarget);

        do {
            unsigned int from = bitScanForward(fromBitboard);
            Move move(from, to, FLAG_EP_CAPTURE);
            moves[index++] = move;
        } while(fromBitboard &= fromBitboard - 1);
    }

    moves.resize(index);
    return moves;
}

std::vector<Move> Pawn::getMovesBlack(U64 pawns, U64 empty, U64 enemyPieces, U64 enPassantTarget) {
    std::vector<Move> moves(30); // Max pawns moves possible
    int index = 0;

    U64 able2DblPush = blackPawnsAble2DblPush(pawns, empty);
    U64 able2Push= blackPawnsAble2Push(pawns, empty);

    if(able2DblPush) do {
            int square = bitScanForward(able2DblPush);
            Move move(square, square - 16, FLAG_PAWN_DBL_PUSH);
            moves[index++] = move;
    } while(able2DblPush &= able2DblPush - 1);

    if(able2Push) do {
            int square = bitScanForward(able2Push);
            if(square <= 15) {
                // Promotion
                Move move(square, square - 8, FLAG_QUEEN_PROMOTION);
                moves[index++] = move;
            } else {
                Move move(square, square - 8, FLAG_QUIET);
                moves[index++] = move;
            }
    } while(able2Push &= able2Push - 1);

    // Captures
    U64 able2Capture = blackPawnsAble2CaptureAny(pawns, enemyPieces);
    if(able2Capture) do {
            int square = bitScanForward(able2Capture);
            U64 bitboard = 1UL << square;
            U64 west = blackPawnsWestAttacks(bitboard);
            U64 east = blackPawnsEastAttacks(bitboard);

            if(west & enemyPieces) {
                int targetSquare = bitScanForward(west);

                Move move(square, targetSquare, FLAG_CAPTURE);
                if(targetSquare <= 7) {
                    // Pawn promotion capture
                    move.setFlags(FLAG_QUEEN_PROMOTION_CAPTURE);
                }
                moves[index++] = move;
            }

            if(east & enemyPieces) {
                int targetSquare = bitScanForward(east);

                Move move(square, targetSquare, FLAG_CAPTURE);
                if(targetSquare <= 7) {
                    move.setFlags(FLAG_QUEEN_PROMOTION_CAPTURE);
                }
                moves[index++] = move;
            }
    } while(able2Capture &= able2Capture - 1);

    // En passant moves
    U64 enPassant = blackPawnsAnyAttack(pawns) & enPassantTarget;
    if(enPassant) {
        unsigned int to = bitScanForward(enPassant);
        U64 fromBitboard = pawns & whitePawnAnyAttack(enPassantTarget);

        do {
            unsigned int from = bitScanForward(fromBitboard);
            Move move(from, to, FLAG_EP_CAPTURE);
            moves[index++] = move;
        } while(fromBitboard &= fromBitboard - 1);
    }

    moves.resize(index);
    return moves;
}

std::vector<Move> Pawn::getMoves(U64 pawns, U64 empty, U64 enemyPieces,
                                 U64 enPassantTarget, int team) {
    if(team == WHITE) {
        return getMovesWhite(pawns, empty, enemyPieces, enPassantTarget);
    } else {
        return getMovesBlack(pawns, empty, enemyPieces, enPassantTarget);
    }
}