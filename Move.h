#pragma once

#include <iostream>

class Move {
public:
    Move(unsigned int from, unsigned int to, unsigned int flags) {
        m_move = ((flags & 0xf) << 12) | ((from & 0x3f) << 6) | (to & 0x3f);
    }
    friend std::ostream& operator<<(std::ostream& os, const Move& move) {
        os << move.getFrom() << " " << move.getTo() << " " << move.getFlags();
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

    bool isCapture() const {
        return (m_move & CAPTURE_FLAG) != 0;
    }

private:
    unsigned int m_move;
    const static unsigned int CAPTURE_FLAG = (1Ul << 14);

};
