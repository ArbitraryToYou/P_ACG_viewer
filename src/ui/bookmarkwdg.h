#ifndef BOOKMARKWDG_H
#define BOOKMARKWDG_H

#include <QWidget>
#include <QPointer>

namespace Ui {
class BookMarkWdg;
}

class QButtonGroup;
class BookMarkWdg : public QWidget
{
    Q_OBJECT

public:
    explicit BookMarkWdg(QWidget *parent = nullptr);
    ~BookMarkWdg();

    void initUI();
    void setTotalItems(uint32_t num);
    int32_t getCurrentPage() {return m_curPage;}
    int32_t getItemPerPage() {return m_itemPerPage;}
private:
    void refreshButtons();
signals:
    void showButtons();
    void refreshData();
private slots:
    void onShowButtons();
    void onCurrentIndexChanged(const QString& text);
    void onButtonGroupClicked(int id);
    void onBtnPreClicked();
    void onBtnBackClicked();
    void onEditReturnPressed();
private:
    QPointer<QButtonGroup> m_pBtnGroup;
    uint32_t m_totalItems{0};
    uint32_t m_curPage{1};
    uint32_t m_totalPage{1};
    uint32_t m_itemPerPage{10};
    const uint32_t BTN_NUM = 5;
private:
    Ui::BookMarkWdg *ui;
};

#endif // BOOKMARKWDG_H
