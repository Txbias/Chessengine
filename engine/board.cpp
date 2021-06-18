#include "board.h"

#define MIN(x, y) (x < y ? x : y)
#define MAX(x, y) (x > y ? x : y)

Board::Board() {

    pieces[BLACK] = 0UL;
    pieces[WHITE] = 0UL;

    kingMoved[WHITE] = false;
    kingMoved[BLACK] = false;

    for(int i = 0; i < 2; i++) {
        for(int k = 0; k < 2; k++) {
            rookMoved[i][k] = false;
        }
    }

    initializePieces();

    Position position = getCurrentPosition();
    positions.insert(std::make_pair(position, 1));
}

Board::Board(const std::string& fen) {

    occupied = 0UL;
    kingMoved[0] = true;
    kingMoved[1] = true;

    for(int i = 0; i < 2; i++) {
        for(int k = 0; k < 2; k++) {
            rookMoved[i][k] = true;
        }
    }


    for(int i = 0; i <= 1; i++) {
        pieces[i] = 0UL;
        pawns[i] = 0UL;
        knights[i] = 0UL;
        rooks[i] = 0UL;
        bishops[i] = 0UL;
        queens[i] = 0UL;
        kings[i] = 0UL;
    }

    int fenPosition = 0;
    int currentSquare = 56;

    for(int countSlash = 0; countSlash <= 7; countSlash++) {

        while(fen.at(fenPosition) != '/' && fen.at(fenPosition) != ' ') {

            if(isdigit(fen.at(fenPosition))) {
                int number = fen.at(fenPosition) - 48;
                currentSquare += number;
            } else {
                int team = BLACK; // black piece
                if(isupper(fen.at(fenPosition))) {
                    // White piece
                    team = WHITE;
                }

                char piece = tolower(fen.at(fenPosition));
                U64* targetPieces;

                switch(piece) {
                    case 'p':
                        // pawn
                        targetPieces = pawns;
                        break;
                    case 'r':
                        // rook
                        targetPieces = rooks;
                        break;
                    case 'n':
                        // knight
                        targetPieces = knights;
                        break;
                    case 'b':
                        // bishop
                        targetPieces = bishops;
                        break;
                    case 'q':
                        // queen
                        targetPieces = queens;
                        break;
                    case 'k':
                        // king
                        targetPieces = kings;
                        break;
                }

                targetPieces[team] |= (1UL << currentSquare);
                occupied |= (1UL << currentSquare);
                pieces[team] |= (1UL << currentSquare);
                currentSquare++;
            }

            fenPosition++;
        }
        fenPosition++;

        currentSquare = (((currentSquare - 1) / 8) - 1) * 8; // Set Square to next row start
    }

    if(fen.at(fenPosition) == 'w') {
        actingTeam = WHITE;
    } else if(fen.at(fenPosition) == 'b') {
        actingTeam = BLACK;
    } else {
        std::cerr << "Error" << std::endl;
    }

    fenPosition += 2;

    while(fen.at(fenPosition) != '-' && fen.at(fenPosition) != ' ') {
        int team = BLACK;
        if(isupper(fen.at(fenPosition))) {
            team = WHITE;
        }
        kingMoved[team] = false;

        if(tolower(fen.at(fenPosition)) == 'k') {
            rookMoved[team][0] = false;
        } else if(tolower(fen.at(fenPosition)) == 'q') {
            rookMoved[team][1] = false;
        }

        fenPosition++;
    }

    fenPosition++;

    if(fen.at(fenPosition) != '-') {
        // En passant target
        int file = fen.at(fenPosition) - 97;
        int row = (fen.at(fenPosition + 1) - 48) - 1;

        enPassantSquare = row * 8 + file;
        enPassantTarget = 1UL << enPassantSquare;
        fenPosition++;
    }

    fenPosition += 2;
    //TODO: implement half move clock

    fenPosition++;
    if(fen.at(fenPosition) != ' ') {
        fenPosition++;
    }

    amountFullMoves = std::stoi(fen.substr(fenPosition));

    Position position = getCurrentPosition();
    positions.insert(std::make_pair(position, 1));
}

