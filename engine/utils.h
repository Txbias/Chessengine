#pragma once

#include <vector>
#include "Move.h"

#define FLAG_QUIET 0
#define FLAG_PAWN_DBL_PUSH 1
#define FLAG_KING_CASTLE 2
#define FLAG_QUEEN_CASTLE 3
#define FLAG_CAPTURE 4
#define FLAG_EP_CAPTURE 5
#define FLAG_QUEEN_PROMOTION 11
#define FLAG_QUEEN_PROMOTION_CAPTURE 15

typedef unsigned long U64;
typedef U64(*bitShiftFunction)(U64);

int getCardinality(unsigned long bitboard);

U64 eastOne(U64 bitboard);
U64 westOne(U64 bitboard);
U64 northOne(U64 bitboard);
U64 southOne(U64 bitboard);
U64 northEast(U64 bitboard);
U64 northWest(U64 bitboard);
U64 southEast(U64 bitboard);
U64 southWest(U64 bitboard);

void setBit(unsigned long &bitboard, unsigned int pos);
unsigned long setRow(unsigned long bitboard, unsigned int row);
unsigned long setFile(unsigned long bitboard, unsigned long file);
unsigned int getFlag(unsigned int target, U64 enemyPieces);

std::vector<Move> getSlidingMovesNorth(U64 pieces, U64 ownPieces, U64 enemyPieces);
std::vector<Move> getSlidingMovesSouth(U64 pieces, U64 ownPieces, U64 enemyPieces);
std::vector<Move> getSlidingMovesEast(U64 pieces, U64 ownPieces, U64 enemyPieces);
std::vector<Move> getSlidingMovesWest(U64 pieces, U64 ownPieces, U64 enemyPieces);

std::vector<Move> getSlidingMovesNorthEast(U64 slidingPieces, U64 ownPieces, U64 enemyPieces);
std::vector<Move> getSlidingMovesNorthWest(U64 pieces, U64 ownPieces, U64 enemyPieces);
std::vector<Move> getSlidingMovesSouthEast(U64 pieces, U64 ownPieces, U64 enemyPieces);
std::vector<Move> getSlidingMovesSouthWest(U64 pieces, U64 ownPieces, U64 enemyPieces);
std::vector<Move> getSlidingMoves(bitShiftFunction direction, bitShiftFunction oppositeDirection,
                                  U64 slidingPieces, U64 OwnPieces, U64 enemyPieces);

U64 getSlidingTargets(U64 slidingPieces, U64 ownPieces, U64 enemyPieces,
                      const std::vector<bitShiftFunction>& directions, bool countBlocked=false);

const U64 notAFile = ~setFile(0UL, 0);
const U64 notHFile = ~setFile(0UL, 7);
const U64 not1Row = ~setRow(0UL, 0);
const U64 not8Row = ~setRow(0UL, 7);