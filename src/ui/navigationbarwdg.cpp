#include "navigationbarwdg.h"
#include "ui_navigationbarwdg.h"

NavigationBarWdg::NavigationBarWdg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NavigationBarWdg)
{
    ui->setupUi(this);
}

NavigationBarWdg::~NavigationBarWdg()
{
    delete ui;
}
