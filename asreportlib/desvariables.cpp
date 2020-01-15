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


#include "desvariables.h"
#include "ui_desvariables.h"

QDesVariables::QDesVariables(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QDesVariables)
{
    ui->setupUi(this);

m_modelvars = FormSettings->Variables();

ui->treevariables->setModel(m_modelvars);
ui->treevariables->expandAll();

}

QDesVariables::~QDesVariables()
{
    delete ui;
}


void QDesVariables::on_butaddvar_clicked()
{
QStandardItem *item, *selitem, *parent;
QModelIndex index;

if(!m_modelvars->rowCount()) return;

index = ui->treevariables->currentIndex();

selitem = m_modelvars->itemFromIndex(index);

if(!selitem)
    parent =  m_modelvars->item(0);
else{
    if(!selitem->parent())
          parent = selitem;
    else
          parent = selitem->parent();
}

item = new QStandardItem("New variable " + QString::number(parent->rowCount() + 1));
parent->appendRow(item);

ui->treevariables->setExpanded(parent->index(), true);
}



void QDesVariables::on_butaddgroup_clicked()
{
QStandardItem *item;

item = new QStandardItem("New group " + QString::number(m_modelvars->rowCount() + 1));
m_modelvars->appendRow(item);
}

void QDesVariables::on_butdelete_clicked()
{
QStandardItem *item;
QModelIndex index;

index = ui->treevariables->currentIndex();

if(!index.isValid()) return;

item = m_modelvars->itemFromIndex(index);

if(!item) return;


if(!item->parent())
        m_modelvars->removeRow(item->row());
else
   item->parent()->removeRow(item->row());


}

void QDesVariables::on_buttext_clicked()
{
QDesTextEditor texteditor(this);
QStringList list;
QStandardItem *item = 0, *child = 0, *parent = 0;
QString str;

list.clear();
for (int i = 0; i < m_modelvars->rowCount(); ++i) {
    item = m_modelvars->item(i);

    list.append(item->text());

    if(item->hasChildren())
    {
        for (int c = 0; c < item->rowCount(); ++c) {
            child = item->child(c);

            list.append(" " + child->text());
        }
     }
 }

texteditor.SetStrings(list);

texteditor.exec();

if(texteditor.result() == QDialog::Accepted)
{
    m_modelvars->clear();

    list = texteditor.Strings();

    foreach (QString value, list){
        str = value.simplified();
        item = new QStandardItem(str);

        if(value.at(0) != ' '){
            m_modelvars->appendRow(item);
            parent = item;
        } else
        {
            if(!parent)
                m_modelvars->appendRow(item);
            else
                parent->appendRow(item);
        }
    }

    ui->treevariables->expandAll();
}

}

void QDesVariables::on_butedit_clicked()
{
QModelIndex index;
QKeyEvent* pePress;

index = ui->treevariables->currentIndex();

if(!index.isValid()) return;

pePress =  new QKeyEvent(QEvent::KeyPress, Qt::Key_F2, Qt::NoModifier);

QApplication::sendEvent(ui->treevariables, pePress);
}
