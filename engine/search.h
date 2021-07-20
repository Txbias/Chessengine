#pragma once

#include <memory>

#include "move.h"
#include "transposition_table.h"


namespace Search {

    /**
     * @brief Returns the best move found for a team
     *
     * @param fen The current state of the board as FEN string
     * @param t The Transposition table used by the board before
     * @param team The team for which a move should be found
     * @return Returns the best move found
     */
    Move getBestMove(const std::string &fen, const std::shared_ptr<TranspositionTable> & t,
                     int team);

}