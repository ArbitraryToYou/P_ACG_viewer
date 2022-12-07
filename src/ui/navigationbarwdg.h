#ifndef NAVIGATIONBARWDG_H
#define NAVIGATIONBARWDG_H

#include <QWidget>

namespace Ui {
class NavigationBarWdg;
}

class NavigationBarWdg : public QWidget
{
    Q_OBJECT

public:
    explicit NavigationBarWdg(QWidget *parent = nullptr);
    ~NavigationBarWdg();

private:
    Ui::NavigationBarWdg *ui;
};

#endif // NAVIGATIONBARWDG_H
