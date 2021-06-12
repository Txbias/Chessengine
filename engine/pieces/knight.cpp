#include "knight.h"

typedef unsigned long U64;

std::array<int, 64> Knight::pieceSquareTable() {
    constexpr const std::array<int, 64> table = {
            -50,-40,-30,-30,-30,-30,-40,-50,
            -40,-20,  0,  0,  0,  0,-20,-40,
            -30,  0, 10, 15, 15, 10,  0,-30,
            -30,  5, 15, 20, 20, 15,  5,-30,
            -30,  0, 15, 20, 20, 15,  0,-30,
            -30,  5, 10, 15, 15, 10,  5,-30,
            -40,-20,  0,  5,  5,  0,-20,-40,
            -50,-40,-30,-30,-30,-30,-40,-50,
    };

    return table;
}

constexpr U64 Knight::noNoEa(const U64 b) {return (b << 17) & notAFile ;}
constexpr U64 Knight::noEaEa(const U64 b) {return (b << 10) & notABFile;}
constexpr U64 Knight::soEaEa(const U64 b) {return (b >>  6) & notABFile;}
constexpr U64 Knight::soSoEa(const U64 b) {return (b >> 15) & notAFile ;}
constexpr U64 Knight::noNoWe(const U64 b) {return (b << 15) & notHFile ;}
constexpr U64 Knight::noWeWe(const U64 b) {return (b <<  6) & notGHFile;}
constexpr U64 Knight::soWeWe(const U64 b) {return (b >> 10) & notGHFile;}
constexpr U64 Knight::soSoWe(const U64 b) {return (b >> 17) & notHFile ;}


U64 Knight::getTargets(U64 knights, U64 ownPieces) {
    U64 targets = noNoEa(knights);
    targets |= noEaEa(knights);
    targets |= soEaEa(knights);
    targets |= soSoEa(knights);
    targets |= noNoWe(knights);
    targets |= noWeWe(knights);
    targets |= soWeWe(knights);
    targets |= soSoWe(knights);

    return targets & (~ownPieces);
}

U64 Knight::getOriginKnights(U64 knights, U64 target, U64 ownPieces) {
    // ownPieces has to be inverted
    U64 possibleFields = getTargets(target, ~ownPieces);
    return possibleFields & knights;
}


std::vector<Move> Knight::getMoves(U64 knights, U64 enemyPieces, U64 ownPieces) {
    std::vector<Move> moves;

    U64 knightTargets = getTargets(knights, ownPieces);

    std::vector<int> setBits = getSetBits(knightTargets);

    for(int i : setBits) {
        int flag = FLAG_QUIET;
        if(enemyPieces & (1UL << i)) {
            // Enemy piece at target position
            flag = FLAG_CAPTURE;
        }

        U64 originKnights = getOriginKnights(knights, 1UL << i, ownPieces);

        std::vector<int> originSetBits = getSetBits(originKnights);

        for(int k : originSetBits) {
            Move move(k, i, flag);
            moves.emplace_back(move);
        }
    }

    return moves;
}