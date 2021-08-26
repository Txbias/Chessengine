#include "utils.h"

#include <random>

/**
 * Calculates the number of set bits in the given bitboard
 */
int getCardinality(unsigned long bitboard) {

    static const U64 k1 = 0x5555555555555555;
    static const U64 k2 = 0x3333333333333333;
    static const U64 k4 = 0x0f0f0f0f0f0f0f0f;
    static const U64 kf = 0x0101010101010101;

    bitboard = bitboard - ((bitboard >> 1) & k1);
    bitboard = (bitboard & k2) + ((bitboard >> 2) & k2);
    bitboard = (bitboard + (bitboard >> 4)) & k4;
    bitboard = (bitboard * kf) >> 56;
    return (int) bitboard;
}

int bitScanForward(U64 bitboard) {
    return getCardinality((bitboard & -bitboard) - 1);
}

unsigned long randomU64Number() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return gen();
}

unsigned long setRow(unsigned long bitboard, unsigned int row) {
    for(int i = 0; i < 8; i++) {
        bitboard |= (1UL << (row * 8 + i));
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

const static U64 notHFile = 0x7F7F7F7F7F7F7F7F;
const static U64 notAFile = 0xFEFEFEFEFEFEFEFE;

U64 eastOne(U64 bitboard) {
    return (bitboard & notHFile) << 1;
}

U64 westOne(U64 bitboard) {
    return (bitboard & notAFile) >> 1;
}

U64 northOne(U64 bitboard) {
    const static U64 not8Row = 0xFFFFFFFFFFFFFF;
    return (bitboard & not8Row) << 8;
}

U64 southOne(U64 bitboard) {
    const static U64 not1Row = 0xFFFFFFFFFFFFFF00;
    return (bitboard & not1Row) >> 8;
}

U64 northEast(U64 bitboard){
    return eastOne(northOne(bitboard));
}

U64 northWest(U64 bitboard){
    return (bitboard << 7) & notHFile;
}

U64 southEast(U64 bitboard){
    return (bitboard >> 7) & notAFile;
}

U64 southWest(U64 bitboard){
    return (bitboard >> 9) & notHFile;
}

U64 northFill(U64 bitboard) {
    bitboard |= (bitboard <<  8);
    bitboard |= (bitboard << 16);
    bitboard |= (bitboard << 32);
    return bitboard;
}

U64 southFill(U64 bitboard) {
    bitboard |= (bitboard >>  8);
    bitboard |= (bitboard >> 16);
    bitboard |= (bitboard >> 32);
    return bitboard;
}

U64 fileFill(U64 bitboard) {
    return southFill(bitboard) | northFill(bitboard);
}

U64 wFrontSpans(U64 bitboard) {
    return northOne(northFill(bitboard));
}

U64 bFrontSpans(U64 bitboard) {
    return southOne(southFill(bitboard));
}

std::vector<int> getSetBits(U64 bitboard) {
    std::vector<int> indices(64);
    int index = 0;
    if(bitboard) do {
        indices[index++] = bitScanForward(bitboard);
    } while(bitboard &= bitboard - 1);
    indices.resize(index);

    return indices;
}

U64 getSlidingTargets(U64 slidingPieces, U64 ownPieces, U64 enemyPieces,
                      const std::vector<bitShiftFunction>& directions, bool countBlocked) {

    U64 targets = 0UL;
    for(auto direction : directions) {
        U64 piecesCopy = slidingPieces;
        if(countBlocked) {
            while ((piecesCopy = direction(piecesCopy)) != 0) {
                targets |= piecesCopy;
                piecesCopy &= ~enemyPieces;
                piecesCopy &= ~ownPieces;
            }
        } else {
            while((piecesCopy = (direction(piecesCopy) & ~ownPieces)) != 0) {
                targets |= piecesCopy;
                piecesCopy &=~enemyPieces;
            }
        }
    }

    return targets;
}

const U64 rookMagics[64] = {
        0xa8002c000108020ULL, 0x6c00049b0002001ULL, 0x100200010090040ULL, 0x2480041000800801ULL, 0x280028004000800ULL,
        0x900410008040022ULL, 0x280020001001080ULL, 0x2880002041000080ULL, 0xa000800080400034ULL, 0x4808020004000ULL,
        0x2290802004801000ULL, 0x411000d00100020ULL, 0x402800800040080ULL, 0xb000401004208ULL, 0x2409000100040200ULL,
        0x1002100004082ULL, 0x22878001e24000ULL, 0x1090810021004010ULL, 0x801030040200012ULL, 0x500808008001000ULL,
        0xa08018014000880ULL, 0x8000808004000200ULL, 0x201008080010200ULL, 0x801020000441091ULL, 0x800080204005ULL,
        0x1040200040100048ULL, 0x120200402082ULL, 0xd14880480100080ULL, 0x12040280080080ULL, 0x100040080020080ULL,
        0x9020010080800200ULL, 0x813241200148449ULL, 0x491604001800080ULL, 0x100401000402001ULL, 0x4820010021001040ULL,
        0x400402202000812ULL, 0x209009005000802ULL, 0x810800601800400ULL, 0x4301083214000150ULL, 0x204026458e001401ULL,
        0x40204000808000ULL, 0x8001008040010020ULL, 0x8410820820420010ULL, 0x1003001000090020ULL, 0x804040008008080ULL,
        0x12000810020004ULL, 0x1000100200040208ULL, 0x430000a044020001ULL, 0x280009023410300ULL, 0xe0100040002240ULL,
        0x200100401700ULL, 0x2244100408008080ULL, 0x8000400801980ULL, 0x2000810040200ULL, 0x8010100228810400ULL,
        0x2000009044210200ULL, 0x4080008040102101ULL, 0x40002080411d01ULL, 0x2005524060000901ULL, 0x502001008400422ULL,
        0x489a000810200402ULL, 0x1004400080a13ULL, 0x4000011008020084ULL, 0x26002114058042ULL
};

const U64 bishopMagics[64] = {
        0x89a1121896040240ULL, 0x2004844802002010ULL, 0x2068080051921000ULL, 0x62880a0220200808ULL, 0x4042004000000ULL,
        0x100822020200011ULL, 0xc00444222012000aULL, 0x28808801216001ULL, 0x400492088408100ULL, 0x201c401040c0084ULL,
        0x840800910a0010ULL, 0x82080240060ULL, 0x2000840504006000ULL, 0x30010c4108405004ULL, 0x1008005410080802ULL,
        0x8144042209100900ULL, 0x208081020014400ULL, 0x4800201208ca00ULL, 0xf18140408012008ULL, 0x1004002802102001ULL,
        0x841000820080811ULL, 0x40200200a42008ULL, 0x800054042000ULL, 0x88010400410c9000ULL, 0x520040470104290ULL,
        0x1004040051500081ULL, 0x2002081833080021ULL, 0x400c00c010142ULL, 0x941408200c002000ULL, 0x658810000806011ULL,
        0x188071040440a00ULL, 0x4800404002011c00ULL, 0x104442040404200ULL, 0x511080202091021ULL, 0x4022401120400ULL,
        0x80c0040400080120ULL, 0x8040010040820802ULL, 0x480810700020090ULL, 0x102008e00040242ULL, 0x809005202050100ULL,
        0x8002024220104080ULL, 0x431008804142000ULL, 0x19001802081400ULL, 0x200014208040080ULL, 0x3308082008200100ULL,
        0x41010500040c020ULL, 0x4012020c04210308ULL, 0x208220a202004080ULL, 0x111040120082000ULL, 0x6803040141280a00ULL,
        0x2101004202410000ULL, 0x8200000041108022ULL, 0x21082088000ULL, 0x2410204010040ULL, 0x40100400809000ULL,
        0x822088220820214ULL, 0x40808090012004ULL, 0x910224040218c9ULL, 0x402814422015008ULL, 0x90014004842410ULL,
        0x1000042304105ULL, 0x10008830412a00ULL, 0x2520081090008908ULL, 0x40102000a0a60140ULL,
};

const int rookIndexBits[64] = {
        12, 11, 11, 11, 11, 11, 11, 12,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        12, 11, 11, 11, 11, 11, 11, 12
};

const int bishopIndexBits[64] = {
        6, 5, 5, 5, 5, 5, 5, 6,
        5, 5, 5, 5, 5, 5, 5, 5,
        5, 5, 7, 7, 7, 7, 5, 5,
        5, 5, 7, 9, 9, 7, 5, 5,
        5, 5, 7, 9, 9, 7, 5, 5,
        5, 5, 7, 7, 7, 7, 5, 5,
        5, 5, 5, 5, 5, 5, 5, 5,
        6, 5, 5, 5, 5, 5, 5, 6
};

U64 BISHOP_MASKS[64] = {};
U64 ROOK_MASKS[64] = {};
U64 rookAttacks[64][4096] = {{0}};
U64 bishopAttacks[64][1024] = {{0}};


U64 getBlockersFromIndex(int index, U64 mask) {
    U64 blockers = 0UL;
    int bits = getCardinality(mask);
    for(int i = 0; i < bits; i++) {
        int bitPos = bitScanForward(mask);
        //int bitPos = __builtin_ffsll(mask) - 1;
        mask &= mask - 1;
        if(index & (1 << i)) {
            blockers |= (1UL << bitPos);
        }
    }
    return blockers;
}

bool bitboardsInitialized = false;

void initializeMagicBitboards() {

    if(bitboardsInitialized) {
        return;
    }
    bitboardsInitialized = true;

    // Initialize masks
    const static std::vector<bitShiftFunction> bishopDirections = {
            northEast, northWest, southEast, southWest
    };
    const static std::vector<bitShiftFunction> rookDirections = {
            northOne, eastOne, westOne, southOne
    };

    const U64 aFile = ~0xFEFEFEFEFEFEFEFE;
    const U64 hFile = ~0x7F7F7F7F7F7F7F7F;
    const U64 rank_one = ~0xFFFFFFFFFFFFFF00;
    const U64 rank_eight = ~0xFFFFFFFFFFFFFF;
    const U64 noEdges = ~0xFF818181818181FF;
    const U64 noCorners = 0x7EFFFFFFFFFFFF7E;

    for(int i = 0; i < 64; i++) {
        // Initialize bishops
        U64 bishopMask = getSlidingTargets(1UL << i, 1UL << i, 0UL, bishopDirections);
        bishopMask &= noEdges;
        BISHOP_MASKS[i] = bishopMask;

        // Initialize rooks
        U64 rookMask = getSlidingTargets(1UL << i, 1UL << i, 0UL, rookDirections);

        U64 localNoEdges = noEdges;
        if((1UL << i) & aFile) {
            localNoEdges |= aFile;
        }
        if((1UL << i) & hFile) {
            localNoEdges |= hFile;
        }
        if((1UL << i) & rank_one) {
            localNoEdges |= rank_one;
        }
        if((1UL << i) & rank_eight) {
            localNoEdges |= rank_eight;
        }

        localNoEdges &= noCorners;

        rookMask &= localNoEdges;
        ROOK_MASKS[i] = rookMask;
    }

    // Initialize rook attacks
    for(int square = 0; square < 64; square++) {
        for(int blockerIndex = 0; blockerIndex < (1 << rookIndexBits[square]); blockerIndex++) {
            U64 blockers = getBlockersFromIndex(blockerIndex, ROOK_MASKS[square]);
            U64 key = (blockers * rookMagics[square]) >> (64 - rookIndexBits[square]);
            rookAttacks[square][key] =
                    getSlidingTargets(1UL << square, 1UL << square, blockers, rookDirections);

        }
    }

    // Initialize bishop attacks
    for(int square = 0; square < 64; square++) {
        for(int blockerIndex = 0; blockerIndex < (1 << bishopIndexBits[square]); blockerIndex++) {
            U64 blockers = getBlockersFromIndex(blockerIndex, BISHOP_MASKS[square]);
            bishopAttacks[square][(blockers * bishopMagics[square]) >> (64 - bishopIndexBits[square])] =
                    getSlidingTargets(1UL << square, 1UL << square, blockers, bishopDirections);
        }
    }
}

U64 getBishopMasks(int square) {
    return BISHOP_MASKS[square];
}

U64 getRookMasks(int square) {;
    return ROOK_MASKS[square];
}

U64 getBishopAttacks(int square, U64 key) {
    return bishopAttacks[square][key];
}

U64 getRookAttacks(int square, U64 key) {
    return rookAttacks[square][key];
}

int getBishopIndexBit(int square) {
    return bishopIndexBits[square];
}

int getRookIndexBit(int square) {
    return rookIndexBits[square];
}

U64 getBishopMagic(int square) {
    return bishopMagics[square];
}
U64 getRookMagic(int square) {
    return rookMagics[square];
}