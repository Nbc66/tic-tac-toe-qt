#pragma once
#include <QAbstractTableModel>
#include <QStyledItemDelegate>
#include <QPainter>
#include "Board.h"

class BoardTableModel : public QAbstractTableModel {
    Q_OBJECT
public:
    explicit BoardTableModel(QObject* parent = nullptr);
    void bind(Board* board);

    int rowCount(const QModelIndex & = {}) const override;
    int columnCount(const QModelIndex & = {}) const override;
    QVariant data(const QModelIndex& idx, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& idx) const override;

    void notifyChanged();

private:
    Board* m_board = nullptr;
};

class BoardDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    using QStyledItemDelegate::QStyledItemDelegate;
    void paint(QPainter* p, const QStyleOptionViewItem& opt, const QModelIndex& idx) const override;
    QSize sizeHint(const QStyleOptionViewItem& opt, const QModelIndex&) const override;
};
