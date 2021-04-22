#include "utils.h"


unsigned long nortOne(unsigned long bitboard) {
    return bitboard << 8;
}

unsigned long soutOne(unsigned long bitboard) {
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