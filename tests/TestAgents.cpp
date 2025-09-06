// --- TestAgents.cpp ---
#include <QtTest/QtTest>
#include "Agents.h"
#include "Board.h"

class TestAgents : public QObject {
    Q_OBJECT
private slots:
    void testRandomAgentChoosesValidMove() {
        Board b(3);
        RandomAgent agent;
        Move mv = agent.chooseMove(b, Player::X);
        QVERIFY(mv.isValid());
        QVERIFY(b.cell(mv.r, mv.c) == Cell::Empty);
    }

    void testMinimaxAgentBlocksOpponent() {
        Board b(3);
        b.set(0, 0, Cell::O);
        b.set(0, 1, Cell::O);

        MinimaxAgent agent;
        Move mv = agent.chooseMove(b, Player::X);
        QVERIFY(mv.isValid());

        // Ensure opponent is no longer winning after this move
        b.set(mv.r, mv.c, Cell::X);
        b.recomputeWinner();
        QVERIFY(b.winner() != Cell::O);
    }

    void testMinimaxAgentWinsIfPossible() {
        Board b(3);
        b.set(1, 0, Cell::X);
        b.set(1, 1, Cell::X);

        MinimaxAgent agent;
        Move mv = agent.chooseMove(b, Player::X);

        QVERIFY(mv.isValid());

        qDebug() << "Minimax chose row=" << mv.r << " col=" << mv.c;

        // Apply move
        bool ok = b.set(mv.r, mv.c, Cell::X);
        QVERIFY(ok);

        b.recomputeWinner();
        qDebug() << "After move, winner:" << static_cast<int>(b.winner());

        QCOMPARE(b.winner(), Cell::X);
    }

};

QTEST_MAIN(TestAgents)
#include "TestAgents.moc"
