#include "tableview.h"
#include "tableheaderview.h"

TableView::TableView(QWidget* parent)
    : QTableView(parent)
{
    m_horizontalHeader = new TableHeaderView(Qt::Horizontal, this);
    m_verticalHeader = new TableHeaderView(Qt::Vertical, this);
    setHorizontalHeader(m_horizontalHeader);
    setVerticalHeader(m_verticalHeader);
}

TableView::~TableView()
{

}
