#ifndef FORMDETAILLIST2_H
#define FORMDETAILLIST2_H

#include <QWidget>
#include "mainwindow.h"

namespace Ui {
class FormDetailList2;
}

class FormDetailList2 : public QWidget
{
    Q_OBJECT

public:
    explicit FormDetailList2(QWidget *parent = 0);
    ~FormDetailList2();

private slots:
void slotDetailSelect(const QModelIndex &current, const QModelIndex &previous);

void slotModelOnNextRec(QObject *);

void slotBeginPrintOrder();

void slotBeginPrint();
private:
    Ui::FormDetailList2 *ui;

    QSqlQueryModel* m_customerorders;
    QSqlQueryModel* m_itemsparts;

     QSqlDatabase m_db;

public:
     void initPrint();

     void initPrintOrders();


};

#endif // FORMDETAILLIST2_H
