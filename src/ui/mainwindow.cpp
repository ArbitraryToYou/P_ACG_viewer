#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopWidget>
#include <QSqlQuery>
#include <QResizeEvent>
#include "datatablewdg.h"
#include "bookmarkwdg.h"
#include "utility/defs.h"
#include "utility/GlobalData.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // Q_INIT_RESOURCE(resource);

    ui->setupUi(this);
    initSql();
    initUI();
    getSqlDataCount();
    connect(ui->wdgBookMark, &BookMarkWdg::refreshData, this, &MainWindow::onRefreshData);
}

MainWindow::~MainWindow()
{
    if (m_db.isOpen())
    {
        m_db.close();
        m_db = QSqlDatabase();
    }
    if (QSqlDatabase::contains(SQLCONN_NAME))
        m_db.removeDatabase(SQLCONN_NAME);
    delete ui;
}

void MainWindow::initUI()
{
    ui->statusbar->hide();
    ui->menubar->hide();
    // 默认自带两个tab页面
    ui->tabWidget->removeTab(1);
    ui->tabWidget->removeTab(0);

    if (m_bDbOpen)
    {
        QSqlQuery query(m_db);
        query.exec("SHOW TABLES");
        while (query.next())
        {
            QString tbName = query.value(0).toString();
            DataTableWdg* tab = new DataTableWdg(tbName, this);
            ui->tabWidget->addTab(tab, tbName);
            DEBUG_MSG("table name: " << tbName);
        }
    }
    if (ui->tabWidget->count() > 0)
    {
        // init curTableName
        m_curTbName = ui->tabWidget->tabText(ui->tabWidget->currentIndex());

        QSqlQuery query(m_db);
        query.exec(QString("DESC %1").arg(m_curTbName));
        QStringList desc;
        while (query.next())
        {
            desc.append(query.value(0).toString());
        }
        DataTableWdg* pCurWdg = qobject_cast<DataTableWdg*>(ui->tabWidget->currentWidget());
        if (nullptr != pCurWdg)
        {
            m_curTbWdg = pCurWdg;
            //pCurWdg->openTable(m_curTbName, desc);
            pCurWdg->setHeader(desc);
            loadSqlData();
        }
    }
}

void MainWindow::initSql()
{
    m_db = QSqlDatabase::addDatabase("QMYSQL", SQLCONN_NAME);
    m_db.setHostName(Joky::g_sqlInfo.serverIp);
    m_db.setPort(Joky::g_sqlInfo.port);
    m_db.setUserName(Joky::g_sqlInfo.userName);
    m_db.setPassword(Joky::g_sqlInfo.passwrod);
    m_db.setDatabaseName(DATABASE_NAME);
    m_bDbOpen = m_db.open();
}

uint32_t MainWindow::getSqlDataCount()
{
    QSqlQuery q(m_db);
    QString sql(QString("SELECT count(*) FROM %1").arg(m_curTbName));
    q.exec(sql);
    if (q.next())
    {
        uint32_t count = q.value(0).toUInt();
        DEBUG_MSG("Table: " << m_curTbName << ", count: " << count);
        ui->wdgBookMark->setTotalItems(count);
        return count;
    }
    return 0;
}

void MainWindow::loadSqlData()
{
    m_curTbWdg->clearTable();
    int32_t curPage = ui->wdgBookMark->getCurrentPage();
    DEBUG_MSG("Current Page: " << curPage);
    if (0 >= curPage)
    {
        DEBUG_MSG("Current data page less then 0.");
        return;
    }
    int32_t itemPerPage = ui->wdgBookMark->getItemPerPage();
    int32_t begin = (curPage - 1) * itemPerPage;
    //int32_t end = curPage * itemPerPage;

    QSqlQuery q(m_db);
    // begin开始位置，往后多少个数据
    QString sql = QString("SELECT * FROM %1 LIMIT %2,%3").arg(m_curTbName).arg(begin).arg(itemPerPage);
    q.exec(sql);
    QStringList datas;
    DataTableWdg::MapLinks mMaplinks;
    int32_t nIndex = 0;
    while (q.next())
    {
        datas.clear();
        QMap<QString, QString> mlinks;
        int32_t id = q.value(0).toInt();
        QString title = q.value(1).toString();
        QString type = q.value(2).toString();
        QString date = q.value(3).toString();
        QStringList links = q.value(4).toString().split('|');
        QStringList names = q.value(5).toString().split('|');
        for (int i = 0; i < names.size(); ++i)
        {
            if (i < links.size())
            {
                mlinks[names[i]] = links[i];
            }
        }

        datas << QString::number(id) << title << type << date << tr("点击查看") << tr("点击查看");

        // 插入表中
        if (nullptr != m_curTbWdg)
            m_curTbWdg->addItem(datas);
        mMaplinks[nIndex++].swap(mlinks);
    }
    m_curTbWdg->setImageLinks(mMaplinks);
}

void MainWindow::onReshow()
{
    QDesktopWidget* pDesk = QApplication::desktop();
    this->move((pDesk->width() - this->width()) / 2, (pDesk->height() - this->height()) / 2);
    if (this->isHidden())
        this->show();
    activateWindow();
}

void MainWindow::onRefreshData()
{
    loadSqlData();
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
   int32_t w = event->size().width();
   m_curTbWdg != nullptr ? m_curTbWdg->onChangeColWidth(w) : (void)0;
   event->accept();
}
