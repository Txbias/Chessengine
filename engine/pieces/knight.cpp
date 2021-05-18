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

U64 Knight::noNoEa(U64 b) {return (b << 17) & notAFile ;}
U64 Knight::noEaEa(U64 b) {return (b << 10) & notABFile;}
U64 Knight::soEaEa(U64 b) {return (b >>  6) & notABFile;}
U64 Knight::soSoEa(U64 b) {return (b >> 15) & notAFile ;}
U64 Knight::noNoWe(U64 b) {return (b << 15) & notHFile ;}
U64 Knight::noWeWe(U64 b) {return (b <<  6) & notGHFile;}
U64 Knight::soWeWe(U64 b) {return (b >> 10) & notGHFile;}
U64 Knight::soSoWe(U64 b) {return (b >> 17) & notHFile ;}


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

std::string getBits(U64 bitboard) {
    std::string res;
    for(int i = 0; i < 64; i++) {
        if(bitboard & 1UL << i) {
            res += "1";
        } else {
            res += "0";
        }
    }
    return res;
}

U64 Knight::getOriginKnights(U64 knights, U64 target, U64 ownPieces) {
    // ownPieces has to be inverted
    U64 possibleFields = getTargets(target, ~ownPieces);
    return possibleFields & knights;
}


std::vector<Move> Knight::getMoves(U64 knights, U64 enemyPieces, U64 ownPieces) {
    std::vector<Move> moves;

    U64 knightTargets = getTargets(knights, ownPieces);

    for(int i = 0; i < 64; i++) {
        if(knightTargets & (1UL << i)) {
            int flag = FLAG_QUIET;
            if(enemyPieces & (1UL << i)) {
                // Enemy piece at target position
                flag = FLAG_CAPTURE;
            }

            U64 originKnights = getOriginKnights(knights, 1UL << i, ownPieces);

            for(int k = 0; k < 64; k++) {
                if(originKnights & (1UL << k)) {
                    Move move(k, i, flag);
                    moves.emplace_back(move);
                }
            }

        }
    }

    return moves;
}