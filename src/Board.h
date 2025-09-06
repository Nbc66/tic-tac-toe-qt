#pragma once
#include <QVector>
#include <QPoint>
#include <vector>

enum class Cell { Empty, X, O };
enum class Player { X, O };

inline Player other(Player p) { return p == Player::X ? Player::O : Player::X; }
inline Cell cellFor(Player p) { return p == Player::X ? Cell::X : Cell::O; }

struct Move {
    int r = -1;
    int c = -1;
    bool isValid() const { return r >= 0 && c >= 0; }
};

class Board {
public:
    explicit Board(int n = 3);

    void setSize(int n);
    int size() const;
    void reset();

    bool set(int r, int c, Cell v);
    bool unset(int r, int c);

    bool isFull() const;

    QVector<Move> availableMoves() const;
    Cell winner() const;
    const QVector<QPoint>& winningLine() const;

    void recomputeWinner();

    Cell cell(int r, int c) const {
        return m_cells[index(r, c)];
    }

    Cell at(int r, int c) const {
        return m_cells[index(r, c)];
    }

private:
    int m_n;
    std::vector<Cell> m_cells;
    Cell m_cachedWinner;
    QVector<QPoint> m_winningLine;

    bool inBounds(int r, int c) const;
    int index(int r, int c) const;
};
