/********************************************
*         QASReport ver.1.01                *
*           Report class                    *
*                                           *
*  Copyright (c) 2015-2016  Sergey Zgukov   *
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


#include "dessettings.h"
#include "ui_dessettings.h"

DesSettings * FormSettings;


DesSettings::DesSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DesSettings)
{
    ui->setupUi(this);

    setWindowFlags(this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);

    ui->tabWidget->removeTab(1);

QSettings settings(QDir::currentPath() + "/settings.ini",QSettings::IniFormat);

settings.beginGroup("DesSettings");
ui->checkshowgrid->setChecked(settings.value("checkshowgrid",true).toBool());
ui->checkaligngrid->setChecked(settings.value("checkaligngrid",true).toBool());
ui->radio4point->setChecked(settings.value("radio4point",true).toBool());
ui->radio8point->setChecked(settings.value("radio8point",false).toBool());
ui->radio18point->setChecked(settings.value("radio18point",false).toBool());
ui->radiomm->setChecked(settings.value("radiomm",true).toBool());
ui->radioinch->setChecked(settings.value("radioinch",false).toBool());
ui->checkeditafterinsert->setChecked(settings.value("checkeditafterinsert",true).toBool());
ui->checkshowheaders->setChecked(settings.value("checkshowheaders",true).toBool());
settings.endGroup();

m_Variables = new QStandardItemModel(this);
m_Fields = new QStandardItemModel(this);
m_SysVars = new QStandardItemModel(this);
m_Tables = new QStandardItemModel(this);
m_Functions = new QStandardItemModel(this);

m_textEditor = 0;
}

bool DesSettings::showGrid()
{
  return ui->checkshowgrid->isChecked();
}

void DesSettings::setShowGrid(bool val)
{
ui->checkshowgrid->setChecked(val);
}

bool DesSettings::alignGrid()
{
return ui->checkaligngrid->isChecked();
}

void DesSettings::setAlignGrid(bool val)
{
 ui->checkaligngrid->setChecked(val);
}

int DesSettings::sizeGrid()
{
 if(ui->radio4point->isChecked()) return 4;
 if(ui->radio8point->isChecked()) return 8;
 if(ui->radio18point->isChecked()) return 18;
}

void DesSettings::setSizeGrid(int val)
{
if(val == 4) ui->radio4point->setChecked(true); else
if(val == 8) ui->radio8point->setChecked(true); else
if(val == 18) ui->radio18point->setChecked(true); else
 ui->radio4point->setChecked(true);
}

QTypeUnit DesSettings::typeRuleMeasure()
{
 if(ui->radioinch->isChecked()) return  rmInch; else
   return rmMill;
}

void DesSettings::setTypeRuleMeasure(QTypeUnit val)
{
    if(val == rmInch)ui->radioinch->setChecked(true);
    else ui->radiomm->setChecked(true);
}

bool DesSettings::editAfterInsert()
{
    return ui->checkeditafterinsert->isChecked();
}

void DesSettings::setEditAfterInsert(bool val)
{
  ui->checkeditafterinsert->setChecked(val);
}

bool DesSettings::showBandHeaders()
{
    return ui->checkshowheaders->isChecked();
}

void DesSettings::setShowBandHeaders(bool val)
{
   ui->checkshowheaders->setChecked(val);
}


DesSettings::~DesSettings()
{
QSettings settings(QDir::currentPath() + "/settings.ini",QSettings::IniFormat);

settings.beginGroup("DesSettings");
settings.setValue("checkshowgrid",ui->checkshowgrid->isChecked());
settings.setValue("checkaligngrid",ui->checkaligngrid->isChecked());
settings.setValue("radio4point",ui->radio4point->isChecked());
settings.setValue("radio8point",ui->radio8point->isChecked());
settings.setValue("radio18point",ui->radio18point->isChecked());
settings.setValue("radiomm",ui->radiomm->isChecked());
settings.setValue("radioinch",ui->radioinch->isChecked());
settings.setValue("checkeditafterinsert",ui->checkeditafterinsert->isChecked());
settings.setValue("checkshowheaders",ui->checkshowheaders->isChecked());
settings.endGroup();

delete ui;
}

QObject * DesSettings::GetEditor(QWidget *parent,QDesType type)
{
    if(type == dtText){
        if(!m_textEditor)
            m_textEditor = new QDesTextEdit(parent);

        return m_textEditor;
    }

    return Q_NULLPTR;
}
