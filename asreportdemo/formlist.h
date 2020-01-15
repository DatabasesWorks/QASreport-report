#ifndef FORMLIST_H
#define FORMLIST_H

#include <QtWidgets>
#include "mainwindow.h"

namespace Ui {
class FormList;
}

class FormList : public QWidget
{
    Q_OBJECT

public:
    explicit FormList(QWidget *parent = 0);
    ~FormList();

private:
    Ui::FormList *ui;


    QSqlQueryModel* m_customer;

private slots:
void slotBeginPrint();
void slotGetValue(QString paramName, QVariant &paramValue);

public:

   QSqlQueryModel* customer(){return m_customer;}
   QTableView * tableView();

   void initPrint();



};

#endif // FORMLIST_H
