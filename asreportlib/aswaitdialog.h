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

#ifndef QDESWAITDIALOG_H
#define QDESWAITDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QThread>

namespace Ui {
class QDesWaitDialog;
}

class QASWaitDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QASWaitDialog(QWidget *parent = 0);
    ~QASWaitDialog();


private slots:
   void slotUpdateText(int);

   void on_butCancel_clicked();

private:
    Ui::QDesWaitDialog *ui;

    QString m_procesmessage;


signals:
 void cancelProcess();

public:

    void setProcessMessage(QString value);
    void setMessage(QString);
    void setEnableCancel(bool value);

};

#endif // QDESWAITDIALOG_H
