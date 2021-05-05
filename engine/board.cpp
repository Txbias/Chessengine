#include "board.h"


Board::Board() {
    initializePieces();

}

void Board::initializePieces() {

    pieces[BLACK] = 0UL;
    pieces[WHITE] = 0UL;

    unsigned long whitePawns = 0;
    whitePawns = setRow(whitePawns, 1);
    pawns[WHITE] = whitePawns;
    occupied |= whitePawns;
    pieces[WHITE] |= whitePawns;

    unsigned long blackPawns = 0;
    blackPawns = setRow(blackPawns, 6);
    pawns[BLACK] = blackPawns;
    occupied |= blackPawns;
    pieces[BLACK] |= blackPawns;

    unsigned long whiteRook = 0;
    setBit(whiteRook, 0);
    setBit(whiteRook, 7);
    rooks[WHITE] = whiteRook;
    occupied |= whiteRook;
    pieces[WHITE] |= whiteRook;

    unsigned long blackRook = 0;
    setBit(blackRook, 63);
    setBit(blackRook, 56);
    rooks[BLACK] = blackRook;
    occupied |= blackRook;
    pieces[BLACK] |= blackRook;

    unsigned long whiteKnight = 0;
    setBit(whiteKnight, 1);
    setBit(whiteKnight, 6);
    knights[WHITE] = whiteKnight;
    occupied |= whiteKnight;
    pieces[WHITE] |= whiteKnight;

    unsigned long blackKnight = 0;
    setBit(blackKnight, 57);
    setBit(blackKnight, 62);
    knights[BLACK] = blackKnight;
    occupied |= blackKnight;
    pieces[BLACK] |= blackKnight;

    unsigned long whiteBishop = 0;
    setBit(whiteBishop, 2);
    setBit(whiteBishop, 5);
    bishops[WHITE] = whiteBishop;
    occupied |= whiteBishop;
    pieces[WHITE] |= whiteBishop;

    unsigned long blackBishop = 0;
    setBit(blackBishop, 58);
    setBit(blackBishop, 61);
    bishops[BLACK] = blackBishop;
    occupied |= blackBishop;
    pieces[BLACK] |= blackBishop;

    unsigned long whiteQueen = 0;
    setBit(whiteQueen, 3);
    queens[WHITE] = whiteQueen;
    occupied |= whiteQueen;
    pieces[WHITE] |= whiteQueen;

    unsigned long blackQueen = 0;
    setBit(blackQueen, 59);
    queens[BLACK] = blackQueen;
    occupied |= blackQueen;
    pieces[BLACK] |= blackQueen;

    unsigned long whiteKing = 0;
    setBit(whiteKing, 4);
    kings[WHITE] = whiteKing;
    occupied |= whiteKing;
    pieces[WHITE] |= whiteKing;

    unsigned long blackKing = 0;
    setBit(blackKing, 60);
    kings[BLACK] = blackKing;
    occupied |= blackKing;
    pieces[BLACK] |= blackKing;
}

void Board::executeMove(Move move) {

    int team = getTeam(move.getFrom());
    if(move.isCapture()) {
        move.setCapturedPiece(getPieceType(move.getTo(), ENEMY(team)));
    }

    unsigned long *targetPieces = getTargetPieces(move.getFrom(), team);

    U64 originSquare = ~(1UL << move.getFrom());
    // Clear origin square
    targetPieces[team] &= originSquare;
    occupied &= originSquare;
    pieces[team] &= originSquare;

    // Set bit at new position
    U64 target = 1UL << move.getTo();
    targetPieces[team] |= target;
    occupied |= target;
    pieces[team] |= target;

    if(move.isCapture()) {
        unsigned long* targetPieces = getTargetPieces(move.getTo(), ENEMY(team));
        U64 targetSquare = ~(1UL << move.getTo());
        targetPieces[ENEMY(team)] &= targetSquare;
        pieces[ENEMY(team)] &= targetSquare;
    }

    moves.push(move);
}

void Board::undoLastMove() {
    if(moves.empty()) {
        std::cerr << "No move left!" << std::endl;
        return;
    }

    Move move = moves.top();
    moves.pop();

    int team = getTeam(move.getTo());
    int enemyTeam = ENEMY(team);

    unsigned long* movedPieces = getTargetPieces(move.getTo(), team);

    // Save new position
    U64 originSquare = 1UL << move.getFrom();
    movedPieces[team] |= originSquare;
    occupied |= originSquare;
    pieces[team] |= originSquare;

    // Clear position
    U64 currentPos = ~(1UL << move.getTo());
    movedPieces[team] &= currentPos;
    occupied &= currentPos;
    pieces[team] &= currentPos;

    if(move.isCapture()) {
        unsigned int capturedPiece = move.getCapturedPiece();
        unsigned long* capturedPieces;

        switch(capturedPiece) {
            case PAWN:
                capturedPieces = pawns;
                break;
            case ROOK:
                capturedPieces = rooks;
                break;
            case KNIGHT:
                capturedPieces = knights;
                break;
            case BISHOP:
                capturedPieces = bishops;
                break;
            case QUEEN:
                capturedPieces = queens;
                break;
            case KING:
                capturedPieces = kings;
                break;
        }

        capturedPieces[enemyTeam] |= ~currentPos;
        pieces[enemyTeam] |= ~currentPos;
        occupied |= ~currentPos;
    }
}

