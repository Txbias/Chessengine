#pragma once

typedef unsigned long U64;

class Position {
public:

    Position(int actingTeam, unsigned int enPassantSquare, bool canCastle[2][2],
             U64 pieces[2][6]);

    bool operator<(const Position &position) const;
    bool operator==(const Position &position) const;

private:
    int actingTeam;
    unsigned int enPassantSquare;
    U64 allPieces = 0UL;
    bool canCastle[2][2];

    U64 pieces[2][6];
};