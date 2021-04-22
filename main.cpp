#include <iostream>

#include "board.h"

int main() {

    Board board;

    std::vector<Move> moves = board.getAllMoves(0);

    for(Move move : moves) {
        std::cout << move << std::endl;
    }

    return 0;
}
