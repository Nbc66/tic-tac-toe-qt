#include "GameController.h"
#include <QTimer>

GameController::GameController(QObject* parent)
    : QObject(parent), m_board(3) {
    m_model = new BoardTableModel(this);
    m_model->bind(&m_board);
    m_agents[0] = std::make_unique<RandomAgent>();
    m_agents[1] = std::make_unique<MinimaxAgent>();
    setCurrentPlayer(Player::X);
}

BoardTableModel* GameController::model() const { return m_model; }

void GameController::setBoardSize(int n) {
    bool wasAI = m_vsAI;
    m_board.setSize(n);
    m_board.reset();
    setCurrentPlayer(Player::X);
    m_model->notifyChanged();
    emit boardResized(n);
    setVsAI(wasAI);
}

void GameController::resetGame() {
    m_board.reset();
    m_gameOver = false;
    setCurrentPlayer(Player::X);
    m_model->notifyChanged();
    emit statusMessage("New game. X to move.");
    // If AI is X, let it move immediately
    maybeAiTurn();
}

void GameController::setAgent(Player p, std::unique_ptr<IAgent> agent) {
    m_agents[p == Player::X ? 0 : 1] = std::move(agent);
}

void GameController::setVsAI(bool enabled) {
    m_vsAI = enabled;
    emit statusMessage(enabled ? "Human vs AI" : "Human vs Human");
    maybeAiTurn();
}

bool GameController::vsAI() const { return m_vsAI; }

void GameController::setAISide(Player side)
{
    m_aiSide = side;
}

Player GameController::aiSide() const
{
    return m_aiSide;
}

void GameController::humanClick(int r, int c) {
    if (m_gameOver) return;
    if (m_vsAI && m_current == m_aiSide) return;
    tryPlay(r, c);
    maybeAiTurn();
}

void GameController::setCurrentPlayer(Player p) {
    m_current = p;
    emit currentPlayerChanged(p);
    emit statusMessage(QString("%1 to move").arg(p == Player::X ? "X" : "O"));
}

void GameController::tryPlay(int r, int c) {
    if (m_gameOver) return;
    if (!m_board.set(r, c, cellFor(m_current))) return;
    m_board.recomputeWinner();
    m_model->notifyChanged();

    if (m_board.winner() != Cell::Empty) {
        m_gameOver = true;
        QString w = m_board.winner() == Cell::X ? "X" : "O";
        emit gameFinished(w + " wins!");
        emit statusMessage(w + " wins!");
        return;
    }
    if (m_board.isFull()) {
        m_gameOver = true;
        emit gameFinished("Draw");
        emit statusMessage("Draw");
        return;
    }
    setCurrentPlayer(other(m_current));
}

void GameController::maybeAiTurn() {
    if (!m_vsAI || m_gameOver) return;
    if (m_current != m_aiSide) return;
    auto& agent = m_agents[m_aiSide == Player::X ? 0 : 1];
    if (!agent) return;

    QTimer::singleShot(200, this, [this, &agent] {
        Move mv = agent->chooseMove(m_board, m_aiSide);
        if (mv.isValid()) tryPlay(mv.r, mv.c);
        });
}
