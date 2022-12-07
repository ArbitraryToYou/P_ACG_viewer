#include "imagepreviewwdg.h"
#include "ui_imagepreviewwdg.h"
#include <QGraphicsDropShadowEffect>
#include <QBitmap>
#include <QPainter>

ImagePreviewWdg::ImagePreviewWdg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImagePreviewWdg)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->hide();

    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0, 0);
    shadow->setColor(Qt::lightGray);
    shadow->setBlurRadius(10);
    ui->widget->setGraphicsEffect(shadow);

//    m_timer = new QTimer(this);
//    connect(m_timer, &QTimer::timeout, this, &ImagePreviewWdg::onTimerFinished);
//    m_timer->start(15000);
}

ImagePreviewWdg::~ImagePreviewWdg()
{
    delete ui;
}

void ImagePreviewWdg::setPixmap(const QPixmap &img)
{
    ui->label->setPixmap(roundImage(img, 10));
}

void ImagePreviewWdg::setSize(QSize size)
{
    this->resize(size.width() + 20, size.height() + 20);
    ui->label->resize(size);
    ui->label->move(0, 0);
}

void ImagePreviewWdg::onTimerFinished()
{
    this->hide();
}

QPixmap ImagePreviewWdg::roundImage(const QPixmap &img, int32_t radius)
{
    if (img.isNull())
    {
        return QPixmap();
    }
    QSize size(img.size());
    QBitmap mask(size);
    QPainter painter(&mask);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.fillRect(mask.rect(), Qt::white);
    painter.setBrush(QColor(0, 0, 0));
    painter.drawRoundedRect(mask.rect(), radius, radius);
    QPixmap image = img;
    image.setMask(mask);
    return image;
}
