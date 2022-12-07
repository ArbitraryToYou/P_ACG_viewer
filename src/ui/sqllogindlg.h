#ifndef SQLLOGINDLG_H
#define SQLLOGINDLG_H

#include <QDialog>

namespace Ui {
class SqlLoginDlg;
}

class SqlLoginDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SqlLoginDlg(QWidget *parent = nullptr);
    ~SqlLoginDlg();

    void initUI();
public slots:
    void onBtnOkClicked(bool);
    void onBtnCancelClicked(bool);
    void onBtnQuitClicked(bool);
private: // events
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
private: // data
    bool bMousePress{false};
    QPoint posPre;
private:
    Ui::SqlLoginDlg *ui;
};

#endif // SQLLOGINDLG_H
