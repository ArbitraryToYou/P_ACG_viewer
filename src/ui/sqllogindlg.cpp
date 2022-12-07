#include "sqllogindlg.h"
#include "ui_sqllogindlg.h"
#include <QFontDatabase>
#include <QSqlDatabase>
#include <QSqlError>
#include <QMouseEvent>
#include <QMessageBox>
#include <QGraphicsDropShadowEffect>
#include "utility/GlobalData.h"
#include "utility/defs.h"

//Joky::SqlInfo g_sqlInfo;

SqlLoginDlg::SqlLoginDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SqlLoginDlg)
{
    Q_INIT_RESOURCE(resource);

    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0, 0);
    shadow->setColor(Qt::lightGray);
    shadow->setBlurRadius(16);
    ui->widget_3->setGraphicsEffect(shadow);

    initUI();

    connect(ui->btnOk, SIGNAL(clicked(bool)), SLOT(onBtnOkClicked(bool)));
    connect(ui->btnCancel, SIGNAL(clicked(bool)), SLOT(onBtnCancelClicked(bool)));
    connect(ui->btn_quit, SIGNAL(clicked(bool)), SLOT(onBtnQuitClicked(bool)));
}

SqlLoginDlg::~SqlLoginDlg()
{

    delete ui;
}

void SqlLoginDlg::initUI()
{
//    QFont font(":/font/geetype.ttf", 12, QFont::Normal);

    int fontId = QFontDatabase::addApplicationFont(":/font/apple_slim.ttf");
    QString fontName = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont font(fontName, 12);
    ui->label_title->setFont(font);
    font.setPointSize(9);
    ui->label_ip->setFont(font);
    ui->label_port->setFont(font);
    ui->label_user->setFont(font);
    ui->label_pd->setFont(font);
    ui->btnOk->setFont(font);
    ui->btnCancel->setFont(font);
}

void SqlLoginDlg::onBtnOkClicked(bool clicked)
{
    Q_UNUSED(clicked);
    // do check sql
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", SQLCONN_NAME);
        db.setHostName(ui->Edit_ip->text());
        db.setPort(ui->spinBox_port->value());
        db.setUserName(ui->Edit_user->text());
        db.setPassword(ui->Edit_pd->text());
        if (!db.open())
        {
            DEBUG_MSG("Connect MYSQL Database IP: " << ui->Edit_ip->text()\
                      << ", Port: " << ui->spinBox_port->value()\
                      << ", User: " << ui->Edit_user->text()\
                      << ", Password: " << ui->Edit_pd->text()\
                      << " error.Error is " << db.lastError().text());
            // show messageBox
            QMessageBox msgBox("Error", db.lastError().text(), QMessageBox::Warning,
                               QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
            msgBox.exec();
            return;
        }

        Joky::g_sqlInfo.serverIp = ui->Edit_ip->text();
        Joky::g_sqlInfo.port = ui->spinBox_port->value();
        Joky::g_sqlInfo.userName = ui->Edit_user->text();
        Joky::g_sqlInfo.passwrod = ui->Edit_pd->text();

        db.close();
    }
    // removeDatabase()会增加db的引用计数，导致原先的db仍然没有释放，因此将db放置在作用域中
    QSqlDatabase::removeDatabase(SQLCONN_NAME);
    accept();
}

void SqlLoginDlg::onBtnCancelClicked(bool clicked)
{
    Q_UNUSED(clicked);
    reject();
}

void SqlLoginDlg::onBtnQuitClicked(bool clicked)
{
    Q_UNUSED(clicked);
    reject();
}

void SqlLoginDlg::mouseMoveEvent(QMouseEvent *event)
{
    if (bMousePress && (Qt::LeftButton & event->buttons()))
    {
        this->move(event->globalPos() - posPre);
//        DEBUG_MSG("posX:" << event->pos().x() << ", posY:" << event->pos().y());
//        DEBUG_MSG("globalPosX:" << event->globalPos().x() << ", globalPosY:" << event->globalPos().y());
    }
    event->accept();
}

void SqlLoginDlg::mousePressEvent(QMouseEvent *event)
{
    if (Qt::LeftButton == event->button())
    {
        QPoint wdgPos = ui->widget->pos();
        if (ui->widget->rect().contains(event->pos() - wdgPos))  // 鼠标位置要 - 窗口左上角位置？
        {
            posPre = event->globalPos() - this->pos();
            bMousePress = true;
            setCursor(QCursor(Qt::ClosedHandCursor));
        }
    }
    event->accept();
}

void SqlLoginDlg::mouseReleaseEvent(QMouseEvent *event)
{
    bMousePress = false;
    setCursor(QCursor(Qt::ArrowCursor));
    event->accept();
}
