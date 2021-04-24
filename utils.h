#pragma once

#include <vector>
#include "Move.h"

#define FLAG_QUIET 0
#define FLAG_PAWN_DBL_PUSH 1
#define FLAG_CAPTURE 4
typedef unsigned long U64;

int getCardinality(unsigned long bitboard);

U64 eastOne(U64 bitboard);
U64 westOne(U64 bitboard);
U64 northOne(U64 bitboard);
U64 southOne(U64 bitboard);

void setBit(unsigned long &bitboard, unsigned int pos);
unsigned long setRow(unsigned long bitboard, unsigned int row);
unsigned long setFile(unsigned long bitboard, unsigned long file);
unsigned int getFlag(unsigned int target, U64 enemyPieces);

std::vector<Move> getSlidingMovesNorth(U64 pieces, U64 ownPieces, U64 enemyPieces);
std::vector<Move> getSlidingMovesSouth(U64 pieces, U64 ownPieces, U64 enemyPieces);
std::vector<Move> getSlidingMovesEast(U64 pieces, U64 ownPieces, U64 enemyPieces);
std::vector<Move> getSlidingMovesWest(U64 pieces, U64 ownPieces, U64 enemyPieces);



const U64 notAFile = ~setFile(0UL, 0);
const U64 notHFile = ~setFile(0UL, 7);
const U64 not1Row = ~setRow(0UL, 0);
const U64 not8Row = ~setRow(0UL, 7);