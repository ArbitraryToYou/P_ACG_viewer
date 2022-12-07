#ifndef DEFS_H
#define DEFS_H

#ifdef QT_DEBUG
#include <QDebug>
#define DEBUG_MSG(msg) qDebug() << "[" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << "]" << msg
#else
#define DEBUG_MSG(msg) ((void)0)
#endif // QT_DEBUG

#define SQLCONN_NAME "p_acg"
#define DATABASE_NAME "p_acg"

#endif // DEFS_H
