#include <iostream>
#include <sstream>
#include <string>

#include "board.h"
#include "logger.h"
#include "utils.h"


template <typename Out>
void split(const std::string &s, char delim, Out result) {
    std::istringstream iss(s);
    std::string item;
    while (std::getline(iss, item, delim)) {
        *result++ = item;
    }
}

int main() {

    Board board;
    initializeMagicBitboards();

    bool hasMoved = false;
    int team = 0;

    while(true) {
        std::string line;
        std::getline(std::cin, line);

        Logger::debug(line);
        std::vector<std::string> commands;
        split(line, ' ', std::back_inserter(commands));

        if(commands[0] == "xboard") {
            std::cout << std::endl;
        } else if(commands[0] == "protover") {
            std::cout << "feature usermove=1 done=1" << std::endl;
        } else if(commands[0] == "usermove") {
            std::string move = commands[1];

            if(!hasMoved) {
                team = 1;
            }

            board.executeUserMove(Move::fromNotation(move));

            Logger::debug(board.getBoardPrintable());

            Move bestMove = board.getBestMove(team);
            std::cout << "move " << Move::toNotation(bestMove) << std::endl;

            board.executeMove(bestMove);
            std::cout << "Value white: " << board.valuePosition(0) << std::endl;
            std::cout << "Value black: " << board.valuePosition(1) << std::endl;
            board.printBoard();

            Logger::debug(board.getBoardPrintable());
            hasMoved = true;
        } else if(commands[0] == "quit") {
            break;
        } else if(commands[0] == "go") {
            if(hasMoved) {
                continue;
            }

            Move bestMove = board.getBestMove(0);
            std::cout << "move " << Move::toNotation(bestMove) << std::endl;
            board.executeMove(bestMove);

            std::cout << "Value white: " << board.valuePosition(0) << std::endl;
            std::cout << "Value black: " << board.valuePosition(1) << std::endl;
            board.printBoard();

            Logger::debug(board.getBoardPrintable());
            hasMoved = true;
        }
    }

    return 0;
}
