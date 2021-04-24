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
            unsigned int k = 0;
            while(!(ownPieces & (origin << k))) {
                k += 8;
            }

            // Check if a enemy piece gets captured
            unsigned int flag = getFlag(i, enemyPieces);

            Move move(i-k, i, flag);
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
            unsigned int origin = 1UL << i;
            unsigned int k = 1;
            while(!(ownPieces & (origin >> k))) {
                k++;
            }

            unsigned int flag = getFlag(i, enemyPieces);
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
            unsigned int origin = 1UL << i;
            unsigned int k = 1;
            while(!(ownPieces & (origin << k))) {
                k++;
            }

            unsigned int flag = getFlag(i, enemyPieces);
            Move move(i+k, i, flag);
            moves.emplace_back(move);
        }
    }

    return moves;
}