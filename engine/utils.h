#pragma once

#include <vector>
#include "move.h"


typedef U64(*bitShiftFunction)(U64);


int getCardinality(unsigned long bitboard);
int bitScanForward(U64 bitboard);

// helper functions for shifting bitboards
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

/**
 * Calculates the indices of all set bits
 * @return vector with indices of set bits
 */
std::vector<int> getSetBits(U64 bitboard);


/**
 * Calculates all squares attackable by some sliding pieces
 * @param slidingPieces Bitboard with sliding pieces
 * @param ownPieces     Bitboard with all friendly pieces
 * @param enemyPieces   Bitboard with all enemy pieces
 * @param directions    Vector of functions that shift a given bitboard
 *                      in the direction the pieces can move
 * @param countBlocked  Sets whether squares with pieces, that can
 *                      be captured should be included
 *
 * @return  Bitboard with all squares that can be attacked marked
 */
U64 getSlidingTargets(U64 slidingPieces, U64 ownPieces, U64 enemyPieces,
                      const std::vector<bitShiftFunction>& directions, bool countBlocked=false);

/**
 * @brief Initializes all arrays needed for the magic bitboards.
 * Needs to be called at the start of the program
 */
void initializeMagicBitboards();

U64 getBlockersFromIndex(int index, U64 mask);

// functions for accessing precalculated moves
U64 getBishopMasks(int square);
U64 getRookMasks(int square);
U64 getBishopAttacks(int square, U64 key);
U64 getRookAttacks(int square, U64 key);
int getBishopIndexBit(int square);
int getRookIndexBit(int square);
U64 getBishopMagic(int square);
U64 getRookMagic(int square);


