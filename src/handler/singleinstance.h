#ifndef SINGLEINSTANCE_H
#define SINGLEINSTANCE_H

#include <QObject>
#include <QLocalServer>
#include <QLocalSocket>
#include <QPointer>

class SingleInstance : public QObject
{
    Q_OBJECT
public:
    SingleInstance(QObject* parent = 0);
    ~SingleInstance();

    bool isRunning(QString srvName);
    bool listen(QString srvName = ".");
signals:
    void reshow();
private slots:
    void onNewConnection();
    void onReadyRead();
private:
    QPointer<QLocalSocket> m_pSocket;
    QLocalServer m_server;
};

#endif // SINGLEINSTANCE_H
