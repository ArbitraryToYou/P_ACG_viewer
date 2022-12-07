#ifndef MYTBITEMMODEL_H
#define MYTBITEMMODEL_H

#include <QAbstractItemModel>
#include <QObject>

class MyTbItemModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit MyTbItemModel(QObject *parent = nullptr);
};

#endif // MYTBITEMMODEL_H
