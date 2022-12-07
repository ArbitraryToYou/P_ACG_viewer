#ifndef ADDRESSLABEL_H
#define ADDRESSLABEL_H

#include <QLabel>
#include <QObject>

class AddressLabel : public QLabel
{
    Q_OBJECT
public:
    explicit AddressLabel(QWidget* parent = nullptr);
signals:
    void enterSignal(const QString& text);
    void leaveSignal(const QString& text);
    void mouseMove(const QPoint& pos, const QString& text);
private:
    virtual void enterEvent(QEvent* ev) override;
    virtual void leaveEvent(QEvent* ev) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
private:
};

#endif // ADDRESSLABEL_H
