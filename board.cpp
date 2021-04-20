#include "board.h"


Board::Board() {
    initializePieces();

}

void Board::initializePieces() {
    unsigned long whitePawns = 0;
    setRow(whitePawns, 1);
    pawns[WHITE] = whitePawns;
    occupied |= whitePawns;
    white |= whitePawns;

    unsigned long blackPawns = 0;
    setRow(blackPawns, 6);
    pawns[BLACK] = blackPawns;
    occupied |= blackPawns;
    black |= blackPawns;

    unsigned long whiteRook = 0;
    setBit(whiteRook, 0);
    setBit(whiteRook, 7);
    rooks[WHITE] = whiteRook;
    occupied |= whiteRook;
    white |= whiteRook;

    unsigned long blackRook = 0;
    setBit(blackRook, 63);
    setBit(blackRook, 56);
    rooks[BLACK] = blackRook;
    occupied |= blackRook;
    black |= blackRook;

    unsigned long whiteKnight = 0;
    setBit(whiteKnight, 1);
    setBit(whiteKnight, 6);
    knights[WHITE] = whiteKnight;
    occupied |= whiteKnight;
    white |= whiteKnight;

    unsigned long blackKnight = 0;
    setBit(blackKnight, 57);
    setBit(blackKnight, 62);
    knights[BLACK] = blackKnight;
    occupied |= blackKnight;
    black |= blackKnight;

    unsigned long whiteBishop = 0;
    setBit(whiteBishop, 2);
    setBit(whiteBishop, 5);
    bishops[WHITE] = whiteBishop;
    occupied |= whiteBishop;
    white |= whiteBishop;

    unsigned long blackBishop = 0;
    setBit(blackBishop, 58);
    setBit(blackBishop, 61);
    bishops[BLACK] = blackBishop;
    occupied |= blackBishop;
    black |= blackBishop;

    unsigned long whiteQueen = 0;
    setBit(whiteQueen, 3);
    queens[WHITE] = whiteQueen;
    occupied |= whiteQueen;
    white |= whiteQueen;

    unsigned long blackQueen = 0;
    setBit(blackQueen, 59);
    queens[BLACK] = blackQueen;
    occupied |= blackQueen;
    black |= blackQueen;

    unsigned long whiteKing = 0;
    setBit(whiteKing, 4);
    queens[WHITE] = whiteKing;
    occupied |= whiteKing;
    white |= whiteKing;

    unsigned long blackKing = 0;
    setBit(blackKing, 60);
    queens[BLACK] = blackKing;
    occupied |= blackKing;
    black |= blackKing;
}


std::vector<Move> Board::getAllMoves() {

}
