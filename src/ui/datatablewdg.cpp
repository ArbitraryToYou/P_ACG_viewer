#include "datatablewdg.h"
#include "ui_datatablewdg.h"
#include "utility/defs.h"
#include <QSqlDatabase>
#include <QStandardItem>
#include <QHeaderView>

// id, title, type, date, links[, names]

DataTableWdg::DataTableWdg(const QString& tableName, QWidget *parent) :
    QWidget(parent),
    m_tbName(tableName),
    ui(new Ui::DataTableWdg)
{
    ui->setupUi(this);

    QSqlDatabase db = QSqlDatabase::database(SQLCONN_NAME);
    if (db.isValid())
    {
        m_model = new TableModel(this);
        m_selectionModel = new QItemSelectionModel(m_model);
        ui->tableView->setModel(m_model);
        ui->tableView->setSelectionModel(m_selectionModel);
        ui->tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
        ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);

        ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
        ui->tableView->setMouseTracking(true);
    }else
    {
        DEBUG_MSG("The database is invalid.");
    }


    connect(ui->tableView, SIGNAL(clicked(const QModelIndex&)), SLOT(onViewClicked(const QModelIndex&)));
    connect(ui->tableView, SIGNAL(entered(const QModelIndex&)), SLOT(onViewEntered(const QModelIndex&)));
}

DataTableWdg::~DataTableWdg()
{
    m_pImgWdg.clear();
    delete ui;
}

void DataTableWdg::setHeader(const QStringList& des)
{
    m_model->setHorizontalHeaderLabels(des);
    ui->tableView->setColumnHidden(Tag::ID, true);      // 隐藏id列
    ui->tableView->setColumnHidden(Tag::NAMES, true);   // 隐藏names列
}

void DataTableWdg::addItem(const QStringList& data)
{
    int32_t nRow = m_model->rowCount();
    for (int i = 0; i < data.size(); i++)
    {
        QStandardItem* aItem = new QStandardItem(data.at(i));
        if (i > 1)
        {
            aItem->setTextAlignment(Qt::AlignCenter);
        }
        m_model->setItem(nRow, i, aItem);
    }
}

void DataTableWdg::setImageLinks(MapLinks& links)
{
    m_mapImgLinks.swap(links);
}

void DataTableWdg::clearTable()
{
    DEBUG_MSG("Row count is " << m_model->rowCount());
    m_model->removeRows(0, m_model->rowCount());
}

void DataTableWdg::onChangeColWidth(int32_t w)
{
    w = w > LEFT_SPACE ? w - LEFT_SPACE : w;

    const int32_t width = 200;
    int32_t left_space = w / 2 - 2 * width;
    ui->tableView->setColumnWidth(1, w / 2);
    ui->tableView->setColumnWidth(2, width);
    ui->tableView->setColumnWidth(3, width);
    ui->tableView->setColumnWidth(4, left_space > 0 ? left_space : width);
}

void DataTableWdg::onViewClicked(const QModelIndex& index)
{
    if (index.column() == Tag::LINKS)
    {
        // 显示详细链接信息
        if (nullptr == m_pImgWdg)
        {
            m_pImgWdg = new ImageDisplayWdg();
            m_pImgWdg->setAttribute(Qt::WA_QuitOnClose, false); // 坑点
        }
        int32_t row = index.row();
        m_pImgWdg->addListItem(m_mapImgLinks[row]);
        m_pImgWdg->setTitle(m_model->item(row, Tag::TITLE)->text());
        // 来点偏移更好
        //m_pImgWdg->move(0, 0);
        m_pImgWdg->show();
    }
}

void DataTableWdg::onViewEntered(const QModelIndex& index)
{
    QStandardItem* item = m_model->itemFromIndex(index);
    if (nullptr == item)
        return;
    if (index.column() == Tag::LINKS)
    {
        setCursor(Qt::PointingHandCursor);
        //item->setBackground(QBrush(QColor(255, 200, 227, 200)));
    }else
    {
        setCursor(Qt::ArrowCursor);
        //item->setBackground(QBrush(QColor(255, 255, 255)));
    }
}
