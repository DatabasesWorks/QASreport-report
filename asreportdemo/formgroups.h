#ifndef FORMGROUPS_H
#define FORMGROUPS_H

#include <QWidget>
#include "mainwindow.h"

namespace Ui {
class FormGroups;
}

class FormGroups : public QWidget
{
    Q_OBJECT

public:
    explicit FormGroups(QWidget *parent = 0);
    ~FormGroups();

private slots:
void slotBeginPrint();


private:
    Ui::FormGroups *ui;

    QSqlQueryModel* m_groups;

    QSqlDatabase m_db;

public:
     void initPrint();



};

#endif // FORMGROUPS_H
