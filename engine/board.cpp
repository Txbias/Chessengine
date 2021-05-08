#include "board.h"

#define SEARCH_DEPTH 4

Board::Board() {
    initializePieces();

}

void Board::initializePieces() {

    pieces[BLACK] = 0UL;
    pieces[WHITE] = 0UL;

    kingMoved[WHITE] = false;
    kingMoved[BLACK] = false;

    for(int i = 0; i < 2; i++) {
        for(int k = 0; k < 2; k++) {
            rookMoved[i][k] = false;
        }
    }

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
    actingTeam = ENEMY(team);
    unsigned long *targetPieceEnemy = getTargetPieces(move.getTo(), ENEMY(team));

    if(targetPieceEnemy != nullptr) {
        //TODO: for all promotions
        if(move.isPromotion()) {
            move.setFlags(FLAG_QUEEN_PROMOTION_CAPTURE);
        } else {
            move.setFlags(FLAG_CAPTURE);
        }
    }

    if(move.isCapture()) {
        unsigned int pieceType = getPieceType(move.getTo(), ENEMY(team));
        if(pieceType == -1) {
            std::cerr << "Des ist net gut" << std::endl;
        }

        move.setCapturedPiece(pieceType);
    }

    unsigned long *targetPieces = getTargetPieces(move.getFrom(), team);

    if(targetPieces == nullptr) {
        std::cout << "nullptr" << std::endl;
        std::cout << "tried move: " << Move::toNotation(move) << std::endl;
        std::cout << "team: " << team << std::endl;
        std::cout << "flags: " << move.getFlags() << std::endl;
        printBoard();
    }

    if(targetPieces == kings) {
        if(move.getFrom() / 8 == 0 && move.getTo() / 8 == 0 && team == WHITE && move.getFrom() == 4) {
            if(move.getTo() == 6) {
                move.setFlags(FLAG_KING_CASTLE);
            } else if(move.getTo() == 2) {
                move.setFlags(FLAG_QUEEN_CASTLE);
            }
        }

        if(move.getFrom() / 8 == 7 && move.getTo() / 8 == 7 && team == BLACK && move.getFrom() == 60) {
            if(move.getTo() == 62) {
                move.setFlags(FLAG_KING_CASTLE);
            } else if(move.getTo() == 58) {
                move.setFlags(FLAG_QUEEN_CASTLE);
            }
        }
    }

    if(targetPieces == kings) {
        if(!kingMoved[team]) {
            kingMoved[team] = true;
            move.setInitialMoveKing();
        }
    } else if(targetPieces == rooks) {
        if(move.getFrom() == 0 && team == WHITE) {
            if(!rookMoved[WHITE][0]) {
                move.setInitialMoveRook();
                rookMoved[WHITE][0] = true;
            }
        } else if(move.getFrom() == 7 && team == WHITE) {
            if(!rookMoved[WHITE][1]) {
                move.setInitialMoveRook();
                rookMoved[WHITE][1] = true;
            }
        } else if(move.getFrom() == 56 && team == BLACK) {
            if(!rookMoved[BLACK][0]) {
                move.setInitialMoveRook();
                rookMoved[BLACK][0] = true;
            }
        } else if(move.getFrom() == 63 && team == BLACK) {
            if(!rookMoved[BLACK][1]) {
                move.setInitialMoveRook();
                rookMoved[BLACK][1] = true;
            }
        }
    }

    U64 originSquare = ~(1UL << move.getFrom());
    // Clear origin square
    targetPieces[team] &= originSquare;
    occupied &= originSquare;
    pieces[team] &= originSquare;

    // Set bit at new position
    U64 target = 1UL << move.getTo();

    if(move.isPromotion()) {
        // TODO: for all types of promotions
        queens[team] |= target;
    } else {
        targetPieces[team] |= target;
    }
    occupied |= target;
    pieces[team] |= target;

    if(move.isCastle()) {
        U64 originSquare;
        U64 targetSquare;
        if(move.getFlags() == FLAG_KING_CASTLE) {
            // Move rook
            if(team == WHITE) {
                originSquare = ~(1UL << 7);
            } else if(team == BLACK) {
                originSquare = ~(1UL << 63);
            }

            if(team == WHITE) {
                targetSquare = 1UL << 5;
            } else if(team == BLACK) {
                targetSquare = 1UL << 61;
            }

        } else if(move.getFlags() == FLAG_QUEEN_CASTLE) {
            if(team == WHITE) {
                originSquare = ~1UL;
            } else if(team == BLACK) {
                originSquare = ~(1UL << 56);
            }

            if(team == WHITE) {
                targetSquare = 1UL << 3;
            } else if(team == BLACK) {
                targetSquare = 1UL << 59;
            }
        }

        rooks[team] &= originSquare;
        occupied &= originSquare;
        pieces[team] &= originSquare;

        rooks[team] |= targetSquare;
        occupied |= targetSquare;
        pieces[team] |= targetSquare;
    }

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

    actingTeam = enemyTeam;

    unsigned long* movedPieces = getTargetPieces(move.getTo(), team);

    // Save new position
    U64 originSquare = 1UL << move.getFrom();
    if(move.isPromotion()) {
        pawns[team] |= originSquare;
    } else {
        movedPieces[team] |= originSquare;
    }
    occupied |= originSquare;
    pieces[team] |= originSquare;

    // Clear position
    U64 currentPos = ~(1UL << move.getTo());
    movedPieces[team] &= currentPos;
    occupied &= currentPos;
    pieces[team] &= currentPos;

    if(move.isInitialMoveKing()) {
        kingMoved[team] = false;
    } else if(move.isInitialMoveRook()) {
        if(move.getFrom() == 0 && team == WHITE) {
            rookMoved[WHITE][0] = false;
        } else if(move.getFrom() == 7 && team == WHITE) {
            rookMoved[WHITE][1] = false;
        } else if(move.getFrom() == 56 && team == BLACK) {
            rookMoved[BLACK][0] = false;
        } else if(move.getFrom() == 63 && team == BLACK) {
            rookMoved[BLACK][1] = false;
        } else {
            std::cerr << "Move is supposed to be initial Move for king or rook but it isn't" << std::endl;
        }
    }

    if(move.isCastle()) {
        U64 originSquare;
        U64 targetSquare;
        if(move.getFlags() == FLAG_KING_CASTLE) {
            // Move rook
            if(team == WHITE) {
                originSquare = ~(1UL << 5);
            } else if(team == BLACK) {
                originSquare = ~(1UL << 61);
            }

            if(team == WHITE) {
                targetSquare = 1UL << 7;
            } else if(team == BLACK) {
                targetSquare = 1UL << 63;
            }

        } else if(move.getFlags() == FLAG_QUEEN_CASTLE) {
            if(team == WHITE) {
                originSquare = ~(1UL << 3);
            } else if(team == BLACK) {
                originSquare = ~(1UL << 59);
            }

            if(team == WHITE) {
                targetSquare = 1UL;
            } else if(team == BLACK) {
                targetSquare = 1UL << 56;
            }
        }

        rooks[team] &= originSquare;
        occupied &= originSquare;
        pieces[team] &= originSquare;

        rooks[team] |= targetSquare;
        occupied |= targetSquare;
        pieces[team] |= targetSquare;
    }

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
    std::cout << getBoardPrintable() << std::endl;
}

