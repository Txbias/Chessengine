#include "board.h"


Board::Board() {
    initializePieces();

}

void Board::initializePieces() {
    unsigned long whitePawns = 0;
    whitePawns = setRow(whitePawns, 1);
    pawns[WHITE] = whitePawns;
    occupied |= whitePawns;
    white |= whitePawns;

    unsigned long blackPawns = 0;
    blackPawns = setRow(blackPawns, 6);
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
    kings[WHITE] = whiteKing;
    occupied |= whiteKing;
    white |= whiteKing;

    unsigned long blackKing = 0;
    setBit(blackKing, 60);
    kings[BLACK] = blackKing;
    occupied |= blackKing;
    black |= blackKing;
}


std::vector<Move> Board::getAllMoves(int team) {
    std::vector<Move> moves;
    U64 empty = ~occupied;

    U64 enemy = team == WHITE ? black : white;
    U64 ownPieces = team == WHITE ? white : black;

    std::vector<Move> pawnMoves = Pawn::getMoves(pawns[team], empty, black, team);
    moves.insert(moves.end(), std::begin(pawnMoves), std::end(pawnMoves));

    std::vector<Move> knightMoves = Knight::getMoves(knights[team], enemy, ownPieces);
    moves.insert(moves.end(), std::begin(knightMoves), std::end(knightMoves));

    std::vector<Move> rookMoves = Rook::getMoves(rooks[team], ownPieces, enemy);
    moves.insert(moves.end(), std::begin(rookMoves), std::end(rookMoves));

    std::vector<Move> bishopMoves = Bishop::getMoves(bishops[team], ownPieces, enemy);
    moves.insert(moves.end(), std::begin(bishopMoves), std::end(bishopMoves));

    std::vector<Move> queenMoves = Queen::getMoves(queens[team], ownPieces, enemy);
    moves.insert(moves.end(), std::begin(queenMoves), std::end(queenMoves));

    std::vector<Move> kingMoves = King::getMoves(kings[team], ownPieces, enemy);
    moves.insert(moves.end(), std::begin(kingMoves), std::end(kingMoves));

    return moves;
}

int Board::valuePosition(int team) {
    int value = 0;

    int valuePawnsWhite = getCardinality(pawns[WHITE]) * VALUE_PAWN;
    int valuePawnsBlack = getCardinality(pawns[BLACK]) * VALUE_PAWN;
    value += team == WHITE ? (valuePawnsWhite - valuePawnsBlack) : (valuePawnsBlack - valuePawnsWhite);

    int valueKnightsWhite = getCardinality(knights[WHITE]) * VALUE_KNIGHT;
    int valueKnightsBlack = getCardinality(knights[BLACK]) * VALUE_KNIGHT;
    value += team == WHITE ? (valueKnightsWhite - valueKnightsBlack) : (valueKnightsBlack - valueKnightsWhite);

    int valueRooksWhite = getCardinality(rooks[WHITE]) * VALUE_ROOK;
    int valueRooksBlack = getCardinality(rooks[BLACK]) * VALUE_ROOK;
    value += team == WHITE ? (valueRooksWhite - valueRooksBlack) : (valueRooksBlack - valueRooksWhite);

    int valueBishopsWhite = getCardinality(bishops[WHITE]) * VALUE_BISHOP;
    int valueBishopsBlack = getCardinality(bishops[BLACK]) * VALUE_BISHOP;
    value += team == WHITE ? (valueBishopsWhite - valueBishopsBlack) : (valueBishopsBlack - valueBishopsWhite);

    int valueQueensWhite = getCardinality(queens[WHITE]) * VALUE_QUEEN;
    int valueQueensBlack = getCardinality(queens[BLACK]) * VALUE_QUEEN;
    value += team == WHITE ? (valueQueensWhite + valueQueensBlack) : (valueQueensBlack - valueQueensWhite);

    int whiteKing = getCardinality(kings[WHITE]) * VALUE_KING;
    int blackKing = getCardinality(kings[BLACK]) * VALUE_KING;
    value += team == WHITE ? (whiteKing - blackKing) : (blackKing - whiteKing);

    return value;
}