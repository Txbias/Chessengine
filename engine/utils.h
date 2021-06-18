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

#define ENEMY(team) (team == 0  ? 1 : 0)

typedef unsigned long U64;
typedef U64(*bitShiftFunction)(U64);

const U64 notAFile = 0xFEFEFEFEFEFEFEFE;
const U64 notHFile = 0x7F7F7F7F7F7F7F7F;
const U64 not1Row = 0xFFFFFFFFFFFFFF00;
const U64 not8Row = 0xFFFFFFFFFFFFFF;

const U64 k1 = 0x5555555555555555;
const U64 k2 = 0x3333333333333333;
const U64 k4 = 0x0f0f0f0f0f0f0f0f;
const U64 kf = 0x0101010101010101;

int getCardinality(unsigned long bitboard);
int bitScanForward(U64 bitboard);

U64 eastOne(U64 bitboard);
U64 westOne(U64 bitboard);
U64 northOne(U64 bitboard);
U64 southOne(U64 bitboard);
U64 northEast(U64 bitboard);
U64 northWest(U64 bitboard);
U64 southEast(U64 bitboard);
U64 southWest(U64 bitboard);

U64 northFill(U64 bitboard);
U64 southFill(U64 bitboard);
U64 fileFill(U64 bitboard);

void setBit(unsigned long &bitboard, unsigned int pos);
unsigned long setRow(unsigned long bitboard, unsigned int row);
unsigned long setFile(unsigned long bitboard, unsigned long file);
unsigned int getFlag(unsigned int target, U64 enemyPieces);

std::vector<int> getSetBits(U64 bitboard);

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
