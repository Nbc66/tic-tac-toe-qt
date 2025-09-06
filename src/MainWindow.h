#pragma once
#include <QMainWindow>
#include <QTableView>
#include <QComboBox>
#include "GameController.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow();

private slots:
    void showStatus(const QString& s);
    void announce(const QString& msg);
    void changeBoardSize();
    void changeAISide(int idx);
    void resizeCells(int n);

private:
    QTableView* m_view = nullptr;
    GameController* m_ctrl = nullptr;
    QAction* m_aiToggle = nullptr;
    QComboBox* m_aiSideCombo = nullptr;
protected:
    void resizeEvent(QResizeEvent* event) override;
    void showEvent(QShowEvent* event) override;
};
