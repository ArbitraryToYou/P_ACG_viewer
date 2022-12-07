#include "tabledelegate.h"

TableDelegate::TableDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{

}

TableDelegate::~TableDelegate()
{

}

void TableDelegate::paint(QPainter* painter, const QStyleOptionViewItem &opt, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter, opt, index);
}

QSize TableDelegate::sizeHint(const QStyleOptionViewItem &opt, const QModelIndex &index) const
{
    return QStyledItemDelegate::sizeHint(opt, index);
}