std::string Board::getBoardPrintable() {
    std::string output;
    std::string row[8];
    for(int i = 63; i >= 0; i--) {
        if((i+1) % 8 == 0) {
            for(int k = 0; k < 8; k++) {
                output += row[k];
            }
            output += "\n";
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

        if(type == -1) {
            std::cerr << "Des ist auch net gut" << std::endl;
        }

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
    for(auto & k : row) {
        output += k;
    }
    return output;
}

unsigned long * Board::getTargetPieces(unsigned int targetSquare, int team) {
    unsigned int pieceType = getPieceType(targetSquare, team);

    unsigned long *targetPieces = nullptr;
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


    return -1;
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

U64 Board::getTargetMap(int team) {
    U64 targets = 0UL;
    int enemyTeam = ENEMY(team);
    targets |= Pawn::getAttackTargets(pawns[team], team);
    targets |= Rook::getTargets(rooks[team], pieces[team], pieces[enemyTeam]);
    targets |= Knight::getTargets(knights[team], pieces[team]);
    targets |= Bishop::getTargets(bishops[team], pieces[team], pieces[enemyTeam]);
    targets |= Queen::getTargets(queens[team], pieces[team], pieces[enemyTeam]);
    targets |= King::getTargets(kings[team], pieces[team]);

    return targets;
}

bool Board::inCheck(int team) {
    U64 enemyTargets = getTargetMap(ENEMY(team));
    return enemyTargets & kings[team];
}

bool Board::checkMate(int team) {
    int enemyTeam = ENEMY(team);
    U64 enemyTargets = getTargetMap(enemyTeam);

    if(!(enemyTargets & kings[team])) {
        // Not even check
        return false;
    }

    U64 kingTargets = King::getTargets(kings[team], pieces[team]);

    kingTargets &= ~enemyTargets;

    if(kingTargets) {
        // King has squares he can move to
        return false;
    }

    U64 ownTeamTargets = getTargetMap(team);
    std::vector<Move> enemyMoves = getAllMoves(enemyTeam);

    int attacksOnKing = 0;
    int canBeCaptured = 0;
    for(Move move : enemyMoves) {
        if(!(kings[team] & (1UL << move.getTo()))) {
            // Piece doesn't attack king
            continue;
        }

        if(ownTeamTargets & (1UL << move.getFrom())) {
            // Attacking piece can be captured
            canBeCaptured++;
        }

        attacksOnKing++;
    }

    if(attacksOnKing == 1 && canBeCaptured == 1) {
        // the only attacking piece can be captured
        return false;
    }

    // TODO: finish

    return true;
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

    std::vector<Move> castleMoves = King::getCastlingMoves(occupied, getTargetMap(enemyTeam),
                                                           team, kingMoved, rookMoved);
    moves.insert(moves.end(), std::begin(castleMoves), std::end(castleMoves));

    return moves;
}

Move Board::getBestMove(int team) {
    int actingTeamBefore = actingTeam;
    actingTeam = team;

    Move bestMove(0, 0, 0);
    alphaBeta(INT16_MIN / 2, INT16_MAX / 2, SEARCH_DEPTH, team, bestMove);

    actingTeam = actingTeamBefore;
    return bestMove;
}

int Board::alphaBeta(int alpha, int beta, int depthLeft, int team, Move &bestMove) {
    if(depthLeft == 0) {
        return valuePosition(actingTeam);
    }

    std::vector<Move> allMoves = getAllMoves(actingTeam);

    for(int i = 0; i < allMoves.size(); i++) {
        executeMove(allMoves[i]);

        if(depthLeft == SEARCH_DEPTH) {
            if(inCheck(ENEMY(actingTeam))) {
                undoLastMove();
                continue;
            }
        }

        int score = -alphaBeta(-beta, -alpha, depthLeft - 1, team, bestMove);
        undoLastMove();
        if(score >= beta) {
            return beta;
        }
        if(score > alpha) {
            alpha = score;
            if(actingTeam == ENEMY(team) && depthLeft == SEARCH_DEPTH) {
                bestMove = allMoves[i];
            }
        }
    }

    return alpha;
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
    value += team == WHITE ? (valueQueensWhite - valueQueensBlack) : (valueQueensBlack - valueQueensWhite);

    int whiteKing = getCardinality(kings[WHITE]) * VALUE_KING;
    int blackKing = getCardinality(kings[BLACK]) * VALUE_KING;
    value += team == WHITE ? (whiteKing - blackKing) : (blackKing - whiteKing);

    return value;
}