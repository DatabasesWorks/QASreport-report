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


#include "aswaitdialog.h"
#include "ui_aswaitdialog.h"

QASWaitDialog::QASWaitDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QDesWaitDialog)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
}

void QASWaitDialog::slotUpdateText(int position)
{
    ui->labelmes->setText(m_procesmessage.arg(position));
}

void QASWaitDialog::setProcessMessage(QString value)
{
    m_procesmessage = value;
}

void QASWaitDialog::setMessage(QString value)
{
    ui->labelmes->setText(value);
}

void QASWaitDialog::setEnableCancel(bool value)
{
    ui->framecancel->setVisible(value);
}

QASWaitDialog::~QASWaitDialog()
{
    delete ui;
}

void QASWaitDialog::on_butCancel_clicked()
{
    emit cancelProcess();
}
