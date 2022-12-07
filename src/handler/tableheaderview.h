#ifndef TABLEHEADERVIEW_H
#define TABLEHEADERVIEW_H

#include <QHeaderView>

class TableHeaderView : public QHeaderView
{
    Q_OBJECT
public:
    TableHeaderView(Qt::Orientation orientation, QWidget* parent = nullptr);
    virtual ~TableHeaderView();
};

#endif // TABLEHEADERVIEW_H
