#include "imagedisplaywdg.h"
#include "ui_imagedisplay.h"
#include <QMouseEvent>
#include <QListWidgetItem>
#include <QPixmap>
#include <QScreen>
#include <QFileDialog>
#include <QThread>
#include <QMessageBox>
#include <QGraphicsDropShadowEffect>
#include "imagelinkitem.h"
#include "utility/defs.h"
#include "imagepreviewwdg.h"

ImageDisplayWdg::ImageDisplayWdg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageDisplayWdg)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    // 添加阴影
    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect(ui->widgetHeader);
    shadowEffect->setColor(Qt::lightGray);
    shadowEffect->setBlurRadius(16);
    shadowEffect->setOffset(0, 0);
    ui->widget->setGraphicsEffect(shadowEffect); // 只能设置给一个

    ui->progressBar->hide();

    m_pNetManager = new QNetworkAccessManager(this);
    m_pDownloadManager = new QNetworkAccessManager(this);
    connect(m_pNetManager, SIGNAL(finished(QNetworkReply*)), SLOT(onReplyFinished(QNetworkReply*)));
    connect(m_pDownloadManager, SIGNAL(finished(QNetworkReply*)), SLOT(onDownloadFinished(QNetworkReply*)));

    connect(ui->btn_quit, SIGNAL(clicked()), SLOT(onBtnQuitClicked()));
    connect(ui->btn_down, SIGNAL(clicked()), SLOT(onBtnDownloadClicked()));
    connect(ui->chb_name, SIGNAL(toggled(bool)), SLOT(onChbNameChecked(bool)));
}

ImageDisplayWdg::~ImageDisplayWdg()
{
    m_pImgPreviewWdg.clear();
    delete ui;
}

void ImageDisplayWdg::setTitle(const QString& title)
{
    ui->label_title->setText(title);
}

void ImageDisplayWdg::addListItem(const QMap<QString, QString> &data)
{
    int32_t dataCount = data.size();
    int32_t listCount = ui->listWidget->count();
    int32_t maxCount = listCount > dataCount ? listCount : dataCount;
    for (int32_t i = 0; i < maxCount; ++i)
    {
        if (i >= dataCount)
        {// 多余删除
            QListWidgetItem* item = ui->listWidget->item(i);
            if (nullptr != item)
            {
                ImageLinkItem* itemWdg = qobject_cast<ImageLinkItem*>(ui->listWidget->itemWidget(item));
                if (nullptr != itemWdg)
                {
                    delete itemWdg;
                    itemWdg = nullptr;
                }
                delete item;
                item = nullptr;
            }
            continue;
        }
        QListWidgetItem* item = ui->listWidget->item(i);
        if (nullptr == item)
        {
            item = new QListWidgetItem;
            item->setSizeHint(QSize(32, 32));
            ui->listWidget->addItem(item);

            ImageLinkItem* itemWdg = new ImageLinkItem;
            connect(itemWdg, SIGNAL(enterLabel(const QString&)), SLOT(onEnterLabel(const QString&)));
            connect(itemWdg, SIGNAL(leaveLabel(const QString&)), SLOT(onLeaveLabel(const QString&)));
            connect(itemWdg, &ImageLinkItem::mouseMove, this, &ImageDisplayWdg::onLabelMouseMove);
            itemWdg->setItemData(data.keys()[i], data.values()[i]);
            ui->listWidget->setItemWidget(item, itemWdg);
        }else
        {
            ImageLinkItem* itemWdg = qobject_cast<ImageLinkItem*>(ui->listWidget->itemWidget(item));
            if (nullptr == itemWdg)
            {
                itemWdg = new ImageLinkItem;
                connect(itemWdg, SIGNAL(enterLabel(const QString&)), SLOT(onEnterLabel(const QString&)));
                connect(itemWdg, SIGNAL(leaveLabel(const QString&)), SLOT(onLeaveLabel(const QString&)));
                connect(itemWdg, &ImageLinkItem::mouseMove, this, &ImageDisplayWdg::onLabelMouseMove);
            }
            itemWdg->setItemData(data.keys()[i], data.values()[i]);
            ui->listWidget->setItemWidget(item, itemWdg);
        }
    }
}

void ImageDisplayWdg::clearList()
{
    for (int i = 0; i < ui->listWidget->count(); ++i)
    {
        QListWidgetItem* item = ui->listWidget->item(i);
        ImageLinkItem* itemWdg = qobject_cast<ImageLinkItem*>(ui->listWidget->itemWidget(item));
        if (nullptr != itemWdg)
        {
            delete itemWdg;
            itemWdg = nullptr;
        }
        delete item;
        item = nullptr;
    }
}

