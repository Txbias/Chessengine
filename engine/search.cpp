#include "search.h"

#include <vector>
#include <future>

#include "board.h"
#include "thread_pool.h"
#include "utils.h"

#define WHITE 0
#define BLACK 1

#define CHECKMATE_SCORE INT32_MAX
#define PENALTY_BAD_DRAW -5000

int quiesce(Board &board, int alpha, int beta, int depth) {
    if(board.checkMate(board.actingTeam)) {
        return -CHECKMATE_SCORE;
    }

    if(board.checkMate(ENEMY(board.actingTeam))) {
        return CHECKMATE_SCORE;
    }

    int standPat = board.valuePosition(board.actingTeam);

    if(standPat >= beta) {
        return beta;
    } else if(alpha < standPat) {
        alpha = standPat;
    }

    std::vector<Move> allMoves = board.getAllMoves(board.actingTeam);

    if(allMoves.empty()) {
        if(!board.isStaleMate(0) && !board.isStaleMate(1)) {
            return board.valuePosition(board.actingTeam);
        } else {
            int value = board.valuePosition(board.actingTeam);
            if(value >= 0) {
                return PENALTY_BAD_DRAW;
            } else if(value < -500) {
                return 200;
            } else {
                return 100;
            }
        }
    }

    for(Move move : allMoves) {
        if(!move.isCapture()) {
            continue;
        }

        board.executeMove(move);

        U64 positionHash = board.getPositionHash();
        Entry entry = board.transpositionTable->getEntry(positionHash);
        if(entry.hash != 0) {
            board.undoLastMove();
            if(entry.score >= beta) {
                return beta;
            }
            if(entry.score > alpha) {
                alpha = entry.score;
            }
            continue;
        }

        // Check if move was legal
        if(board.inCheck(ENEMY(board.actingTeam))) {
            board.undoLastMove();
            continue;
        }

        if(board.threeFoldRepetition || board.isStaleMate(board.actingTeam)) {
            board.undoLastMove();
            int evaluation = board.valuePosition(board.actingTeam);
            if(evaluation >= 0) {
                return PENALTY_BAD_DRAW;
            } else if(evaluation < -500) {
                return 200;
            } else {
                return 100;
            }
        }

        int score = -quiesce(board, -beta, -alpha, depth + 1);
        board.undoLastMove();

        // Save to transposition table
        entry = Entry(positionHash, getSearchDepth() + depth, score);
        board.transpositionTable->addEntry(entry);

        if(score >= beta) {
            return beta;
        }

        int BIG_DELTA = 900;
        if(move.isPromotion()) {
            BIG_DELTA += 775;
        }

        if(score < alpha - BIG_DELTA) {
            return alpha;
        }

        if(score > alpha) {
            alpha = score;
        }
    }
    return alpha;
}


int alphaBeta(Board &board, int alpha, int beta, int depthLeft, int team) {
    if(depthLeft <= 0) {
        // Only go in quiesce search if no team is in check
        if(!board.inCheck(0) && !board.inCheck(1)) {
            return quiesce(board, alpha, beta, 0);
        }
    }

    if(board.checkMate(board.actingTeam)) {
        return -CHECKMATE_SCORE;
    }

    std::vector<Move> allMoves = board.getAllMoves(board.actingTeam);

    if(allMoves.empty()) {
        int evaluation = board.valuePosition(board.actingTeam);

        if(evaluation >= 0) {
            return PENALTY_BAD_DRAW;
        } else if(evaluation < -500) {
            return 200;
        } else {
            return 100;
        }
    }

    for(const auto &move : allMoves) {

        board.executeMove(move);

        U64 positionHash = board.getPositionHash();
        Entry entry = board.transpositionTable->getEntry(positionHash);
        if(entry.hash != 0) {
            board.undoLastMove();
            if(entry.score >= beta) {
                return beta;
            }
            if(entry.score > alpha) {
                alpha = entry.score;
            }
            continue;
        }

        // Check if move was legal
        if (board.inCheck(ENEMY(board.actingTeam))) {
            board.undoLastMove();
            continue;
        }

        if(depthLeft == 1 && !move.isCapture() &&
            !board.inCheck(WHITE) && !board.inCheck(BLACK)) {
            // futility pruning
            int valueAfter = board.valuePosition(board.getTeam(move.getTo()));
            const int MARGIN = 330;
            if(valueAfter + MARGIN < alpha) {
                board.undoLastMove();
                continue;
            }

        }

        int score;
        if(board.threeFoldRepetition || board.isStaleMate(board.actingTeam)) {
            int evaluation = board.valuePosition(board.actingTeam);

            if(evaluation >= 0) {
                score = PENALTY_BAD_DRAW;
            } else if(evaluation < -500) {
                score = 200;
            } else {
                score = 100;
            }
        } else {
            score = -alphaBeta(board, -beta, -alpha, depthLeft - 1, team);
        }
        board.undoLastMove();

        // Save to transposition table
        entry = Entry(positionHash, getSearchDepth() - depthLeft, score);
        board.transpositionTable->addEntry(entry);

        if(score >= beta) {
            return beta;
        }
        if(score > alpha) {
            alpha = score;
        }
    }

    return alpha;
}

int valueMove(const std::string &fen, const Move &move,
              const std::shared_ptr<TranspositionTable> &t, int team) {
    Board board(fen);
    board.setTranspositionTable(t);
    board.executeMove(move);

    if(board.inCheck(team) || board.checkMate(team)) {
        return -CHECKMATE_SCORE;
    }

    if(board.checkMate(ENEMY(team))) {
        return CHECKMATE_SCORE;
    }

    if(board.isStaleMate(team) || board.isStaleMate(ENEMY(team))) {
        int value = board.valuePosition(team);

        if(value >= 0) {
            return PENALTY_BAD_DRAW;
        } else if(value < -500) {
            return 200;
        } else {
            return 100;
        }
    }

    int value = -alphaBeta(board, INT32_MIN / 100, INT32_MAX / 100,
                                 getSearchDepth() - 1, ENEMY(team));

    return value;
}


Move Search::getBestMove(const std::string &fen,
                         const std::shared_ptr<TranspositionTable> &t, int team) {

    Board board(fen);
    std::vector<Move> allMoves = board.getAllMoves(team);

    std::mutex moveMutex;

    Move bestMove(0, 0, 0);
    int bestMoveValue = INT32_MIN;
    std::mutex bestMoveMutex;

    std::vector<std::future<void>> futures(allMoves.size());

    const unsigned long size = allMoves.size();
    for(int i = 0; i < size; i++) {
        futures[i] = ThreadPool::getInstance().enqueue([&] {
            std::unique_lock<std::mutex> moveLock(moveMutex);
            Move move = allMoves[0];
            allMoves.erase(allMoves.begin());
            moveLock.unlock();

            int value = valueMove(fen, move, t, team);

            std::lock_guard<std::mutex> bestMoveLock(bestMoveMutex);
            if ((bestMove.getFrom() == 0 && bestMove.getTo() == 0) ||
                    value > bestMoveValue) {

                bestMove = move;
                bestMoveValue = value;
            }
        });
    }

    for(auto &future : futures) {
        future.wait();
    }

    t->updateAfterSearch();

    return bestMove;
}