void Board::initializePieces() {
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

/**
 * Requires no flags to be set for the given move the be executed. Is used
 * for moves of the enemy.
 * @param move The move to execute
 */
void Board::executeUserMove(Move move) {
    int team = getTeam(move.getFrom());

    if(move.getTo() == enPassantSquare && getPieceType(move.getFrom(), team) == PAWN) {
        move.setFlags(FLAG_EP_CAPTURE);
    }

    unsigned long *targetPieceEnemy = getTargetPieces(move.getTo(), ENEMY(team));

    if(targetPieceEnemy != nullptr) {
        //TODO: for all promotions
        if(move.isPromotion()) {
            move.setFlags(FLAG_QUEEN_PROMOTION_CAPTURE);
        } else {
            move.setFlags(FLAG_CAPTURE);
        }
    }

    unsigned long *targetPieces = getTargetPieces(move.getFrom(), team);

    // Check for castling
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
    } else if(targetPieces == pawns) {
        if(team == WHITE) {
            if(move.getFrom() / 8 == 1 && move.getTo() / 8 == 3) {
                move.setFlags(FLAG_PAWN_DBL_PUSH);
            }
        } else {
            if(move.getFrom() / 8 == 6 && move.getTo() / 8 == 4) {
                move.setFlags(FLAG_PAWN_DBL_PUSH);
            }
        }
    }

    executeMove(move);
}

