#include <mutex>

#include "board.h"
#include "thread_pool.h"
#include "search.h"
#include "pawns.h"
#include "knight.h"
#include "rook.h"
#include "bishop.h"
#include "queen.h"
#include "king.h"

#define MIN(x, y) (x < y ? x : y)
#define MAX(x, y) (x > y ? x : y)

bool initialized = false;
int SEARCH_DEPTH = 4;

// random piece keys [piece][square]
U64 pieceKeys[12][64];

// random enPassant keys
U64 enPassantKeys[64];

// random castling keys
U64 castlingKeys[16];

// random side key
U64 sideKey;

void setSearchDepth(int depth) {
    SEARCH_DEPTH = depth;
}

int getSearchDepth() {
    return SEARCH_DEPTH;
}

void initializeHashKeys() {
    if(initialized) {
        return;
    }
    initialized = true;
    for(int square = 0; square < 64; square++) {
        for(int i = 0; i < 12; i++) {
            pieceKeys[i][square] = randomU64Number();
        }

        enPassantKeys[square] = randomU64Number();
    }

    for(int i = 0; i < 16; i++) {
        castlingKeys[i] = randomU64Number();
    }

    sideKey = randomU64Number();
}

Board::Board() {

    transpositionTable = std::make_shared<TranspositionTable>();
    initializeHashKeys();

    initializeMagicBitboards();

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

    U64 currPos = getPositionHash();
    positions.insert(std::make_pair(currPos, 1));
}

Board::Board(const std::string& fen) {

    transpositionTable = std::make_shared<TranspositionTable>();
    initializeHashKeys();

    initializeMagicBitboards();

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

    if(fen.at(fenPosition) == ' ') fenPosition++;

    if(fen.at(fenPosition) != '-') {
        // En passant target
        int file = fen.at(fenPosition) - 97;
        int row = (fen.at(fenPosition + 1) - 48) - 1;

        enPassantSquare = row * 8 + file;
        enPassantTarget = 1UL << enPassantSquare;
        fenPosition++;
    } else {
        enPassantSquare = 0;
        enPassantTarget = 0;
    }

    fenPosition += 2;
    //TODO: implement half move clock

    fenPosition++;
    if(fen.at(fenPosition) != ' ') {
        fenPosition++;
    }

    amountFullMoves = std::stoi(fen.substr(fenPosition));

    U64 posHash = getPositionHash();
    positions.insert(std::make_pair(posHash, 1));
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

    unsigned long whiteRook = 1UL | (1UL << 7);
    rooks[WHITE] = whiteRook;
    occupied |= whiteRook;
    pieces[WHITE] |= whiteRook;

    unsigned long blackRook = (1UL << 63) | (1UL << 56);
    rooks[BLACK] = blackRook;
    occupied |= blackRook;
    pieces[BLACK] |= blackRook;

    unsigned long whiteKnight = (1UL << 1) | (1UL << 6);
    knights[WHITE] = whiteKnight;
    occupied |= whiteKnight;
    pieces[WHITE] |= whiteKnight;

    unsigned long blackKnight = (1UL << 57) | (1UL << 62);
    knights[BLACK] = blackKnight;
    occupied |= blackKnight;
    pieces[BLACK] |= blackKnight;

    unsigned long whiteBishop = (1UL << 2) | (1UL << 5);
    bishops[WHITE] = whiteBishop;
    occupied |= whiteBishop;
    pieces[WHITE] |= whiteBishop;

    unsigned long blackBishop = (1UL << 58) | (1UL << 61);
    bishops[BLACK] = blackBishop;
    occupied |= blackBishop;
    pieces[BLACK] |= blackBishop;

    unsigned long whiteQueen = 1UL << 3;
    queens[WHITE] = whiteQueen;
    occupied |= whiteQueen;
    pieces[WHITE] |= whiteQueen;

    unsigned long blackQueen = 1UL << 59;
    queens[BLACK] = blackQueen;
    occupied |= blackQueen;
    pieces[BLACK] |= blackQueen;

    unsigned long whiteKing = 1UL << 4;
    kings[WHITE] = whiteKing;
    occupied |= whiteKing;
    pieces[WHITE] |= whiteKing;

    unsigned long blackKing = 1UL << 60;
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
        // Save en passant square for the next move
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
            std::cerr << "Couldn't identify captured piece" << std::endl;
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
        if(move.getFlags() == FLAG_QUEEN_PROMOTION || move.getFlags() == FLAG_QUEEN_PROMOTION_CAPTURE) {
            queens[team] |= target;
        } else if(move.getFlags() == FLAG_ROOK_PROMOTION || move.getFlags() == FLAG_ROOK_PROMOTION_CAPTURE) {
            rooks[team] |= target;
        } else if(move.getFlags() == FLAG_BISHOP_PROMOTION || move.getFlags() == FLAG_BISHOP_PROMOTION_CAPTURE) {
            bishops[team] |= target;
        } else if(move.getFlags() == FLAG_KNIGHT_PROMOTION || move.getFlags() == FLAG_KNIGHT_PROMOTION_CAPTURE) {
            knights[team] |= target;
        }
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

        } else {
            // Queen castle
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

    actingTeam = ENEMY(team);
    if(team == BLACK) {
        amountFullMoves++;
    }

    // Save current position
    U64 currPosHash = getPositionHash();
    auto it2 = positions.find(currPosHash);
    if(it2 == positions.end()) {
        positions[currPosHash] = 1;
    } else {
        positions[currPosHash]++;
        if(positions[currPosHash] == 3) {
            threeFoldRepetition = true;
        }
    }
}