void ImageDisplayWdg::checkPosition(const QPoint& pos, const QSize& size, QPoint& resPos, QSize& resSize)
{
    // 先赋初值，要是没修改，就会是0
    resPos = QPoint(pos.x() + 15, pos.y() + 15);
    resSize = size;

    const uint32_t nMaxLabelSize = 800;
    qreal hrate = size.height() / qreal(nMaxLabelSize);
    qreal wrate = size.width() / qreal(nMaxLabelSize);
    if (hrate > 1.0 && hrate >= wrate)
    {
        resSize.setHeight(size.height() / hrate);
        resSize.setWidth(size.width() / hrate);
    }else if (wrate > 1.0 && hrate < wrate)
    {
        resSize.setWidth(size.width() / wrate);
        resSize.setHeight(size.height() / wrate);
    }
    QScreen* screen = QGuiApplication::primaryScreen();
    QSize gSize = screen->size();
    int32_t delt = gSize.height() - resPos.y() - resSize.height();
    if (delt < 0)
    {
        resPos.setY(resPos.y() + delt < 0 ? 0 : resPos.y() + delt);
    }// 没判断X是否符和
}

int32_t ImageDisplayWdg::getSelImgCount()
{
    int32_t nSelImg = 0;
    for (int32_t i = 0; i < ui->listWidget->count(); ++i)
    {
        QListWidgetItem* wdgItem = ui->listWidget->item(i);
        if (nullptr == wdgItem) continue;
        ImageLinkItem* item = qobject_cast<ImageLinkItem*>(ui->listWidget->itemWidget(wdgItem));
        if (nullptr == item) continue;
        if (item->isItemSelected()) ++nSelImg;
    }
    return nSelImg;
}

void ImageDisplayWdg::onBtnQuitClicked()
{
    close();
}

void ImageDisplayWdg::onBtnDownloadClicked()
{
    // 判断是否有选中项,没选中则退出
    if (getSelImgCount() <= 0)
    {
        // 请先选择图片
        QMessageBox::information(this, tr("提示"),tr("请先选择图片"));
        return;
    }

    if (m_isDowning)
    {
        // 提示
        QMessageBox::information(this, tr("提示"),tr("请先等待下载完成"));
        return;
    }
    m_isDowning = true;

    // 选择位置
    QFileDialog dlg(this, tr("请选择保存目录:"),m_savePath);
    dlg.setFileMode(QFileDialog::Directory);
    dlg.setViewMode(QFileDialog::Detail);
    if (dlg.exec())
    {
        QStringList fileNames = dlg.selectedFiles();
        if (!(fileNames.last().isEmpty()))
        {
            m_savePath = fileNames.last();
        }
    }else
    {
        return;
    }

    // 遍历列表
    for (int i = 0; i < ui->listWidget->count(); ++i)
    {
        QListWidgetItem* item = ui->listWidget->item(i);
        if (nullptr == item)
        {
            DEBUG_MSG("Item number [" << i+1 << "] is null.");
            continue;
        }
        ImageLinkItem* itemWdg = qobject_cast<ImageLinkItem*>(ui->listWidget->itemWidget(item));
        if (nullptr == itemWdg)
        {
            DEBUG_MSG("Item number [" << i+1 << "] is null.");
            continue;
        }
        if (itemWdg->isItemSelected())
        {
            // 获取名称和链接
            QString url = itemWdg->getUrl();
            m_mapDownload[url] = itemWdg->getName();
        }
    }
    ui->progressBar->setValue(0);
    ui->progressBar->show();
    m_totalDownSize = m_mapDownload.size();
    for (auto& url : m_mapDownload.keys())
    {
        m_pDownloadManager->get(QNetworkRequest(QUrl(url)));
        // 暂停几毫秒
        QThread::msleep(100);
    }
}

void ImageDisplayWdg::onChbNameChecked(bool checked)
{
    // 选择所有项
    for (int i = 0; i < ui->listWidget->count(); ++i)
    {
        QListWidgetItem* item = ui->listWidget->item(i);
        ImageLinkItem* itemWdg = qobject_cast<ImageLinkItem*>(ui->listWidget->itemWidget(item));
        if (nullptr != itemWdg)
        {
            itemWdg->setItemSelected(checked);
        }
    }
}

void ImageDisplayWdg::onEnterLabel(const QString& text)
{
    DEBUG_MSG("enter label " << text);
    if (nullptr == m_pNetManager)
        return;
    m_pNetManager->get(QNetworkRequest(QUrl(text)));//m_pNetReply =

    setCursor(Qt::WaitCursor);
}

void ImageDisplayWdg::onLeaveLabel(const QString& text)
{
    DEBUG_MSG("leave label " << text);

    if (!m_pImgPreviewWdg.isNull())
    {
        m_pImgPreviewWdg->hide();
    }
    setCursor(Qt::ArrowCursor);
}

