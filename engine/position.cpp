#include "position.h"

Position::Position(int actingTeam, unsigned int enPassantSquare, bool canCastle[2][2],
                   U64 pieces[2][6]) {
    this->actingTeam = actingTeam;
    this->enPassantSquare = enPassantSquare;

    for(int i = 0; i < 2; i++) {
        for(int k = 0; k < 2; k++) {
            this->canCastle[i][k] = canCastle[i][k];
        }
        for(int k = 0; k < 6; k++) {
            this->pieces[i][k] = pieces[i][k];
            allPieces |= pieces[i][k];
        }
    }
}

bool Position::operator<(const Position &position) const {
    return allPieces < position.allPieces;
}

bool Position::operator==(const Position &position) const {
    if(actingTeam != position.actingTeam) {
        return false;
    }

    if(enPassantSquare != position.enPassantSquare) {
        return false;
    }

    for(int i = 0; i < 2; i++) {
        for(int k = 0; k < 2; k++) {
            if(canCastle[i][k] != position.canCastle[i][k]) {
                return false;
            }
        }
        for(int k = 0; k < 6; k++) {
            if(pieces[i][k] != position.pieces[i][k]) {
                return false;
            }
        }
    }

    return true;
}
