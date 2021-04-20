#include <iostream>

#include "Move.h"

int main() {
    Move move(0, 1, 8);


    std::cout << move.isCapture() << std::endl;
    return 0;
}
