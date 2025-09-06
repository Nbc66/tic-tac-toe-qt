#include "Agents.h"
#include <limits>

// --- Random agent ---
Move RandomAgent::chooseMove(const Board& board, Player) {
    auto moves = board.availableMoves();
    if (moves.isEmpty()) return {};
    static thread_local std::mt19937 rng{ std::random_device{}() };
    std::uniform_int_distribution<> dist(0, moves.size() - 1);
    return moves[dist(rng)];
}

// --- Minimax agent ---
Move MinimaxAgent::chooseMove(const Board& board, Player self) {
    if (board.size() != 3) return m_fallback.chooseMove(board, self);

    int bestScore = std::numeric_limits<int>::min();
    Move best;

    for (auto mv : board.availableMoves()) {
        Board b = board;
        b.set(mv.r, mv.c, cellFor(self));   // set recomputes winner
        int score = minimax(b, other(self), self, false);
        if (score > bestScore) {
            bestScore = score;
            best = mv;
        }
    }

    if (!best.isValid())
        return m_fallback.chooseMove(board, self);

    return best;
}

int MinimaxAgent::minimax(Board& b, Player toMove, Player me, bool isMax, int depth) {
    if (b.winner() == cellFor(me)) return 10 - depth;
    if (b.winner() == cellFor(other(me))) return depth - 10;
    if (b.isFull()) return 0;

    int best = isMax ? std::numeric_limits<int>::min()
        : std::numeric_limits<int>::max();

    for (auto mv : b.availableMoves()) {
        b.set(mv.r, mv.c, cellFor(toMove));
        int sc = minimax(b, other(toMove), me, !isMax, depth + 1);
        b.unset(mv.r, mv.c);
        if (isMax) best = std::max(best, sc);
        else       best = std::min(best, sc);
    }
    return best;
}
