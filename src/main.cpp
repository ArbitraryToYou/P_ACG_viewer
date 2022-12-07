#include "ui/mainwindow.h"

#include <QApplication>
#include <QScopedPointer>
#include "handler/singleinstance.h"
#include "utility/defs.h"
#include "ui/sqllogindlg.h"

// argc一般是1, argv[0]是程序的绝对路径
int main(int argc, char *argv[])
{
    // 如果没有创建QApplication对象，信号是发不出去的！
    QApplication a(argc, argv);

    // 单实例程序实现
    QScopedPointer<SingleInstance> spSingleInst;
    spSingleInst.reset(new SingleInstance);
    QString srvName("monkey");
    if (spSingleInst->isRunning(srvName))
    {
        DEBUG_MSG("oh no, another application is already running.This will be quit.");
        return 0;
    }
    spSingleInst->listen(srvName);

    QScopedPointer<SqlLoginDlg> spSqlDlg{new SqlLoginDlg};
    if (QDialog::Accepted != spSqlDlg->exec())
    {
        DEBUG_MSG("Application will be quit.");
        return 0;
    }

    MainWindow w;
    QObject::connect(spSingleInst.data(), &SingleInstance::reshow, &w, &MainWindow::onReshow);
    w.show();
    return a.exec();
}
