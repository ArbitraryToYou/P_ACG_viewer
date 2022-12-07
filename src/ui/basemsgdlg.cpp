#include "basemsgdlg.h"
#include "ui_basemsgdlg.h"

BaseMsgDlg::BaseMsgDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BaseMsgDlg)
{
    ui->setupUi(this);
}

BaseMsgDlg::~BaseMsgDlg()
{
    delete ui;
}
