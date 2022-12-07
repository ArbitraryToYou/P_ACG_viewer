#ifndef IMAGEDISPLAYWDG_H
#define IMAGEDISPLAYWDG_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSharedPointer>
#include <QLabel>
#include <QTimer>

namespace Ui {
class ImageDisplayWdg;
}

class ImagePreviewWdg;
class ImageDisplayWdg : public QWidget
{
    Q_OBJECT

public:
    explicit ImageDisplayWdg(QWidget *parent = nullptr);
    ~ImageDisplayWdg();

    void setTitle(const QString& title);
    void addListItem(const QMap<QString, QString>& data);
    void clearList();
    void checkPosition(const QPoint& pos, const QSize& size, QPoint& resPos, QSize& resSize);
    int32_t getSelImgCount();
public slots:
    void onBtnQuitClicked();
    void onBtnDownloadClicked();
    void onChbNameChecked(bool);
    void onEnterLabel(const QString& text);
    void onLeaveLabel(const QString& text);
    void onReplyFinished(QNetworkReply*);
    void onDownloadFinished(QNetworkReply*);
    void onTimeout();
    void onLabelMouseMove(const QPoint& pos, const QString& text);
private: // events
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void hideEvent(QHideEvent* event) override;
private: // data
    bool bMousePress{false};
    QPoint posPre;
    QNetworkAccessManager* m_pNetManager{nullptr};
    QNetworkAccessManager* m_pDownloadManager{nullptr};
    //QNetworkReply* m_pNetReply{nullptr};
    QSharedPointer<ImagePreviewWdg> m_pImgPreviewWdg{nullptr};
    QMap<QString, QString> m_mapDownload;
    QString m_savePath{"."};
    bool m_isDowning{false};
    int32_t m_totalDownSize{0};
    QTimer* m_timer{nullptr};
private:
    Ui::ImageDisplayWdg *ui;
};

#endif // IMAGEDISPLAYWDG_H
