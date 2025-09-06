#pragma once
#include <QObject>
#include "Board.h"
#include "Agents.h"
#include "BoardModel.h"

class GameController : public QObject {
    Q_OBJECT
public:
    explicit GameController(QObject* parent = nullptr);
    BoardTableModel* model() const;

    void setBoardSize(int n);
    void resetGame();
    void setAgent(Player p, std::unique_ptr<IAgent> agent);
    void setVsAI(bool enabled);
    bool vsAI() const;
    void setAISide(Player side);
    Player aiSide() const;


public slots:
    void humanClick(int r, int c);

signals:
    void statusMessage(const QString&);
    void currentPlayerChanged(Player);
    void gameFinished(QString resultText);
    void boardResized(int newSize);

private:
    Board m_board;
    BoardTableModel* m_model = nullptr;
    Player m_current = Player::X;
    bool m_gameOver = false;
    bool m_vsAI = true;
    Player m_aiSide = Player::O;
    std::unique_ptr<IAgent> m_agents[2];

    void setCurrentPlayer(Player p);
    void tryPlay(int r, int c);
    void maybeAiTurn();
};