void ImageDisplayWdg::onReplyFinished(QNetworkReply* reply)
{
    //Q_UNUSED(reply);
    DEBUG_MSG("finish");
    if (nullptr != reply)
    {
        reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        reply->attribute(QNetworkRequest::RedirectionTargetAttribute);

        QString url = reply->url().toString();

        QPixmap img;
        if (reply->error() == QNetworkReply::NoError)
        {
            QByteArray buff = reply->readAll();
            img.loadFromData(buff);
        }else
        {
            // 如果出错或正在加载中，显示默认图片
            img.load(":/icon/error_img.png");
        }
        if (img.isNull())
        {
            DEBUG_MSG("Image is null.");
            reply->deleteLater(); // 释放资源
            return ;
        }
        int32_t height = img.height();
        int32_t width = img.width();
        QPoint point = QCursor::pos();
        QPoint resPos;
        QSize resSize;
        checkPosition(point, QSize(width, height), resPos, resSize);

        if (m_pImgPreviewWdg.isNull())
        {
            m_pImgPreviewWdg.reset(new ImagePreviewWdg());
        }
        m_pImgPreviewWdg->setSize(resSize);
        m_pImgPreviewWdg->move(resPos);
        m_pImgPreviewWdg->setPixmap(img.scaled(resSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        m_pImgPreviewWdg->show();

        setCursor(Qt::ArrowCursor);

        reply->deleteLater(); // 释放资源
    }
}

void ImageDisplayWdg::onDownloadFinished(QNetworkReply* reply)
{
    if (nullptr != reply)
    {
        if (reply->error() == QNetworkReply::NoError)
        {
            QString url = reply->url().toString();
            auto iter = m_mapDownload.find(url);
            if (iter == m_mapDownload.end())
            {
                DEBUG_MSG("Image[" << url << "] not find in map.");
                reply->deleteLater();
                return;
            }
            QString fileName = m_savePath + "/" + m_mapDownload[url] + ".jpg";
            QByteArray buf = reply->readAll();
            QPixmap img;
            img.loadFromData(buf);
            if (!img.save(fileName))
            {
                DEBUG_MSG("Image[" << fileName << "] save failed.");
            }else
            {
                DEBUG_MSG("Image[" << fileName << "] Download successful.");
            }
            m_mapDownload.remove(url);

            // 计算进度
            int32_t total = m_mapDownload.size();
            int32_t progress = int32_t((m_totalDownSize - total) / qreal(m_totalDownSize) * 100);
            ui->progressBar->setValue(progress);

            if (m_mapDownload.isEmpty())
            {
                m_isDowning = false;

                if (nullptr == m_timer)
                {
                    m_timer = new QTimer(this);
                    connect(m_timer, SIGNAL(timeout()), SLOT(onTimeout()));
                }
                m_timer->start(2000);
                // 提示下载完成
                QMessageBox::information(this, tr("提示"),tr("下载完成！"));
            }
        }
        reply->deleteLater();
    }
}

void ImageDisplayWdg::onTimeout()
{
    ui->progressBar->hide();
}

void ImageDisplayWdg::onLabelMouseMove(const QPoint& pos, const QString& text)
{// 传过来的是label里面的坐标？
    Q_UNUSED(text);

    if (!m_pImgPreviewWdg.isNull())
    {
        QSize size = m_pImgPreviewWdg->size();
        QPoint resPos;
        QSize resSize;
        checkPosition(pos, size, resPos, resSize);
        m_pImgPreviewWdg->move(resPos);
    }
}

void ImageDisplayWdg::mouseMoveEvent(QMouseEvent *event)
{
    if (bMousePress && (Qt::LeftButton & event->buttons()))
    {
        this->move(event->globalPos() - posPre);
    }
    event->accept();
}

void ImageDisplayWdg::mousePressEvent(QMouseEvent *event)
{
    if (Qt::LeftButton == event->button())
    {
        QPoint wdgPos = ui->widgetHeader->pos();
        if (ui->widgetHeader->rect().contains(event->pos() - wdgPos))
        {
            posPre = event->globalPos() - this->pos();
            bMousePress = true;
            setCursor(QCursor(Qt::ClosedHandCursor));
        }
    }
    event->accept();
}

void ImageDisplayWdg::mouseReleaseEvent(QMouseEvent *event)
{
    bMousePress = false;
    setCursor(QCursor(Qt::ArrowCursor));
    event->accept();
}

void ImageDisplayWdg::hideEvent(QHideEvent *event)
{
    if (!m_pImgPreviewWdg.isNull())
    {
        m_pImgPreviewWdg.clear();
    }
    // 取消所有选中，包括总选项
    ui->chb_name->setChecked(false);
    for (int32_t i = 0; i < ui->listWidget->count(); ++i)
    {
        QListWidgetItem* wdgItem = ui->listWidget->item(i);
        if (nullptr == wdgItem) continue;
        ImageLinkItem* item = qobject_cast<ImageLinkItem*>(ui->listWidget->itemWidget(wdgItem));
        if (nullptr == item) continue;
        item->setItemSelected(false);
    }
    event->accept();
}
