#ifndef DATATABLEWDG_H
#define DATATABLEWDG_H

#include <QWidget>
//#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QPointer>
#include "imagedisplaywdg.h"
#include "handler/tablemodel.h"

namespace Ui {
class DataTableWdg;
}


class DataTableWdg : public QWidget
{
    Q_OBJECT

public:
    using MapLinks = QMap<int32_t, QMap<QString, QString>>;
    enum Tag
    {
        ID = 0,
        TITLE,
        TYPE,
        DATE,
        LINKS,
        NAMES
    };

    explicit DataTableWdg(const QString& tableName, QWidget *parent = nullptr);
    ~DataTableWdg();

    void setHeader(const QStringList& desc);
    void addItem(const QStringList& data);
    void setImageLinks(MapLinks& links);
    void clearTable();
public slots:
    void onChangeColWidth(int32_t w);
    void onViewClicked(const QModelIndex& index);
    void onViewEntered(const QModelIndex& index);   // 需要打开mousetracking
private:
    TableModel* m_model{nullptr};
    QItemSelectionModel* m_selectionModel{nullptr};
    QString m_tbName{""};
    const int32_t LEFT_SPACE = 38; // 列预留空间
    QPointer<ImageDisplayWdg> m_pImgWdg;
    MapLinks m_mapImgLinks;
private:
    Ui::DataTableWdg *ui;
};

#endif // DATATABLEWDG_H
