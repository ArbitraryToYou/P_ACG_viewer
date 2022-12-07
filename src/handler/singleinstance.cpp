#include "singleinstance.h"
#include "utility/defs.h"

SingleInstance::SingleInstance(QObject* parent)
    : QObject(parent)
    , m_pSocket(nullptr)
{
    connect(&m_server, &QLocalServer::newConnection, this, &SingleInstance::onNewConnection);
}

SingleInstance::~SingleInstance()
{
    m_server.close();
}

bool SingleInstance::isRunning(QString srvName)
{
    QLocalSocket socket;
    socket.connectToServer(srvName, QLocalSocket::ReadWrite);
    if (socket.waitForConnected())
    {
        // do something...
        QByteArray buff;
        buff.append(QString("Reshow Old"));
        socket.write(buff);
        socket.waitForBytesWritten();
        socket.close();
        return true;
    }
    return false;
}

bool SingleInstance::listen(QString srvName)
{
    m_server.removeServer(srvName);
    bool bRes = m_server.listen(srvName);
    DEBUG_MSG("server start listen ok.");
    return bRes;
}

void SingleInstance::onNewConnection()
{
    DEBUG_MSG("new connection is coming!");
    m_pSocket.clear();
    m_pSocket = m_server.nextPendingConnection();
    connect(m_pSocket.data(), &QLocalSocket::readyRead, this, &SingleInstance::onReadyRead);
}

void SingleInstance::onReadyRead()
{
    QByteArray tmp = m_pSocket->readAll();
    if ("Reshow Old" == QString(tmp))
    {
        emit reshow();
    }

    m_pSocket->close();
    m_pSocket->deleteLater();
}
