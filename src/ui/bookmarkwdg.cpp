#include "bookmarkwdg.h"
#include "ui_bookmarkwdg.h"
#include <QButtonGroup>
#include "utility/defs.h"

BookMarkWdg::BookMarkWdg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BookMarkWdg)
{
    ui->setupUi(this);
    initUI();

    connect(this, SIGNAL(showButtons()), this, SLOT(onShowButtons()));
    connect(ui->comb_perpage, SIGNAL(currentIndexChanged(const QString&)), SLOT(onCurrentIndexChanged(const QString&)));
    connect(m_pBtnGroup.data(), SIGNAL(buttonClicked(int)), this, SLOT(onButtonGroupClicked(int)));
    connect(ui->btn_pre, SIGNAL(clicked()), this, SLOT(onBtnPreClicked()));
    connect(ui->btn_back, SIGNAL(clicked()), this, SLOT(onBtnBackClicked()));
    connect(ui->Edit_jump, SIGNAL(returnPressed()), SLOT(onEditReturnPressed()));
    DEBUG_MSG("Bookmark widget init ok.");
}

BookMarkWdg::~BookMarkWdg()
{
    delete ui;
}

void BookMarkWdg::initUI()
{
    DEBUG_MSG("Bookmark widget initUI.");
    ui->label_total->setText(tr("共0项"));
    ui->comb_perpage->addItem("10", QVariant(10));
    ui->comb_perpage->addItem("15", QVariant(15));
    ui->comb_perpage->addItem("20", QVariant(20));
    ui->comb_perpage->addItem("25", QVariant(25));

    m_pBtnGroup = new QButtonGroup;
    m_pBtnGroup->addButton(ui->btn_1, 0);
    m_pBtnGroup->addButton(ui->btn_2, 1);
    m_pBtnGroup->addButton(ui->btn_3, 2);
    m_pBtnGroup->addButton(ui->btn_4, 3);
    m_pBtnGroup->addButton(ui->btn_5, 4);
    DEBUG_MSG("initUI ok.");
}

void BookMarkWdg::setTotalItems(uint32_t num)
{
    ui->label_total->setText(QString(tr("共%1项")).arg(num));
    m_totalItems = num;
    emit showButtons();
}

void BookMarkWdg::refreshButtons()
{
    if (m_totalPage > BTN_NUM)
    {
        uint32_t diff = m_totalPage - m_curPage;
        if(diff < BTN_NUM)
        {
            for (uint32_t idx = 0; idx < BTN_NUM; ++idx)
            {
                QPushButton* btn = qobject_cast<QPushButton*>(m_pBtnGroup->button(idx));
                if (nullptr == btn)
                {
                    DEBUG_MSG("This button can't convert to QPushButton.");
                    continue;
                }
                btn->setText(QString::number(m_totalPage - BTN_NUM + idx));
                btn->show();
                btn->setEnabled(m_curPage != m_totalPage - BTN_NUM + idx);// 区分是不是当前页
            }
        }else
        {
            for (uint32_t idx = 0; idx < BTN_NUM; ++idx)
            {
                QPushButton* btn = qobject_cast<QPushButton*>(m_pBtnGroup->button(idx));
                if (nullptr == btn)
                {
                    DEBUG_MSG("This button can't convert to QPushButton.");
                    continue;
                }
                if (idx == 3)
                    btn->setText("...");
                else if(idx == BTN_NUM - 1)
                    btn->setText(QString::number(m_totalPage));
                else
                    btn->setText(QString::number(m_curPage + idx));
                btn->show();
                btn->setEnabled(m_curPage != m_curPage + idx);// 区分是不是当前页
            }
        }
    }else
    {
        for (uint32_t idx = 0; idx < m_totalPage; ++idx)
        {
            QPushButton* btn = qobject_cast<QPushButton*>(m_pBtnGroup->button(idx));
            if (nullptr == btn)
            {
                DEBUG_MSG("This button can't convert to QPushButton.");
                continue;
            }
            btn->setEnabled(m_curPage != idx+1);// 区分是不是当前页
        }
    }
    emit refreshData();
}

// slots
void BookMarkWdg::onShowButtons()
{
    DEBUG_MSG("onShowButtons begin.");
    uint32_t totalPage = m_totalItems % m_itemPerPage ? m_totalItems / m_itemPerPage + 1 : m_totalItems / m_itemPerPage;
    m_totalPage = totalPage;
    DEBUG_MSG("Total pages: " << totalPage);
    if (totalPage > BTN_NUM)
    {
        for (uint32_t idx = 0; idx < BTN_NUM; ++idx)
        {
            QPushButton* btn = qobject_cast<QPushButton*>(m_pBtnGroup->button(idx));
            if (nullptr == btn)
            {
                DEBUG_MSG("This button can't convert to QPushButton.");
                continue;
            }
            if (idx == 3)
            {
                btn->setText("...");
            }else if (idx == BTN_NUM - 1)
            {
                btn->setText(QString::number(totalPage));
            }else
            {
                btn->setText(QString::number(idx+1));
            }
            btn->setEnabled(m_curPage != idx+1);// 区分是不是当前页
        }
    }
    else{
        for (uint32_t idx = 0; idx < BTN_NUM; ++idx)
        {
            QPushButton* btn = qobject_cast<QPushButton*>(m_pBtnGroup->button(idx));
            if (nullptr == btn)
            {
                DEBUG_MSG("This button can't convert to QPushButton.");
                continue;
            }
            if (totalPage <= idx)
            {
                btn->hide();
                continue;
            }
            btn->setText(QString::number(idx+1));
            btn->setEnabled(m_curPage != idx + 1);// 区分是不是当前页
        }
    }

    ui->btn_pre->setEnabled(m_curPage > 1);
    ui->btn_back->setEnabled(m_curPage < m_totalPage);

    m_curPage = 1;
    emit refreshData();
}

void BookMarkWdg::onCurrentIndexChanged(const QString& text)
{
    m_itemPerPage = text.toUInt();
    emit onShowButtons();
}

void BookMarkWdg::onButtonGroupClicked(int id)
{
    QPushButton* btn = qobject_cast<QPushButton*>(m_pBtnGroup->button(id));
    if (nullptr == btn)
    {
        DEBUG_MSG("this button can't convert to QPushButton.");
        return;
    }
    int32_t nPage = btn->text().toInt();
    m_curPage = nPage;
    refreshButtons();
    emit refreshData();
}

void BookMarkWdg::onBtnPreClicked()
{
    if (m_curPage <= 1)
    {
        m_curPage = 1;
        return;
    }

    --m_curPage;
    ui->btn_back->setEnabled(m_curPage < m_totalPage);
    refreshButtons();
}

void BookMarkWdg::onBtnBackClicked()
{
    if (m_curPage >= m_totalPage)
    {
        m_curPage = m_totalPage;
        return;
    }

    ++m_curPage;
    ui->btn_pre->setEnabled(m_curPage > 1);
    refreshButtons();
}

void BookMarkWdg::onEditReturnPressed()
{
    uint32_t page = ui->Edit_jump->text().toUInt();
    m_curPage = page;
    refreshButtons();
}