U64 Board::getPositionHash() {
    U64 key = 0UL;

    U64* allPieces[12] {
            &pawns[0], &pawns[1],
            &knights[0], &knights[1],
            &bishops[0], &bishops[1],
            &rooks[0], &rooks[1],
            &queens[0], &queens[1],
            &kings[0], &kings[1]
    };

    for(int i = 0; i < 12; i++) {
        U64 bitboard = *allPieces[i];
        U64 bitboardCopy = bitboard;

        if(bitboardCopy) {
            do {
                int square = bitScanForward(bitboardCopy);

                // hash piece
                key ^= pieceKeys[i][square];

            } while (bitboardCopy &= bitboardCopy - 1);
        }
    }
    if(enPassantTarget) {
        key ^= enPassantKeys[enPassantSquare];
    }

    unsigned int castlingRights = 0;

    if(!kingMoved[0]) {
        if(rookMoved[0][0]) {
            castlingRights |= 1;
        }
        if(rookMoved[0][1]) {
            castlingRights |= 1 << 1;
        }
    }
    if(!kingMoved[1]) {
        if(rookMoved[1][0]) {
            castlingRights |= 1 << 2;
        }
        if(rookMoved[1][1]) {
            castlingRights |= 1 << 3;
        }
    }
    key ^= castlingKeys[castlingRights];

    if(actingTeam == BLACK) {
        key ^= sideKey;
    }

    return key;
}

void Board::undoLastMove() {
    if(moves.empty()) {
        std::cerr << "No move left!" << std::endl;
        return;
    }

    U64 currPosHash = getPositionHash();
    auto it = positions.find(currPosHash);

    if(it->second == 1) {
        positions.erase(it);
    } else {
        if(it->second == 3) {
            threeFoldRepetition = false;
        }
        positions[currPosHash]--;
    }

    Move move = moves.top();
    moves.pop();

    int team = getTeam(move.getTo());
    int enemyTeam = ENEMY(team);

    actingTeam = ENEMY(actingTeam);

    unsigned long* movedPieces = getTargetPieces(move.getTo(), team);

    // Save new position of piece
    U64 originSquare = 1UL << move.getFrom();
    if(move.isPromotion()) {
        pawns[team] |= originSquare;
    } else {
        movedPieces[team] |= originSquare;
    }
    occupied |= originSquare;
    pieces[team] |= originSquare;

    // Clear old piece position
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

        } else {
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
    if(enPassantSquare != 0) {
        enPassantTarget = 1UL << move.getEPSquareBefore();
    } else {
        enPassantTarget = 0UL;
    }

    if(team == BLACK) {
        amountFullMoves--;
    }
}

void Board::printBoard() {
    std::cout << getBoardPrintable() << std::endl;
}

