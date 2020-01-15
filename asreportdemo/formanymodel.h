#ifndef FORMANYMODEL_H
#define FORMANYMODEL_H

#include <QWidget>
#include "mainwindow.h"

namespace Ui {
class FormAnyModel;
}

class FormAnyModel : public QWidget
{
    Q_OBJECT

public:
    explicit FormAnyModel(QWidget *parent = 0);
    ~FormAnyModel();

private:
    Ui::FormAnyModel *ui;

    QStringListModel m_stringsmodel;
    QStandardItemModel m_itemsmodel;

    void updateData();


public:
    void initPrint();

private slots:
    void on_butAdd_clicked();
    void on_butDel_clicked();
};

#endif // FORMANYMODEL_H
