#pragma once

#include <iostream>
#include <string>

#define PAWN 0
#define ROOK 1
#define KNIGHT 2
#define BISHOP 3
#define QUEEN 4
#define KING 5

#define FLAG_QUIET 0
#define FLAG_PAWN_DBL_PUSH 1
#define FLAG_KING_CASTLE 2
#define FLAG_QUEEN_CASTLE 3
#define FLAG_CAPTURE 4
#define FLAG_QUEEN_PROMOTION 11
#define FLAG_QUEEN_PROMOTION_CAPTURE 15

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

    unsigned int getTeam() const {
        return team;
    }

    unsigned int getCapturedPiece() {
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

    void setCapturedPiece(unsigned int captured) {
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
            //TODO: for all types of promotions
            result += "q";
        }

        return result;
    }

    static Move fromNotation(const std::string& notation) {
        //TODO: flag
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
            } else {
                std::cerr << "Unknown promotion piece: " << notation.at(4) << std::endl;
            }

        }

        return Move(from, to, flag);
    }

private:
    unsigned int m_move;
    unsigned int captured;
    unsigned int epSquareBefore;
    unsigned int team;
    bool initialMoveKing = false;
    bool initialMoveRook = false;

    const static unsigned int CAPTURE_FLAG = (1Ul << 14);

};