void Board::setTranspositionTable(std::shared_ptr<TranspositionTable> t) {
    this->transpositionTable = std::move(t);

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
    U64 enemyPieces = pieces[enemyTeam];
    enemyPieces &= ~kings[enemyTeam];

    targets |= (Pawn::getAttackTargets(pawns[team], team) & pieces[enemyTeam]);
    targets |= Rook::getTargets(rooks[team], pieces[team],
                                enemyPieces, countBlocked);
    targets |= Knight::getTargets(knights[team], pieces[team]);
    targets |= Bishop::getTargets(bishops[team], pieces[team],
                                  enemyPieces, countBlocked);
    targets |= Queen::getTargets(queens[team], pieces[team],
                                 enemyPieces, countBlocked);
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
    U64 enemyTargets = getTargetMap(enemyTeam, true, true);
    enemyTargets |= Pawn::getAttackTargets(pawns[enemyTeam], enemyTeam) & pieces[enemyTeam];

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
    allMoves.reserve(332);
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

Move Board::getBestMove(int team) {
    return Search::getBestMove(getFENString(), transpositionTable, team);
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

int Board::countDoublePawns(int team) {
    static const U64 aFile = 0x101010101010101;
    static const U64 bFile = 0x202020202020202;
    static const U64 cFile = 0x404040404040404;
    static const U64 dFile = 0x808080808080808;
    static const U64 eFile = 0x1010101010101010;
    static const U64 fFile = 0x2020202020202020;
    static const U64 gFile = 0x4040404040404040;
    static const U64 hFile = 0x8080808080808080;

    int count = 0;
    if(pawns[team] & aFile) count++;
    if(pawns[team] & bFile) count++;
    if(pawns[team] & cFile) count++;
    if(pawns[team] & dFile) count++;
    if(pawns[team] & eFile) count++;
    if(pawns[team] & fFile) count++;
    if(pawns[team] & gFile) count++;
    if(pawns[team] & hFile) count++;

    return getCardinality(pawns[team]) - count;
}

#define pieceSquareIndex(team, square) ((team) == BLACK ? (square) : 63 - (square))

int Board::valuePosition(int team) {
    int value = 0;
    int enemy = ENEMY(team);

    int countBishops[2] = {};

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
        if (existsBishop) {
            value += VALUE_BISHOP;
            countBishops[team]++;
        }

        U64 existsBishopEnemy = (1UL << i) & bishops[enemy];
        if (existsBishopEnemy) {
            value -= VALUE_BISHOP;
            countBishops[enemy]++;
        }

        int amountPawnsEnemy = getCardinality(pawns[enemy]);
        U64 existsRook = (1UL << i) & rooks[team];
        if (existsRook) value += Rook::getRookValue(pawns, i, team, VALUE_ROOK);

        int amountPawns = getCardinality(pawns[team]);
        U64 existsRookEnemy = (1UL << i) & rooks[enemy];
        if (existsRookEnemy) value -= Rook::getRookValue(pawns, i, enemy, VALUE_ROOK);

        U64 existsQueen = (1UL << i) & queens[team];
        if (existsQueen) value += VALUE_QUEEN;

        U64 existsQueenEnemy = (1UL << i) & queens[enemy];
        if (existsQueenEnemy) value -= VALUE_QUEEN;

        U64 existsKing = (1UL << i) & kings[team];
        if (existsKing) value += VALUE_KING;

        U64 existsKingEnemy = (1UL << i) & kings[enemy];
        if (existsKingEnemy) value -= VALUE_KING;
    }

    // Mobility score
    value += VALUE_MOBILITY * (countMoves(team) - countMoves(enemy));

    // Bonus for bishop pair
    if(countBishops[team] == 2) {
        value += 30;
    }
    if(countBishops[enemy] == 2) {
        value -= 30;
    }

    int passedPawns = getCardinality(Pawn::passedPawns(pawns, team));
    int enemyPassedPawns = getCardinality(Pawn::passedPawns(pawns, enemy));

    value += passedPawns * 8;
    value -= enemyPassedPawns * 8;

    if(getCardinality(occupied) <= 12) {
        // Center control
        U64 targetMap = getTargetMap(team, false);
        U64 enemyTargetMap = getTargetMap(enemy, false);

        targetMap |= pieces[team];
        enemyTargetMap |= pieces[enemy];


        if (targetMap & (1UL << 26)) {
            value += 6;
        }
        if (targetMap & (1UL << 27)) {
            value += 6;
        }
        if (targetMap & (1UL << 35)) {
            value += 6;
        }
        if (targetMap & (1UL << 36)) {
            value += 6;
        }

        if (enemyTargetMap & (1UL << 26)) {
            value -= 6;
        }
        if (enemyTargetMap & (1UL << 27)) {
            value -= 6;
        }
        if (enemyTargetMap & (1UL << 35)) {
            value -= 6;
        }
        if (enemyTargetMap & (1UL << 36)) {
            value -= 6;
        }
    }

    int doubledPawns = countDoublePawns(team);
    value -= 20 * doubledPawns;

    int enemyDoublePawns = countDoublePawns(enemy);
    value += 20 * enemyDoublePawns;

    if(checkMate(ENEMY(team))) {
        value += 30000;
    } else if(checkMate(team)) {
        value -= 30000;
    }

    return value;
}