/********************************************
*         QASReport ver.1.01                *
*           Report class                    *
*                                           *
*  Copyright (c) 2015-2016  Sergey Zgukov   *
*   E-mail:  serg_@ukr.net                  *
********************************************/
/*
 This library is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 You should have received a copy of the GNU General Public License
 along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef QDESVARIABLES_H
#define QDESVARIABLES_H

#include <QDialog>
#include <QStandardItemModel>
#include <QKeyEvent>
#include "dessettings.h"
#include "desdialogs.h"

namespace Ui {
class QDesVariables;
}

class QDesVariables : public QDialog
{
    Q_OBJECT

public:
    explicit QDesVariables(QWidget *parent = 0);
    ~QDesVariables();

private:
    Ui::QDesVariables *ui;

QStandardItemModel *m_modelvars;

public:


private slots:
void on_butaddvar_clicked();
void on_butaddgroup_clicked();
void on_butdelete_clicked();
void on_buttext_clicked();
void on_butedit_clicked();
};

#endif // QDESVARIABLES_H
