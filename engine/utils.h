#pragma once

#include <vector>
#include "move.h"

#define FLAG_QUIET 0
#define FLAG_PAWN_DBL_PUSH 1
#define FLAG_KING_CASTLE 2
#define FLAG_QUEEN_CASTLE 3
#define FLAG_CAPTURE 4
#define FLAG_EP_CAPTURE 5
#define FLAG_KNIGHT_PROMOTION 8
#define FLAG_BISHOP_PROMOTION 9
#define FLAG_ROOK_PROMOTION 10
#define FLAG_QUEEN_PROMOTION 11
#define FLAG_KNIGHT_PROMOTION_CAPTURE 12
#define FLAG_BISHOP_PROMOTION_CAPTURE 13
#define FLAG_ROOK_PROMOTION_CAPTURE 14
#define FLAG_QUEEN_PROMOTION_CAPTURE 15

#define ENEMY(team) (team == 0  ? 1 : 0)

typedef unsigned long U64;
typedef U64(*bitShiftFunction)(U64);


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

U64 wFrontSpans(U64 bitboard);
U64 bFrontSpans(U64 bitboard);

unsigned long setRow(unsigned long bitboard, unsigned int row);
unsigned long setFile(unsigned long bitboard, unsigned long file);
unsigned long randomU64Number();
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

/**
 * @brief Initializes all arrays needed for the magic bitboards.
 * Needs to be called at the start of the program
 */
void initializeMagicBitboards();

U64 getBlockersFromIndex(int index, U64 mask);

U64 getBishopMasks(int square);
U64 getRookMasks(int square);
U64 getBishopAttacks(int square, U64 key);
U64 getRookAttacks(int square, U64 key);
int getBishopIndexBit(int square);
int getRookIndexBit(int square);
U64 getBishopMagic(int square);
U64 getRookMagic(int square);


