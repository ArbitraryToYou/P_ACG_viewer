#ifndef IMAGEPREVIEWWDG_H
#define IMAGEPREVIEWWDG_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class ImagePreviewWdg;
}

class ImagePreviewWdg : public QWidget
{
    Q_OBJECT

public:
    explicit ImagePreviewWdg(QWidget *parent = nullptr);
    ~ImagePreviewWdg();

    void setPixmap(const QPixmap&);
    void setSize(QSize size);
private slots:
    void onTimerFinished();
private:
    QPixmap roundImage(const QPixmap& img, int32_t radius);
private:
    //QTimer* m_timer{nullptr};
private:
    Ui::ImagePreviewWdg *ui;
};

#endif // IMAGEPREVIEWWDG_H
