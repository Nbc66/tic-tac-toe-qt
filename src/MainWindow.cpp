#include "MainWindow.h"
#include "BoardModel.h"
#include <QToolBar>
#include <QMessageBox>
#include <QInputDialog>
#include <QStatusBar>
#include <QHBoxLayout>
#include <QLabel>
#include <QHeaderView> 

MainWindow::MainWindow() {
    setWindowTitle("Qt Tic-Tac-Toe");
    resize(520, 560);

    m_ctrl = new GameController(this);

    m_view = new QTableView(this);
    m_view->setModel(m_ctrl->model());
    m_view->setItemDelegate(new BoardDelegate(this));
    m_view->horizontalHeader()->hide();
    m_view->verticalHeader()->hide();
    m_view->setSelectionMode(QAbstractItemView::NoSelection);
    m_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    setCentralWidget(m_view);

    auto* tb = addToolBar("Game");
    auto* actNew = tb->addAction("New Game");
    auto* actSize = tb->addAction("Board Size");
    m_aiToggle = new QAction("Human vs AI", this);
    m_aiToggle->setCheckable(true); m_aiToggle->setChecked(true);
    tb->addAction(m_aiToggle);
    m_aiSideCombo = new QComboBox(this);
    m_aiSideCombo->addItems({ "AI = O (Human starts)", "AI = X" });
    QWidget* w = new QWidget(this);
    auto* lay = new QHBoxLayout(w); lay->setContentsMargins(0, 0, 0, 0);
    lay->addWidget(new QLabel("Mode:")); lay->addWidget(m_aiSideCombo);
    tb->addWidget(w);

    statusBar();

    connect(actNew, &QAction::triggered, m_ctrl, &GameController::resetGame);
    connect(actSize, &QAction::triggered, this, &MainWindow::changeBoardSize);
    connect(m_aiToggle, &QAction::toggled, m_ctrl, &GameController::setVsAI);
    connect(m_aiSideCombo, &QComboBox::currentIndexChanged, this, &MainWindow::changeAISide);

    connect(m_ctrl, &GameController::statusMessage, this, &MainWindow::showStatus);
    connect(m_ctrl, &GameController::gameFinished, this, &MainWindow::announce);
    connect(m_ctrl, &GameController::boardResized, this, &MainWindow::resizeCells);

    connect(m_view, &QTableView::clicked, this, [this](const QModelIndex& idx) {
        m_ctrl->humanClick(idx.row(), idx.column());
        });

    resizeCells(m_ctrl->model()->rowCount());
    m_ctrl->resetGame();
}

void MainWindow::showStatus(const QString& s) { statusBar()->showMessage(s, 3000); }

void MainWindow::announce(const QString& msg) {
    QMessageBox::information(this, "Game Over", msg);
}

void MainWindow::changeBoardSize() {
    bool ok = false;
    int n = QInputDialog::getInt(this, "Board size", "N (>=3):", 3, 3, 10, 1, &ok);
    if (ok) m_ctrl->setBoardSize(n);
}

void MainWindow::changeAISide(int idx) {
    if (idx == 0) {
        m_ctrl->setAgent(Player::X, std::make_unique<RandomAgent>());
        m_ctrl->setAgent(Player::O, std::make_unique<MinimaxAgent>());
    }
    else {
        m_ctrl->setAgent(Player::X, std::make_unique<MinimaxAgent>());
        m_ctrl->setAgent(Player::O, std::make_unique<RandomAgent>());
    }
    m_ctrl->resetGame();
}

void MainWindow::resizeCells(int n) {
    int side = qMin(m_view->viewport()->width(), m_view->viewport()->height());
    int cell = qMax(48, side / n);
    m_view->horizontalHeader()->setDefaultSectionSize(cell);
    m_view->verticalHeader()->setDefaultSectionSize(cell);
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);

    if (!m_view || !m_view->model()) return;

    int n = m_view->model()->rowCount();
    if (n <= 0) return;

    // Calculate new cell size based on available viewport size
    int side = qMin(m_view->viewport()->width(), m_view->viewport()->height());
    int cell = qMax(48, side / n);

    m_view->horizontalHeader()->setDefaultSectionSize(cell);
    m_view->verticalHeader()->setDefaultSectionSize(cell);
}

void MainWindow::showEvent(QShowEvent* event)
{
    QMainWindow::showEvent(event);
    resizeCells(m_view->model()->rowCount());
}
