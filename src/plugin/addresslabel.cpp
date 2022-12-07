#include "addresslabel.h"
#include <QEvent>
#include <QMouseEvent>

AddressLabel::AddressLabel(QWidget* parent)
    : QLabel(parent)
{
    setMouseTracking(true);
}

void AddressLabel::enterEvent(QEvent *ev)
{
    Q_UNUSED(ev);
    emit enterSignal(this->text());
}

void AddressLabel::leaveEvent(QEvent *ev)
{
    Q_UNUSED(ev);
    emit leaveSignal(this->text());
}

void AddressLabel::mouseMoveEvent(QMouseEvent *event)
{
    emit mouseMove(event->globalPos(), this->text());
    event->accept();
}