void Board::executeMove(Move move) {

    int team = getTeam(move.getFrom());
    move.setTeam(team);

    U64 currentEPTarget = enPassantTarget;
    unsigned int currentEPSquare = enPassantSquare;

    move.setEPSquareBefore(enPassantSquare);

    enPassantTarget = 0UL;
    enPassantSquare = 0;

    if(move.getFlags() == FLAG_PAWN_DBL_PUSH) {
        if(team == WHITE) {
            enPassantTarget = southOne(1UL << move.getTo());
            enPassantSquare = move.getTo() - 8;
        } else {
            enPassantTarget = northOne(1UL << move.getTo());
            enPassantSquare = move.getTo() + 8;
        }
    }

    if(move.isCapture()) {
        unsigned long* pieceType;
        if(move.getFlags() == FLAG_EP_CAPTURE) {
            pieceType = pawns;
        } else {
            pieceType = getTargetPieces(move.getTo(), ENEMY(team));
        }
        if(pieceType == nullptr) {
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
            if(!rookMoved[BLACK][1]) {
                move.setInitialMoveRook();
                rookMoved[BLACK][1] = true;
            }
        } else if(move.getFrom() == 63 && team == BLACK) {
            if(!rookMoved[BLACK][0]) {
                move.setInitialMoveRook();
                rookMoved[BLACK][0] = true;
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
        U64 targetSquare;
        if(move.getFlags() == FLAG_KING_CASTLE) {
            // Move rook
            if(team == WHITE) {
                originSquare = ~(1UL << 7);
            } else {
                originSquare = ~(1UL << 63);
            }

            if(team == WHITE) {
                targetSquare = 1UL << 5;
            } else {
                targetSquare = 1UL << 61;
            }

        } else if(move.getFlags() == FLAG_QUEEN_CASTLE) {
            if(team == WHITE) {
                originSquare = ~1UL;
            } else {
                originSquare = ~(1UL << 56);
            }

            if(team == WHITE) {
                targetSquare = 1UL << 3;
            } else {
                targetSquare = 1UL << 59;
            }
        } else {
            std::cout << "error execute" << std::endl;
        }

        rooks[team] &= originSquare;
        occupied &= originSquare;
        pieces[team] &= originSquare;

        rooks[team] |= targetSquare;
        occupied |= targetSquare;
        pieces[team] |= targetSquare;
    }

    if(move.isCapture()) {
        U64 targetSquare;
        if(move.getFlags() == FLAG_EP_CAPTURE) {
            targetPieces = pawns;
            if(team == WHITE) {
                targetSquare = ~(1UL << (currentEPSquare - 8));
            } else {
                targetSquare = ~(1UL << (currentEPSquare + 8));
            }
            occupied &= targetSquare;
        } else {
            targetPieces = getTargetPieces(move.getTo(), ENEMY(team));
            targetSquare = ~(1UL << move.getTo());
        }
        targetPieces[ENEMY(team)] &= targetSquare;
        pieces[ENEMY(team)] &= targetSquare;
    }

    moves.push(move);

    // Save current position
    Position position = getCurrentPosition();

    auto it = positions.find(position);
    if(it == positions.end()) {
        // New position
        positions.insert(std::make_pair(position, 1));
    } else {
        it->second++;
        if(it->second == 3) {
            threeFoldRepetition = true;
        }
    }

    actingTeam = ENEMY(team);
    if(team == BLACK) {
        amountFullMoves++;
    }
}

void Board::undoLastMove() {
    if(moves.empty()) {
        std::cerr << "No move left!" << std::endl;
        return;
    }

    Position positionBeforeUnDone = getCurrentPosition();
    auto it = positions.find(positionBeforeUnDone);

    if(it == positions.end()) {
        std::cout << "Unknown position" << std::endl;
    }

    if(it->second == 1) {
        positions.erase(it);
    } else {
        if(it->second == 3) {
            threeFoldRepetition = false;
        }
        it->second--;
    }

    Move move = moves.top();
    moves.pop();

    int team = getTeam(move.getTo());
    int enemyTeam = ENEMY(team);

    actingTeam = ENEMY(actingTeam);

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
            rookMoved[WHITE][1] = false;
        } else if(move.getFrom() == 7 && team == WHITE) {
            rookMoved[WHITE][0] = false;
        } else if(move.getFrom() == 56 && team == BLACK) {
            rookMoved[BLACK][1] = false;
        } else if(move.getFrom() == 63 && team == BLACK) {
            rookMoved[BLACK][0] = false;
        } else {
            std::cerr << "Move is supposed to be initial Move for king or rook but it isn't" << std::endl;
        }
    }

    if(move.isCastle()) {
        U64 targetSquare;
        if(move.getFlags() == FLAG_KING_CASTLE) {
            // Move rook
            if(team == WHITE) {
                originSquare = ~(1UL << 5);
            } else {
                originSquare = ~(1UL << 61);
            }

            if(team == WHITE) {
                targetSquare = 1UL << 7;
            } else {
                targetSquare = 1UL << 63;
            }

        } else if(move.getFlags() == FLAG_QUEEN_CASTLE) {
            if(team == WHITE) {
                originSquare = ~(1UL << 3);
            } else {
                originSquare = ~(1UL << 59);
            }

            if(team == WHITE) {
                targetSquare = 1UL;
            } else {
                targetSquare = 1UL << 56;
            }
        } else {
            std::cout << "Error undo" << std::endl;
        }

        rooks[team] &= originSquare;
        occupied &= originSquare;
        pieces[team] &= originSquare;

        rooks[team] |= targetSquare;
        occupied |= targetSquare;
        pieces[team] |= targetSquare;
    }

    if(move.isCapture()) {
        unsigned long* capturedPieces = move.getCapturedPiece();

        currentPos = ~currentPos;
        if(move.getFlags() == FLAG_EP_CAPTURE) {
            if(team == WHITE) {
                currentPos = southOne(currentPos);
            } else {
                currentPos = northOne(currentPos);
            }
        }

        if(move.getFlags() == FLAG_EP_CAPTURE) {
            capturedPieces = pawns;
        }

        capturedPieces[enemyTeam] |= currentPos;
        pieces[enemyTeam] |= currentPos;
        occupied |= currentPos;
    }

    enPassantSquare = move.getEPSquareBefore();
    enPassantTarget = 1UL << move.getEPSquareBefore();

    if(team == BLACK) {
        amountFullMoves--;
    }
}

