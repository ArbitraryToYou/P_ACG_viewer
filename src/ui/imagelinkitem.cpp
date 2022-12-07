#include "imagelinkitem.h"
#include "ui_imagelinkitem.h"

ImageLinkItem::ImageLinkItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageLinkItem)
{
    ui->setupUi(this);
    connect(ui->label_addr, SIGNAL(enterSignal(const QString&)), this, SIGNAL(enterLabel(const QString&)));
    connect(ui->label_addr, SIGNAL(leaveSignal(const QString&)), this, SIGNAL(leaveLabel(const QString&)));
    connect(ui->label_addr, &AddressLabel::mouseMove, this, &ImageLinkItem::mouseMove);
}

ImageLinkItem::~ImageLinkItem()
{
    delete ui;
}


void ImageLinkItem::setItemData(const QString &name, const QString &link)
{
    ui->chb_name->setText(name);
    ui->label_addr->setText(link);
}

void ImageLinkItem::setItemSelected(bool selected)
{
    ui->chb_name->setChecked(selected);
}

bool ImageLinkItem::isItemSelected()
{
    return ui->chb_name->isChecked();
}

QString ImageLinkItem::getName()
{
    return ui->chb_name->text();
}

QString ImageLinkItem::getUrl()
{
    return ui->label_addr->text();
}
