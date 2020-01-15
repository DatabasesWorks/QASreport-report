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


#ifndef QDESTEXTEDIT_H
#define QDESTEXTEDIT_H

#include <QDialog>
#include <QGridLayout>
#include <QTextEdit>
#include <QStandardItemModel>
#include <QClipboard>
#include "desdefdata.h"
#include "dessettings.h"
#include "deseditors.h"

namespace Ui {
class QDesTextEdit;
}

class QDesTextEdit : public QDialog
{
    Q_OBJECT

public:
    explicit QDesTextEdit(QWidget *parent = 0);
    ~QDesTextEdit();    


private slots:
    void  slotTreeClicked(const QModelIndex &index);
    void on_butwordwrap_clicked();
    void on_butvars_clicked();
    void on_butfuncs_clicked();
    void slotSelectValue(QString);

private:
    Ui::QDesTextEdit *ui;    




public:
void  SetTextEdit(QString);
QString TextEdit();
void  SetScriptEdit(QString);
QString ScriptEdit();



};

#endif // QDESTEXTEDIT_H
