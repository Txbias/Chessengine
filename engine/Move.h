#pragma once

#include <iostream>

#define PAWN 0
#define ROOK 1
#define KNIGHT 2
#define BISHOP 3
#define QUEEN 4
#define KING 5

class Move {
public:
    Move(unsigned int from, unsigned int to, unsigned int flags) {
        m_move = ((flags & 0xf) << 12) | ((from & 0x3f) << 6) | (to & 0x3f);
        captured = 0;
    }
    friend std::ostream& operator<<(std::ostream& os, const Move& move) {
        os << toNotation(move.getFrom()) << " " << toNotation(move.getTo())
            << " " << move.getFlags() << std::endl;

        os << move.getFrom() << " " << move.getTo()
           << " " << move.getFlags();

        return os;
    }

    unsigned int getTo() const {
        return m_move & 0x3f;
    }

    unsigned int getFrom() const {
        return (m_move >> 6) & 0x3f;
    }

    unsigned int getFlags() const {
        return (m_move >> 12) & 0x0f;
    }

    unsigned int getCapturedPiece() {
        return captured;
    }

    bool isCapture() const {
        return (m_move & CAPTURE_FLAG) != 0;
    }

    void setCapturedPiece(unsigned int captured) {
        this->captured = captured;
    }

    static std::string toNotation(unsigned int position) {
        unsigned int row = position / 8 + 1;
        unsigned int fileInt = position  % 8;

        char file = fileInt + 97;

        return file + std::to_string(row);
    }

private:
    unsigned int m_move;
    unsigned int captured;

    const static unsigned int CAPTURE_FLAG = (1Ul << 14);

};