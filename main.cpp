#include <iostream>

#include "board.h"

int main() {

    Board board;

    std::vector<Move> moves = board.getAllMoves(0);

    for(Move move : moves) {
        std::cout << move << std::endl;
        if(move.isCapture()) {
            std::cout << "capture" << std::endl;
        }
        std::cout << std::endl;
    }

    std::cout << "Amount moves: " << moves.size() << std::endl;

    return 0;
}
