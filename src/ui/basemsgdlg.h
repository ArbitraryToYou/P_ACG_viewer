#ifndef BASEMSGDLG_H
#define BASEMSGDLG_H

#include <QDialog>

namespace Ui {
class BaseMsgDlg;
}

class BaseMsgDlg : public QDialog
{
    Q_OBJECT

public:
    explicit BaseMsgDlg(QWidget *parent = nullptr);
    ~BaseMsgDlg();

private:
    Ui::BaseMsgDlg *ui;
};

#endif // BASEMSGDLG_H
