#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class DataTableWdg;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void initUI();
    void initSql();
    uint32_t getSqlDataCount();
    void loadSqlData();
public slots:
    void onReshow();
    void onRefreshData();
private:
    virtual void resizeEvent(QResizeEvent* event) override;
private:
    QSqlDatabase m_db;
    bool m_bDbOpen{false};
    QString m_curTbName{""};
    DataTableWdg* m_curTbWdg{nullptr};
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
