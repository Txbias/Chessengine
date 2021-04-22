#pragma once

#define FLAG_QUIET 0
#define FLAG_PAWN_DBL_PUSH 1
#define FLAG_CAPTURE 4

int getCardinality(unsigned long bitboard);
unsigned long nortOne(unsigned long bitboard);
unsigned long soutOne(unsigned long bitboard);
void setBit(unsigned long &bitboard, unsigned int pos);
void setRow(unsigned long &bitboard, unsigned int row);
unsigned long setFile(unsigned long bitboard, unsigned long file);