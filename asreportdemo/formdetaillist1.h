#ifndef FORMDETAILLIST1_H
#define FORMDETAILLIST1_H

#include <QWidget>
#include "mainwindow.h"

namespace Ui {
class FormDetailList1;
}

class FormDetailList1 : public QWidget
{
    Q_OBJECT

public:
    explicit FormDetailList1(QWidget *parent = 0);
    ~FormDetailList1();


private slots:
void slotDetailSelect(const QModelIndex &current, const QModelIndex &previous);

void slotModelOnNextRec(QObject *);

void slotBeginPrint();

private:
    Ui::FormDetailList1 *ui;


    QSqlQueryModel* m_customerliter;
    QSqlQueryModel* m_customer;

     QSqlDatabase m_db;


public:

    void initPrint();

};

#endif // FORMDETAILLIST1_H
