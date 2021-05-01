#include "utils.h"


int getCardinality(unsigned long bitboard) {
    int cardinality = 0;
    for(int i = 0; i < 64; i++) {
        if(bitboard & (1UL << i)) {
            cardinality++;
        }
    }

    return cardinality;
}

void setBit(unsigned long &bitboard, unsigned int pos) {
    bitboard |= (1UL << pos);
}

unsigned long setRow(unsigned long bitboard, unsigned int row) {
    for(int i = 0; i < 8; i++) {
        setBit(bitboard, row * 8 + i);
    }
    return bitboard;
}

unsigned long setFile(unsigned long bitboard, unsigned long file) {
    for(int i = 0; i < 8; i++) {
        setBit(bitboard, i * 8 + file);
    }
    return bitboard;
}

unsigned int getFlag(unsigned int target, U64 enemyPieces) {

    if((1UL << target) & enemyPieces) {
        return FLAG_CAPTURE;
    } else {
        return FLAG_QUIET;
    }
}

U64 eastOne(U64 bitboard) {
    return (bitboard & notHFile) << 1;
}

U64 westOne(U64 bitboard) {
    return (bitboard & notAFile) >> 1;
}

U64 northOne(U64 bitboard) {
    return (bitboard & not8Row) << 8;
}

U64 southOne(U64 bitboard) {
    return (bitboard & not1Row) >> 8;
}

U64 northEast(U64 bitboard){
    return eastOne(northOne(bitboard));
}

U64 northWest(U64 bitboard){
    return westOne(northOne(bitboard));
}

U64 southEast(U64 bitboard){
    return eastOne(southOne(bitboard));
}

U64 southWest(U64 bitboard){
    return westOne(southOne(bitboard));
}


std::vector<Move> getSlidingMovesNorth(U64 pieces, U64 ownPieces, U64 enemyPieces) {
    std::vector<Move> moves;

    while((pieces = northOne(pieces) & ~ownPieces) != 0) {
        for(unsigned int i = 0; i < 64; i++) {
            if(!(pieces & (1UL << i))) {
                // No piece at current position
                continue;
            }

            // Find origin position
            U64 origin = 1UL << i;
            unsigned int k = 0;
            while(!(ownPieces & (origin >> k))) {
                k += 8;
            }

            // Check if a enemy piece gets captured
            unsigned int flag = getFlag(i, enemyPieces);

            if(flag == FLAG_CAPTURE) {
                // Remove piece from bitboard, to prevent moves trough other
                // pieces
                U64 u = 1UL << (i-k);
                u = ~u;
                pieces = pieces & u;
            }

            Move move(i-k, i, flag);
            moves.emplace_back(move);
        }
    }

    return moves;
}

std::vector<Move> getSlidingMovesSouth(U64 pieces, U64 ownPieces, U64 enemyPieces) {
    std::vector<Move> moves;

    while((pieces = southOne(pieces) & ~ownPieces) != 0) {
        for(unsigned int i = 0; i < 64; i++) {
            if(!(pieces & (1UL << i))) {
                // No piece at current position
                continue;
            }

            // Find origin position
            U64 origin = 1UL << i;
            unsigned int k = 8;
            while(!(ownPieces & (origin << k))) {
                k += 8;
            }

            // Check if a enemy piece gets captured
            unsigned int flag = getFlag(i, enemyPieces);

            if(flag == FLAG_CAPTURE) {
                // Remove piece from bitboard, to prevent moves trough other
                // pieces
                U64 u = 1UL << (i-k);
                u = ~u;
                pieces = pieces & u;
            }

            Move move(i+k, i, flag);
            moves.emplace_back(move);
        }
    }

    return moves;
}

