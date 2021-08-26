#pragma once

#include <iostream>
#include <string>

#include "constants.h"

class Move {
public:
    Move(unsigned int from, unsigned int to, unsigned int flags) {
        m_move = ((flags & 0xf) << 12) | ((from & 0x3f) << 6) | (to & 0x3f);
        captured = nullptr;
    }

    explicit Move() = default;

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

    unsigned int getTeam() const {
        return team;
    }

    unsigned long* getCapturedPiece() {
        return captured;
    }

    bool isCapture() const {
        return (m_move & CAPTURE_FLAG) != 0;
    }

    bool isPromotion() const {
        return getFlags() & (1U << 3);
    }

    bool isCastle() const {
        return getFlags() == FLAG_QUEEN_CASTLE || getFlags() == FLAG_KING_CASTLE;
    }

    bool isInitialMoveKing() const{
        return initialMoveKing;
    }

    bool isInitialMoveRook() const{
        return initialMoveRook;
    }

    unsigned int getEPSquareBefore() {
        return epSquareBefore;
    }

    void setFlags(unsigned int flags) {
        m_move = ((flags & 0xf) << 12) | ((getFrom() & 0x3f) << 6) | (getTo() & 0x3f);
    }

    void setCapturedPiece(unsigned long* captured) {
        this->captured = captured;
    }

    void setTeam(unsigned int team) {
        this->team = team;
    }

    void setInitialMoveKing() {
        initialMoveKing = true;
    }

    void setInitialMoveRook() {
        initialMoveRook = true;
    }

    void setEPSquareBefore(unsigned int square) {
        epSquareBefore = square;
    }

    static std::string toNotation(unsigned int position) {
        unsigned int row = position / 8 + 1;
        unsigned int fileInt = position  % 8;

        char file = fileInt + 97;

        return file + std::to_string(row);
    }

    static std::string toNotation(Move move) {
        std::string result;

        result += toNotation(move.getFrom());
        result += toNotation(move.getTo());

        if(move.isPromotion()) {

            if(move.getFlags() == FLAG_KNIGHT_PROMOTION ||
                move.getFlags() == FLAG_KNIGHT_PROMOTION_CAPTURE) {
                result += 'n';
            } else if(move.getFlags() == FLAG_BISHOP_PROMOTION ||
                move.getFlags() == FLAG_BISHOP_PROMOTION_CAPTURE) {
                result += 'b';
            } else if(move.getFlags() == FLAG_ROOK_PROMOTION ||
                move.getFlags() == FLAG_ROOK_PROMOTION_CAPTURE) {
                result += 'r';
            } else {
                result += 'q';
            }
        }

        return result;
    }

    static Move fromNotation(const std::string& notation) {
        int x = (notation.at(0) - 97);
        int y = std::stoi(notation.substr(1, 2));
        unsigned int from = ((y-1) * 8) + x;

        x = (notation.at(2) - 97);
        y = std::stoi(notation.substr(3));
        unsigned int to = ((y-1) * 8) + x;

        unsigned int flag = FLAG_QUIET;

        if(notation.length() == 5) {
            // Promotion
            if(notation.at(4) == 'q') {
                flag = FLAG_QUEEN_PROMOTION;
            } else if(notation.at(4) == 'r') {
                flag = FLAG_ROOK_PROMOTION;
            } else if(notation.at(4) == 'n') {
                flag = FLAG_KNIGHT_PROMOTION;
            } else if(notation.at(4) == 'b') {
                flag = FLAG_BISHOP_PROMOTION;
            }
        }

        return Move(from, to, flag);
    }

private:
    unsigned int m_move;
    unsigned long* captured;
    unsigned int epSquareBefore;
    unsigned int team;
    bool initialMoveKing = false;
    bool initialMoveRook = false;

    const static unsigned int CAPTURE_FLAG = (1Ul << 14);

};
