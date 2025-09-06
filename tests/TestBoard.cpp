// --- TestBoard.cpp ---
#include <QtTest/QtTest>
#include "Board.h"

class TestBoard : public QObject {
    Q_OBJECT
private slots:
    void testEmptyBoard() {
        Board b(3);
        b.recomputeWinner();
        QCOMPARE(b.winner(), Cell::Empty);
        QVERIFY(!b.isFull());
    }

    void testRowWin() {
        Board b(3);
        QVERIFY(b.set(0, 0, Cell::X));
        QVERIFY(b.set(0, 1, Cell::X));
        QVERIFY(b.set(0, 2, Cell::X));
        b.recomputeWinner();
        QCOMPARE(b.winner(), Cell::X);
    }

    void testColumnWin() {
        Board b(3);
        QVERIFY(b.set(0, 1, Cell::O));
        QVERIFY(b.set(1, 1, Cell::O));
        QVERIFY(b.set(2, 1, Cell::O));
        b.recomputeWinner();
        QCOMPARE(b.winner(), Cell::O);
    }

    void testDraw() {
        Board b(3);
        Cell fill[3][3] = {
            {Cell::X, Cell::O, Cell::X},
            {Cell::X, Cell::O, Cell::O},
            {Cell::O, Cell::X, Cell::X}
        };
        for (int r = 0; r < 3; ++r)
            for (int c = 0; c < 3; ++c)
                QVERIFY(b.set(r, c, fill[r][c]));

        b.recomputeWinner();
        QVERIFY(b.isFull());
        QCOMPARE(b.winner(), Cell::Empty);
    }
};

QTEST_MAIN(TestBoard)
#include "TestBoard.moc"