Position Board::getCurrentPosition() {

    U64 currPiecesPosition[2][6];
    bool canCastle[2][2];

    for(int i = 0; i < 2; i++) {
        currPiecesPosition[i][0] = pawns[i];
        currPiecesPosition[i][1] = knights[i];
        currPiecesPosition[i][2] = bishops[i];
        currPiecesPosition[i][3] = rooks[i];
        currPiecesPosition[i][4] = queens[i];
        currPiecesPosition[i][5] = kings[i];

        if(kingMoved[i]) {
            canCastle[i][0] = false;
            canCastle[i][1] = false;
        } else {
            canCastle[i][0] = !rookMoved[i][0];
            canCastle[i][1] = !rookMoved[i][1];
        }
    }

    return Position(actingTeam, enPassantSquare, canCastle, currPiecesPosition);
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

std::string Board::getFENString() {

    std::string fenString;

    for(int i = 7; i >= 0; i--) {
        int countEmpty = 0;
        for(int k = 0; k < 8; k++) {
            int positionIndex = i * 8 + k;
            U64 position = 1UL << positionIndex;

            if(!(occupied & position)) {
                countEmpty++;
                continue;
            } else {
                if(countEmpty != 0) {
                    fenString += std::to_string(countEmpty);
                    countEmpty = 0;
                }
            }

            for(int p = 0; p <= 1; p++) {
                if(pawns[p] & position) {
                    fenString += (char) ('P' + (p * 32));
                    break;
                } else if(rooks[p] & position) {
                    fenString += (char) ('R' + (p * 32));
                    break;
                } else if(knights[p] & position) {
                    fenString += (char) ('N' + (p * 32));
                    break;
                } else if(bishops[p] & position) {
                    fenString += (char) ('B' + (p * 32));
                    break;
                } else if(queens[p] & position) {
                    fenString += (char) ('Q' + (p * 32));
                    break;
                } else if(kings[p] & position) {
                    fenString += (char) ('K' + (p * 32));
                    break;
                }
            }

        }
        if(countEmpty != 0) {
            fenString += std::to_string(countEmpty);
        }
        if(i != 0) {
            fenString += '/';
        }
    }

    fenString += ' ';
    fenString += actingTeam == 0 ? "w " : "b ";

    unsigned long length = fenString.length();

    for(int i = 0; i < 2; i++) {
        if(kingMoved[i]) {
            continue;
        }

        if(!rookMoved[i][0]) {
            fenString += (char) ('K' + (i * 32));
        }
        if(!rookMoved[i][1]) {
            fenString += (char) ('Q' + (i * 32));
        }
    }

    fenString += ' ';

    if(length == fenString.length()) {
        // No side can castle
        fenString += "- ";
    }

    if(!moves.empty() || enPassantSquare != 0) {
        fenString += Move::toNotation(enPassantSquare);
    } else {
        fenString += '-';
    }
    fenString += " ";

    //TODO: implement half move clock
    fenString += "0 ";

    fenString += std::to_string(amountFullMoves);

    return fenString;
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

U64 Board::getTargetMap(int team, bool includeKing, bool countBlocked) {
    U64 targets = 0UL;
    int enemyTeam = ENEMY(team);
    targets |= (Pawn::getAttackTargets(pawns[team], team) & pieces[enemyTeam]);
    targets |= Rook::getTargets(rooks[team], pieces[team],
                                pieces[enemyTeam], countBlocked);
    targets |= Knight::getTargets(knights[team], pieces[team]);
    targets |= Bishop::getTargets(bishops[team], pieces[team],
                                  pieces[enemyTeam], countBlocked);
    targets |= Queen::getTargets(queens[team], pieces[team],
                                 pieces[enemyTeam], countBlocked);
    if(includeKing) {
        targets |= King::getTargets(kings[team], pieces[team]);
    }

    return targets;
}

bool Board::inCheck(int team) {
    U64 enemyTargets = getTargetMap(ENEMY(team));
    return enemyTargets & kings[team];
}

bool Board::checkMate(int team) {
    int enemyTeam = ENEMY(team);
    U64 enemyTargets = getTargetMap(enemyTeam, true);

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
    unsigned int pieceType = -1;
    Move *attackingMove;
    int attackingMoveIndex = -1;
    for(int i = 0; i < enemyMoves.size(); i++) {
        if(!(kings[team] & (1UL << enemyMoves[i].getTo()))) {
            // Piece doesn't attack king
            continue;
        }

        if(ownTeamTargets & (1UL << enemyMoves[i].getFrom())) {
            // Attacking piece can be captured
            canBeCaptured++;
        }

        pieceType = getPieceType(enemyMoves[i].getFrom(), enemyTeam);
        attackingMoveIndex = i;

        attacksOnKing++;
    }

    if(attacksOnKing == 1 && canBeCaptured == 1) {
        // the only attacking piece can be captured
        return false;
    }

    if(attacksOnKing == 2) {
        return true;
    }

    // Check if check can be blocked by own piece
    if(pieceType == KNIGHT || pieceType == PAWN) {
        return true;
    }

    unsigned int from = enemyMoves[attackingMoveIndex].getFrom();
    unsigned int to = enemyMoves[attackingMoveIndex].getTo();

    U64 inBetweenSquares = 0UL;
    int min = MIN(from, to);
    int max = MAX(from, to);

    if(from / 8 == to / 8) {
        for(int i = min + 1; i < max; i++) {
            inBetweenSquares |= (1UL << i);
        }
    } else if(from % 8 == to % 8) {
        for(int i = min; i < max; i+= 8) {
            inBetweenSquares |= (1UL << i);
        }
    } else {
        U64 fromBitboard = 1UL << from;
        U64 toBitboard = 1UL << to;
        if(from > to) {
            // north -> south
            if(from % 8 > to % 8) {
                //north east -> south west
                while((fromBitboard = (southWest(fromBitboard) & ~toBitboard))) {
                    inBetweenSquares |= fromBitboard;
                }
            } else {
                // north west -> south east
                while((fromBitboard = (southEast(fromBitboard) & ~toBitboard))) {
                    inBetweenSquares |= fromBitboard;
                }
            }
        } else {
            // south -> north
            if(from % 8 > to % 8) {
                // south east -> north west
                while((fromBitboard = (northWest(fromBitboard) & ~toBitboard))) {
                    inBetweenSquares |= fromBitboard;
                }
            } else {
                // south west -> north east
                while((fromBitboard = (northEast(fromBitboard) & ~toBitboard))) {
                    inBetweenSquares |= fromBitboard;
                }
            }
        }
    }

    U64 attackMapWithOutKing = getTargetMap(team, false);
    attackMapWithOutKing |= Pawn::getMoveTargets(pawns[team], ~occupied, team);


    if(inBetweenSquares & attackMapWithOutKing) {
        return false;
    }

    return true;
}

bool Board::isStaleMate(int team) {

    U64 targetMap = getTargetMap(team, false);
    targetMap |= Pawn::getMoveTargets(pawns[team], ~occupied, team);

    if(targetMap) {
        return false;
    }

    if(inCheck(team) || checkMate(team)) {
        return false;
    }

    U64 kingTargetMap = King::getTargets(kings[team], pieces[team]);

    if(!kingTargetMap) {
        return true;
    }


    U64 enemyTargetMap = getTargetMap(ENEMY(team), true, true);

    enemyTargetMap |= Pawn::getAttackTargets(pawns[ENEMY(team)], ENEMY(team));

    return !(kingTargetMap & ~enemyTargetMap);
}

std::vector<Move> Board::getAllMoves(int team) {
    std::vector<Move> allMoves;
    U64 empty = ~occupied;

    int enemyTeam = team == WHITE ? BLACK : WHITE;
    U64 enemyPieces = pieces[enemyTeam];
    U64 ownPieces = pieces[team];

    U64 localEPTarget = enPassantTarget;
    if(actingTeam != team) {
        localEPTarget = 0UL;
    }

    std::vector<Move> pawnMoves = Pawn::getMoves(pawns[team], empty, enemyPieces,
                                                 localEPTarget, team);
    allMoves.insert(allMoves.end(), std::begin(pawnMoves), std::end(pawnMoves));

    std::vector<Move> knightMoves = Knight::getMoves(knights[team], enemyPieces, ownPieces);
    allMoves.insert(allMoves.end(), std::begin(knightMoves), std::end(knightMoves));

    std::vector<Move> rookMoves = Rook::getMoves(rooks[team], ownPieces, enemyPieces);
    allMoves.insert(allMoves.end(), std::begin(rookMoves), std::end(rookMoves));

    std::vector<Move> bishopMoves = Bishop::getMoves(bishops[team], ownPieces, enemyPieces);
    allMoves.insert(allMoves.end(), std::begin(bishopMoves), std::end(bishopMoves));

    std::vector<Move> queenMoves = Queen::getMoves(queens[team], ownPieces, enemyPieces);
    allMoves.insert(allMoves.end(), std::begin(queenMoves), std::end(queenMoves));

    std::vector<Move> kingMoves = King::getMoves(kings[team], ownPieces, enemyPieces);
    allMoves.insert(allMoves.end(), std::begin(kingMoves), std::end(kingMoves));

    std::vector<Move> castleMoves = King::getCastlingMoves(kings[team], rooks[team], occupied,
                                                           getTargetMap(enemyTeam),
                                                           team, kingMoved, rookMoved);
    allMoves.insert(allMoves.end(), std::begin(castleMoves), std::end(castleMoves));

    return allMoves;
}

int Board::valueMove(const std::string &fen, const Move &move, int team) {
    Board board(fen);
    board.executeMove(move);

    if(board.inCheck(team)) {
        return 30000;
    }

    if(board.isStaleMate(team) || board.isStaleMate(ENEMY(team))) {
        int value = board.valuePosition(team);

        if(value > 0) {
            return 5000;
        } else {
            return -500;
        }
    }

    Move bestMove(0, 0, 0);
    int value = board.alphaBeta(INT32_MIN / 100, INT32_MAX / 100,
                          SEARCH_DEPTH - 1, ENEMY(team), bestMove);

    return value;
}

Move Board::getBestMove(int team) {
    std::vector<Move> allMoves = getAllMoves(team);
    std::mutex moveMutex;

    std::string fen = getFENString();

    Move bestMove(0, 0, 0);
    int bestMoveValue;
    std::mutex bestMoveMutex;

    std::vector<std::future<void>> futures(allMoves.size());

    const unsigned long size = allMoves.size();
    int index = 0;
    for(int i = 0; i < size; i++) {
        futures[i] = ThreadPool::getInstance().enqueue([&] {
            std::unique_lock<std::mutex> moveLock(moveMutex);
            Move move = allMoves[0];
            allMoves.erase(allMoves.begin());
            moveLock.unlock();

            int value = -valueMove(fen, move, team);
            std::lock_guard<std::mutex> bestMoveLock(bestMoveMutex);
            if (bestMove.getFrom() == 0 && bestMove.getTo() == 0 && value != -30000) {
                bestMove = move;
                bestMoveValue = value;
            } else {
                if (value > bestMoveValue && value != -30000) {
                    bestMove = move;
                    bestMoveValue = value;
                }
            }
        });
    }

    for(auto &future : futures) {
        future.wait();
    }

    return bestMove;
}

/**
 * Counts how many moves the given team can make
 * @param team 0 for white and 1 for black
 * @return the amount of moves as int
 */
int Board::countMoves(int team) {

    int count = 0;
    int enemyTeam = ENEMY(team);

    count += getCardinality(Pawn::getAttackTargets(pawns[team], team) & pieces[enemyTeam]);
    count += getCardinality(Pawn::getMoveTargets(pawns[team], ~occupied, team));
    count += getCardinality(Rook::getTargets(rooks[team], pieces[team],
                                             pieces[enemyTeam]));
    count += getCardinality(Knight::getTargets(knights[team], pieces[team]));
    count += getCardinality(Bishop::getTargets(bishops[team], pieces[team],
                                               pieces[enemyTeam]));
    count += getCardinality(King::getTargets(kings[team], pieces[team]));

    return count;
}

int Board::alphaBeta(int alpha, int beta, int depthLeft, int team, Move &bestMove) {
    if(depthLeft == 0) {
        return quiesce(alpha, beta, 0);
    }

    std::vector<Move> allMoves = getAllMoves(actingTeam);

    if(checkMate(actingTeam)) {
        return -30000;
    }

    for(int i = 0; i < allMoves.size(); i++) {

        executeMove(allMoves[i]);

        if (inCheck(ENEMY(actingTeam))) {
            undoLastMove();
            continue;
        }

        if(threeFoldRepetition || isStaleMate(actingTeam)) {
            undoLastMove();
            int evaluation = valuePosition(actingTeam);

            if(evaluation >= 0) {
                return -100;
            } else if(evaluation < -500) {
                return 200;
            } else {
                return 100;
            }
        }


        int score = -alphaBeta(-beta, -alpha, depthLeft - 1, team, bestMove);
        undoLastMove();

        if(score >= beta) {
            return beta;
        }
        if(score > alpha) {
            alpha = score;
            if(actingTeam == team && depthLeft == SEARCH_DEPTH) {
                bestMove = allMoves[i];
            }
        }
    }

    return alpha;
}

int Board::quiesce(int alpha, int beta, int depth) {
    if(checkMate(actingTeam)) {
        return -30000;
    }

    int standPat = valuePosition(actingTeam);

    if(standPat >= beta) {
        return beta;
    } else if(alpha < standPat) {
        alpha = standPat;
    }

    std::vector<Move> allMoves = getAllMoves(actingTeam);

    for(Move move : allMoves) {
        if(!move.isCapture()) {
            continue;
        }

        executeMove(move);

        if(inCheck(ENEMY(actingTeam))) {
            undoLastMove();
            continue;
        }

        if(threeFoldRepetition || isStaleMate(actingTeam)) {
            undoLastMove();
            int evaluation = valuePosition(actingTeam);
            if(evaluation >= 0) {
                return -100;
            } else if(evaluation < -500) {
                return 200;
            } else {
                return 100;
            }
        }

        int score = -quiesce(-beta, -alpha, depth + 1);
        undoLastMove();

        if(score >= beta) {
            return beta;
        }

        int BIG_DELTA = 900;
        if(move.isPromotion()) {
            BIG_DELTA += 775;
        }

        if(score < alpha - BIG_DELTA) {
            return alpha;
        }

        if(score > alpha) {
            alpha = score;
        }
    }
    return alpha;
}

#define pieceSquareIndex(team, square) ((team) == BLACK ? (square) : 63 - (square))

int Board::valuePosition(int team) {
    int value = 0;
    int enemy = ENEMY(team);

    for(int i = 0; i < 64; i++) {
        U64 existsPawn = (1UL << i) & pawns[team];
        if (existsPawn) value += VALUE_PAWN + (Pawn::pieceSquareTable()[pieceSquareIndex(team, i)]);

        U64 existsPawnEnemy = (1UL << i) & pawns[enemy];
        if (existsPawnEnemy) value -= VALUE_PAWN + (Pawn::pieceSquareTable()[pieceSquareIndex(enemy, i)]);

        U64 existsKnight = (1UL << i) & knights[team];
        if (existsKnight) value += VALUE_KNIGHT + (Knight::pieceSquareTable()[pieceSquareIndex(team, i)]);

        U64 existsKnightEnemy = (1UL << i) & knights[enemy];
        if (existsKnightEnemy) value -= VALUE_KNIGHT + (Knight::pieceSquareTable()[pieceSquareIndex(enemy, i)]);

        U64 existsBishop = (1UL << i) & bishops[team];
        if (existsBishop) value += VALUE_BISHOP + (Bishop::pieceSquareTable()[pieceSquareIndex(team, i)]);

        U64 existsBishopEnemy = (1UL << i) & bishops[enemy];
        if (existsBishopEnemy) value -= VALUE_BISHOP + (Bishop::pieceSquareTable()[pieceSquareIndex(enemy, i)]);

        int amountPawnsEnemy = getCardinality(pawns[enemy]);
        U64 existsRook = (1UL << i) & rooks[team];
        if (existsRook) value += VALUE_ROOK + (Rook::pieceSquareTable()[pieceSquareIndex(team, i)]) +
                    ((8 - amountPawnsEnemy) * 10);

        int amountPawns = getCardinality(pawns[team]);
        U64 existsRookEnemy = (1UL << i) & rooks[enemy];
        if (existsRookEnemy) value -= VALUE_ROOK + (Rook::pieceSquareTable()[pieceSquareIndex(enemy, i)]) +
                    ((8 - amountPawns) * 10);

        U64 existsQueen = (1UL << i) & queens[team];
        if (existsQueen) value += VALUE_QUEEN + (Queen::pieceSquareTable()[pieceSquareIndex(team, i)]);

        U64 existsQueenEnemy = (1UL << i) & queens[enemy];
        if (existsQueenEnemy) value -= VALUE_QUEEN + (Queen::pieceSquareTable()[pieceSquareIndex(enemy, i)]);

        U64 existsKing = (1UL << i) & kings[team];
        if (existsKing) value += VALUE_KING + (King::pieceSquareTable(queens)[pieceSquareIndex(team, i)]);

        U64 existsKingEnemy = (1UL << i) & kings[enemy];
        if (existsKingEnemy) value -= VALUE_KING + (King::pieceSquareTable(queens)[pieceSquareIndex(enemy, i)]);
    }

    // Mobility score
    value += VALUE_MOBILITY * (countMoves(team) - countMoves(enemy));

    if(checkMate(ENEMY(team))) {
        value += 30000;
    } else if(checkMate(team)) {
        value -= 30000;
    }

    return value;
}