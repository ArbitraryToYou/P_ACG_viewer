#ifndef TABLEDELEGATE_H
#define TABLEDELEGATE_H

#include <QStyledItemDelegate>
#include <QObject>

class TableDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit TableDelegate(QObject *parent = nullptr);
    virtual ~TableDelegate();

    virtual void paint(QPainter* painter, const QStyleOptionViewItem &opt, const QModelIndex &index) const override;
    virtual QSize sizeHint(const QStyleOptionViewItem &opt, const QModelIndex &index) const override;
};

#endif // TABLEDELEGATE_H
