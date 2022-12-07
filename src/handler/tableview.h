#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QTableView>

class TableHeaderView;
class TableView : public QTableView
{
    Q_OBJECT
public:
    explicit TableView(QWidget* parent = nullptr);
    virtual ~TableView();

private:
    TableHeaderView* m_horizontalHeader{nullptr};
    TableHeaderView* m_verticalHeader{nullptr};
};

#endif // TABLEVIEW_H
