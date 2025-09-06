#include <QtTest/QtTest>
#include "GameController.h"

class TestGameController : public QObject {
    Q_OBJECT
private slots:
    void testInitialState() {
        GameController ctrl;
        QVERIFY(ctrl.model()->rowCount() == 3);
        QVERIFY(ctrl.model()->columnCount() == 3);
    }

    void testResetGameClearsBoard() {
        GameController ctrl;
        ctrl.humanClick(0, 0);
        ctrl.resetGame();
        for (int r = 0; r < 3; ++r)
            for (int c = 0; c < 3; ++c)
                QCOMPARE(ctrl.model()->index(r, c).data(Qt::UserRole).toInt(), (int)Cell::Empty);
    }

    void testHumanVsHumanTurnOrder() {
        GameController ctrl;
        ctrl.setVsAI(false);

        ctrl.humanClick(0, 0); // X
        QCOMPARE(ctrl.model()->index(0, 0).data(Qt::UserRole).toInt(), (int)Cell::X);

        ctrl.humanClick(1, 1); // O
        QCOMPARE(ctrl.model()->index(1, 1).data(Qt::UserRole).toInt(), (int)Cell::O);
    }
};

QTEST_MAIN(TestGameController)
#include "TestGameController.moc"
