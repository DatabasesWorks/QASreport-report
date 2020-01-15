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


#include "destextedit.h"
#include "ui_destextedit.h"

QDesTextEdit::QDesTextEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QDesTextEdit)
{
    ui->setupUi(this);

ui->tabEdits->setCurrentIndex(0);
ui->textEdit->setFocus();

ui->tabVariables->setCurrentIndex(0);
ui->tabEdits->setCurrentIndex(0);
ui->tabEdits->setTabEnabled(1,false);

ui->splitter->setSizes(QList<int>() << 400 << 200);

QSettings settings(QDir::currentPath() + "/settings.ini",QSettings::IniFormat);
QList<QVariant> list;
QVariant  varvalue;

settings.beginGroup("QDesTextEdit");

list = QList<QVariant>() << 400 << 200;
varvalue = settings.value("splitterGen",list);
ui->splitter->setSizes(QList<int>() << varvalue.toList()[0].toInt() << varvalue.toList()[1].toInt());
resize(settings.value("size",QSize(680,340)).toSize());
ui->butwordwrap->setChecked(settings.value("butwordwrap",true).toBool());

ui->textEdit->setWordWrapMode(ui->butwordwrap->isChecked()?QTextOption::WordWrap:QTextOption::NoWrap);
ui->scriptEdit->setWordWrapMode(ui->butwordwrap->isChecked()?QTextOption::WordWrap:QTextOption::NoWrap);

settings.endGroup();


ui->treeVars->setModel(FormSettings->Variables());
ui->treeFields->setModel(FormSettings->Fields());
ui->treeSysVars->setModel(FormSettings->SysVars());

ui->treeVars->expandAll();
ui->treeSysVars->expandAll();

QStandardItem * item;
for (int i = 0; i < FormSettings->Fields()->rowCount(); ++i) {
    item = FormSettings->Fields()->item(i);

    if(item->hasChildren())
           ui->treeFields->setExpanded(item->index(),true);
}


connect(ui->treeVars,SIGNAL(doubleClicked(QModelIndex)),SLOT(slotTreeClicked(QModelIndex)));
connect(ui->treeFields,SIGNAL(doubleClicked(QModelIndex)),SLOT(slotTreeClicked(QModelIndex)));
connect(ui->treeSysVars,SIGNAL(doubleClicked(QModelIndex)),SLOT(slotTreeClicked(QModelIndex)));


ui->splitter->setCollapsible(0,false);
ui->splitter->setCollapsible(1,true);
}

void QDesTextEdit::slotTreeClicked(const QModelIndex &index)
{
QStandardItem *item, *parentitem;
QStandardItemModel * model = (QStandardItemModel *)index.model();
QString value;
QTextEdit *edit;

item = model->itemFromIndex(index);

if(!item->parent())
    return;

if(item->hasChildren())
    return;

value = item->text();

if(ui->tabVariables->currentIndex() == 1){
    parentitem = item->parent();

    if(parentitem)
        value = "DB." + parentitem->text() + "." + value;
}

value = "[" + value + "]";

edit = ui->tabEdits->currentIndex() == 0?ui->textEdit:ui->scriptEdit;

QClipboard *clipboard = QApplication::clipboard();

clipboard->setText(value);

if(edit->canPaste())
    edit->paste();
}

void  QDesTextEdit::SetTextEdit(QString value)
{
ui->textEdit->setPlainText(value);

}

QString QDesTextEdit::TextEdit()
{
return ui->textEdit->toPlainText();

}

void  QDesTextEdit::SetScriptEdit(QString value)
{
ui->scriptEdit->setPlainText(value);

}

QString QDesTextEdit::ScriptEdit()
{
return ui->scriptEdit->toPlainText();

}


QDesTextEdit::~QDesTextEdit()
{
QSettings settings(QDir::currentPath() + "/settings.ini",QSettings::IniFormat);
QList<QVariant> list;

settings.beginGroup("QDesTextEdit");

list = QList<QVariant>() << ui->splitter->sizes()[0] << ui->splitter->sizes()[1];
settings.setValue("splitterGen",list);
settings.setValue("size",size());
settings.setValue("butwordwrap",ui->butwordwrap->isChecked());

settings.endGroup();


delete ui;
}


void QDesTextEdit::on_butwordwrap_clicked()
{
    ui->textEdit->setWordWrapMode(ui->butwordwrap->isChecked()?QTextOption::WordWrap:QTextOption::NoWrap);
    ui->scriptEdit->setWordWrapMode(ui->butwordwrap->isChecked()?QTextOption::WordWrap:QTextOption::NoWrap);
}

void QDesTextEdit::on_butvars_clicked()
{
 if(ui->butvars->isChecked())
 ui->splitter->setSizes(QList<int>() << 400 << 200);
 else
 ui->splitter->setSizes(QList<int>() << 400 << 0);

}

void QDesTextEdit::on_butfuncs_clicked()
{
QDesTreeSelect * treeselect;
QWidget * but = (QWidget *)QObject::sender();
QPoint p;

treeselect = new QDesTreeSelect(this);
treeselect->SetModel(FormSettings->Functions());

p = but->parentWidget()->mapToGlobal(but->pos() + QPoint(but->width()/2,but->height()));

connect(treeselect,SIGNAL(signalSelectValue(QString)),this,SLOT(slotSelectValue(QString)));

treeselect->resize(200,200);
treeselect->move(p);

treeselect->show();
}

void QDesTextEdit::slotSelectValue(QString value)
{
QTextEdit *edit;
QString pastvalue;

if(value.isEmpty()) return;

pastvalue = value + "()";

edit = ui->tabEdits->currentIndex() == 0?ui->textEdit:ui->scriptEdit;

QClipboard *clipboard = QApplication::clipboard();

clipboard->setText(pastvalue);

if(edit->canPaste())
    edit->paste();

}
