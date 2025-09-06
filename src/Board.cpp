#include "Board.h"
#include <algorithm>

Board::Board(int n) { setSize(n); }

void Board::setSize(int n) {
    if (n < 3) n = 3;
    m_n = n;
    m_cells.assign(m_n * m_n, Cell::Empty);
    m_winningLine.clear();
}

int Board::size() const { return m_n; }

void Board::reset() {
    std::fill(m_cells.begin(), m_cells.end(), Cell::Empty);
    m_winningLine.clear();
}

bool Board::set(int r, int c, Cell v) {
    if (!inBounds(r, c)) return false;
    if (m_cells[index(r, c)] != Cell::Empty) return false;
    m_cells[index(r, c)] = v;
    recomputeWinner();
    return true;
}

bool Board::unset(int r, int c)
{
    if (!inBounds(r, c)) return false;
    m_cells[index(r, c)] = Cell::Empty;
    m_cachedWinner = Cell::Empty; // force recompute on next call
    recomputeWinner();
    return true;
}

bool Board::isFull() const {
    for (auto& v : m_cells) if (v == Cell::Empty) return false;
    return true;
}

QVector<Move> Board::availableMoves() const {
    QVector<Move> m;
    for (int r = 0; r < m_n; ++r)
        for (int c = 0; c < m_n; ++c)
            if (at(r, c) == Cell::Empty) m.push_back({ r, c });
    return m;
}

Cell Board::winner() const { return m_cachedWinner; }
const QVector<QPoint>& Board::winningLine() const { return m_winningLine; }

void Board::recomputeWinner() {
    m_winningLine.clear();
    m_cachedWinner = Cell::Empty;

    // Rows
    for (int r = 0; r < m_n; ++r) {
        Cell first = at(r, 0);
        if (first == Cell::Empty) continue;
        bool all = true;
        for (int c = 1; c < m_n; ++c) if (at(r, c) != first) { all = false; break; }
        if (all) {
            for (int c = 0; c < m_n; ++c) m_winningLine.push_back(QPoint(r, c));
            m_cachedWinner = first; return;
        }
    }
    // Cols
    for (int c = 0; c < m_n; ++c) {
        Cell first = at(0, c);
        if (first == Cell::Empty) continue;
        bool all = true;
        for (int r = 1; r < m_n; ++r) if (at(r, c) != first) { all = false; break; }
        if (all) {
            for (int r = 0; r < m_n; ++r) m_winningLine.push_back(QPoint(r, c));
            m_cachedWinner = first; return;
        }
    }
    // Diagonal TL-BR
    {
        Cell first = at(0, 0);
        if (first != Cell::Empty) {
            bool all = true;
            for (int i = 1; i < m_n; ++i) if (at(i, i) != first) { all = false; break; }
            if (all) {
                for (int i = 0; i < m_n; ++i) m_winningLine.push_back(QPoint(i, i));
                m_cachedWinner = first; return;
            }
        }
    }
    // Diagonal TR-BL
    {
        Cell first = at(0, m_n - 1);
        if (first != Cell::Empty) {
            bool all = true;
            for (int i = 1; i < m_n; ++i) if (at(i, m_n - 1 - i) != first) { all = false; break; }
            if (all) {
                for (int i = 0; i < m_n; ++i) m_winningLine.push_back(QPoint(i, m_n - 1 - i));
                m_cachedWinner = first; return;
            }
        }
    }
    m_cachedWinner = Cell::Empty;
}

bool Board::inBounds(int r, int c) const { return r >= 0 && c >= 0 && r < m_n && c < m_n; }
int Board::index(int r, int c) const { return r * m_n + c; }
