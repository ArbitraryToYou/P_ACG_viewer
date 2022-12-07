#ifndef GLOBALDATA_H
#define GLOBALDATA_H
#include <QString>

namespace Joky{

struct SqlInfo
{
    QString serverIp{"127.0.0.1"};
    int port{3306};
    QString userName{"root"};
    QString passwrod{""};
};

extern SqlInfo g_sqlInfo;
}// Joky


#endif // GLOBALDATA_H
