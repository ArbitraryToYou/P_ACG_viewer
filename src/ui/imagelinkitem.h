#ifndef IMAGELINKITEM_H
#define IMAGELINKITEM_H

#include <QWidget>
#include "plugin/addresslabel.h"

namespace Ui {
class ImageLinkItem;
}

class ImageLinkItem : public QWidget
{
    Q_OBJECT

public:
    explicit ImageLinkItem(QWidget *parent = nullptr);
    ~ImageLinkItem();

    void setItemData(const QString& name, const QString& link);
    void setItemSelected(bool);
    bool isItemSelected();
    QString getName();
    QString getUrl();
signals:
    void enterLabel(const QString& text);
    void leaveLabel(const QString& text);
    void mouseMove(const QPoint& pos, const QString& text);
private:
    Ui::ImageLinkItem *ui;
};

#endif // IMAGELINKITEM_H
