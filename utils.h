#pragma once

#define FLAG_QUIET 0
#define FLAG_PAWN_DBL_PUSH 1
#define FLAG_CAPTURE 4

inline unsigned long nortOne(unsigned long bitboard) {
    return bitboard << 8;
}

inline unsigned long soutOne(unsigned long bitboard) {
    return bitboard >> 8;
}


void setBit(unsigned long &bitboard, unsigned int pos) {
    bitboard |= (1UL << pos);
}

void setRow(unsigned long &bitboard, unsigned int row) {
    for(int i = 0; i < 8; i++) {
        setBit(bitboard, row * 8 + i);
    }
}

void setFile(unsigned long &bitboard, unsigned long file) {
    for(int i = 0; i < 8; i++) {
        setBit(bitboard, i * 8 + file);
    }
}