#ifndef SQLMANAGER_H
#define SQLMANAGER_H

#include <QObject>

class SqlManager : public QObject
{
    Q_OBJECT
public:
    SqlManager(QObject* parent = nullptr);
};

#endif // SQLMANAGER_H
