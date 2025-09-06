#include "BoardModel.h"
#include <QApplication>

BoardTableModel::BoardTableModel(QObject* parent) : QAbstractTableModel(parent) {}

void BoardTableModel::bind(Board* board) { m_board = board; beginResetModel(); endResetModel(); }
int BoardTableModel::rowCount(const QModelIndex&) const { return m_board ? m_board->size() : 0; }
int BoardTableModel::columnCount(const QModelIndex&) const { return m_board ? m_board->size() : 0; }

QVariant BoardTableModel::data(const QModelIndex& idx, int role) const {
    if (!m_board || !idx.isValid()) return {};
    if (role == Qt::UserRole) return static_cast<int>(m_board->at(idx.row(), idx.column()));
    if (role == Qt::UserRole + 1) {
        const auto& line = m_board->winningLine();
        for (const auto& pt : line)
            if (pt.x() == idx.row() && pt.y() == idx.column()) return true;
        return false;
    }
    return {};
}

Qt::ItemFlags BoardTableModel::flags(const QModelIndex& idx) const {
    return idx.isValid() ? (Qt::ItemIsEnabled | Qt::ItemIsSelectable) : Qt::NoItemFlags;
}

void BoardTableModel::notifyChanged() {
    if (!m_board) return;
    emit dataChanged(index(0, 0), index(rowCount() - 1, columnCount() - 1));
    emit layoutChanged();
}

void BoardDelegate::paint(QPainter* p, const QStyleOptionViewItem& opt, const QModelIndex& idx) const {
    QStyleOptionViewItem o(opt); initStyleOption(&o, idx); o.text.clear();
    bool isWin = idx.data(Qt::UserRole + 1).toBool();
    int cell = idx.data(Qt::UserRole).toInt();

    QStyle* style = o.widget ? o.widget->style() : QApplication::style();
    style->drawControl(QStyle::CE_ItemViewItem, &o, p, o.widget);

    QRect r = o.rect.adjusted(6, 6, -6, -6);
    QPen pen = p->pen(); pen.setWidth(isWin ? 6 : 4); p->setRenderHint(QPainter::Antialiasing, true); p->setPen(pen);

    if (cell == static_cast<int>(Cell::X)) {
        p->drawLine(r.topLeft(), r.bottomRight());
        p->drawLine(r.topRight(), r.bottomLeft());
    }
    else if (cell == static_cast<int>(Cell::O)) {
        p->drawEllipse(r);
    }
}

QSize BoardDelegate::sizeHint(const QStyleOptionViewItem& opt, const QModelIndex&) const {
    return { qMax(64, opt.rect.width()), qMax(64, opt.rect.height()) };
}