void Board::printBoard() {
    std::string row[8];
    for(int i = 63; i >= 0; i--) {
        if((i+1) % 8 == 0) {
            for(int k = 0; k < 8; k++) {
                std::cout << row[k];
            }
            std::cout << std::endl;
        }
        if(!(occupied & (1UL << i))) {
            row[i % 8] = " ";
            continue;
        }

        int team;
        if(pieces[WHITE] & (1UL << i)) {
            team = WHITE;
        } else {
            team = BLACK;
        }

        unsigned int type = getPieceType(i, team);
        std::string code;
        switch(type) {
            case PAWN:
                code = team == WHITE ? "\u2659" : "\u265F";
                break;
            case ROOK:
                code = team == WHITE ? "\u2656" : "\u265C";
                break;
            case KNIGHT:
                code = team == WHITE ? "\u2658" : "\u265E";
                break;
            case BISHOP:
                code = team == WHITE ? "\u2657": "\u265D";
                break;
            case QUEEN:
                code = team == WHITE ? "\u2655" : "\u265B";
                break;
            case KING:
                code = team == WHITE ? "\u2654" : "\u265A";
                break;
        }
        row[i % 8] = code;
    }
    for(int k = 0; k < 8; k++) {
        std::cout << row[k];
    }
    std::cout << std::endl;
}

unsigned long * Board::getTargetPieces(unsigned int targetSquare, int team) {
    unsigned int pieceType = getPieceType(targetSquare, team);

    unsigned long *targetPieces;
    switch(pieceType) {
        case PAWN:
            targetPieces = pawns;
            break;
        case ROOK:
            targetPieces = rooks;
            break;
        case KNIGHT:
            targetPieces = knights;
            break;
        case BISHOP:
            targetPieces = bishops;
            break;
        case QUEEN:
            targetPieces = queens;
            break;
        case KING:
            targetPieces = kings;
            break;
    }

    if(targetPieces == nullptr) {
        std::cerr << "Piece type couldn't be detected" << std::endl;
    }

    return targetPieces;
}

unsigned int Board::getPieceType(unsigned int targetSquare, int team) {
    U64 target = 1UL << targetSquare;

    if(pawns[team] & target) return PAWN;
    if(rooks[team] & target) return ROOK;
    if(knights[team] & target) return KNIGHT;
    if(bishops[team] & target) return BISHOP;
    if(queens[team] & target) return QUEEN;
    if(kings[team] & target) return KING;

    std::cerr << "Couldn't find piece type of square " <<
        targetSquare << " for team " << team << std::endl;

    return KING;
}

int Board::getTeam(unsigned int square) {
    U64 target = 1UL << square;
    if(target & pieces[WHITE]) {
        return WHITE;
    } else if(target & pieces[BLACK]) {
        return BLACK;
    } else {
        std::cerr << "Tried team lookup on empty square!" << std::endl;
        return WHITE;
    }
}

bool Board::inCheck(int team) {
    //TODO: use attack map

    int enemyTeam = ENEMY(team);
    U64 enemyTargets = 0UL;
    enemyTargets |= Pawn::getAttackTargets(pawns[enemyTeam], enemyTeam);
    enemyTargets |= Rook::getTargets(rooks[enemyTeam], pieces[enemyTeam], pieces[team]);
    enemyTargets |= Knight::getTargets(knights[enemyTeam], pieces[enemyTeam]);
    enemyTargets |= Bishop::getTargets(bishops[enemyTeam], pieces[enemyTeam], pieces[team]);
    enemyTargets |= Queen::getTargets(queens[enemyTeam], pieces[enemyTeam], pieces[team]);

    return enemyTargets & kings[team];

    /*std::vector<Move> moves = getAllMoves(ENEMY(team));

    unsigned int kingPosition;
    for(unsigned int i = 0; i < 64; i++) {
        if(kings[team] & (1UL << i)) {
            kingPosition = i;
        }
    }

    for(Move move : moves) {
        if(!move.isCapture()) {
            continue;
        }

        if(move.getTo() == kingPosition) {
            return true;
        }
    }
    return false;*/
}

std::vector<Move> Board::getAllMoves(int team) {
    std::vector<Move> moves;
    U64 empty = ~occupied;

    int enemyTeam = team == WHITE ? BLACK : WHITE;
    U64 enemyPieces = pieces[enemyTeam];
    U64 ownPieces = pieces[team];

    std::vector<Move> pawnMoves = Pawn::getMoves(pawns[team], empty, enemyPieces, team);
    moves.insert(moves.end(), std::begin(pawnMoves), std::end(pawnMoves));

    std::vector<Move> knightMoves = Knight::getMoves(knights[team], enemyPieces, ownPieces);
    moves.insert(moves.end(), std::begin(knightMoves), std::end(knightMoves));

    std::vector<Move> rookMoves = Rook::getMoves(rooks[team], ownPieces, enemyPieces);
    moves.insert(moves.end(), std::begin(rookMoves), std::end(rookMoves));

    std::vector<Move> bishopMoves = Bishop::getMoves(bishops[team], ownPieces, enemyPieces);
    moves.insert(moves.end(), std::begin(bishopMoves), std::end(bishopMoves));

    std::vector<Move> queenMoves = Queen::getMoves(queens[team], ownPieces, enemyPieces);
    moves.insert(moves.end(), std::begin(queenMoves), std::end(queenMoves));

    std::vector<Move> kingMoves = King::getMoves(kings[team], ownPieces, enemyPieces);
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