std::vector<Move> getSlidingMovesEast(U64 pieces, U64 ownPieces, U64 enemyPieces) {

    std::vector<Move> moves;
    while((pieces = eastOne(pieces) & ~ownPieces) != 0) {
        for(unsigned int i = 0; i < 64; i++) {
            if(!(pieces & (1UL << i))) {
                // No piece at current position
                continue;
            }

            //Find origin position
            unsigned long origin = 1UL << i;
            unsigned int k = 1;
            while(!(ownPieces & (origin >> k))) {
                k++;
            }

            unsigned int flag = getFlag(i, enemyPieces);

            if(flag == FLAG_CAPTURE) {
                // Remove piece from bitboard, to prevent moves trough other
                // pieces
                U64 u = 1UL << (i - k);
                u = ~u;
                pieces = pieces & u;
            }

            Move move(i-k, i, flag);
            moves.emplace_back(move);
        }
    }

    return moves;
}

std::vector<Move> getSlidingMovesWest(U64 pieces, U64 ownPieces, U64 enemyPieces) {
    std::vector<Move> moves;

    while((pieces = westOne(pieces) & ~ownPieces) != 0) {
        for(unsigned int i = 0; i < 64; i++) {
            if(!(pieces & (1UL << i))) {
                // No piece at current position
                continue;
            }

            //Find origin position
            unsigned long origin = 1UL << i;
            unsigned int k = 1;
            while(!(ownPieces & (origin << k))) {
                k++;
            }

            unsigned int flag = getFlag(i, enemyPieces);

            if(flag == FLAG_CAPTURE) {
                // Remove piece from bitboard, to prevent moves trough other
                // pieces
                U64 u = 1UL << (i+k);
                u = ~u;
                pieces = pieces & u;
            }

            Move move(i+k, i, flag);
            moves.emplace_back(move);
        }
    }

    return moves;
}

std::vector<Move> getSlidingMovesNorthEast(U64 slidingPieces, U64 ownPieces, U64 enemyPieces) {
    return getSlidingMoves(northEast, southWest, slidingPieces, ownPieces, enemyPieces);
}

std::vector<Move> getSlidingMovesNorthWest(U64 slidingPieces, U64 ownPieces, U64 enemyPieces) {
    return getSlidingMoves(northWest, southEast, slidingPieces, ownPieces, enemyPieces);
}

std::vector<Move> getSlidingMovesSouthEast(U64 slidingPieces, U64 ownPieces, U64 enemyPieces) {
    return getSlidingMoves(southEast, northWest, slidingPieces, ownPieces, enemyPieces);
}

std::vector<Move> getSlidingMovesSouthWest(U64 slidingPieces, U64 ownPieces, U64 enemyPieces) {
    return getSlidingMoves(southWest, northEast, slidingPieces, ownPieces, enemyPieces);
}

std::vector<Move> getSlidingMoves(bitShiftFunction direction, bitShiftFunction oppositeDirection,
                                  U64 slidingPieces, U64 ownPieces, U64 enemyPieces) {

    std::vector<Move> moves;
    U64 ownPiecesWithoutMoving = ownPieces & ~slidingPieces;

    while((slidingPieces = direction(slidingPieces)) != 0) {
        for(unsigned int i = 0; i < 64; i++) {
            if(!(slidingPieces & (1UL << i))) {
                continue;
            }

            // Find origin position
            unsigned long origin = 1UL << i;
            while(!(ownPieces & origin)) {
                origin = oppositeDirection(origin);
            }

            unsigned int from = -1;
            for(unsigned int j = 0; j < 64; j++) {
                if((origin & (1UL << j))) {
                    from = j;
                    break;
                }
            }

            if(from == -1) {
                std::cerr << "Origin position could not be found" << std::endl;
                return moves;
            }

            unsigned int flag = getFlag(i, enemyPieces);
            if(flag == FLAG_CAPTURE || ownPiecesWithoutMoving & (1UL << i)) {
                // Remove piece from bitboard, to prevent moves trough other
                // slidingPieces
                U64 u = 1UL << from;
                u = ~u;
                slidingPieces = slidingPieces & u;
            }

            if(ownPiecesWithoutMoving & (1UL << i)) {
                // Own Piece at target position
                continue;
            }

            Move move(from, i, flag);
            moves.emplace_back(move);
        }
    }

    return moves;
}