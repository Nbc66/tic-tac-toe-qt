#pragma once
#include "Board.h"
#include <memory>
#include <random>

class IAgent {
public:
    virtual ~IAgent() = default;
    virtual Move chooseMove(const Board& board, Player self) = 0;
};

class RandomAgent : public IAgent {
public:
    Move chooseMove(const Board& board, Player) override;
};

class MinimaxAgent : public IAgent {
public:
    Move chooseMove(const Board& board, Player self) override;
private:
    int minimax(Board& b, Player toMove, Player me, bool isMax, int depth = 0);
    void undo(Board& b, const Move& mv);
    RandomAgent m_fallback;
};
