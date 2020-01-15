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


#ifndef DESSETTINGS_H
#define DESSETTINGS_H

#include <QDialog>
#include <QStandardItemModel>
#include "desdefdata.h"
#include "destextedit.h"

class QDesTextEdit;

namespace Ui {
class DesSettings;
}

class DesSettings : public QDialog
{
    Q_OBJECT

public:
    explicit DesSettings(QWidget *parent = 0);
    ~DesSettings();

private:
    Ui::DesSettings *ui;

    QStandardItemModel *m_Tables;
    QStandardItemModel *m_Variables;
    QStandardItemModel *m_Fields;
    QStandardItemModel *m_SysVars;
    QStandardItemModel *m_Functions;

    QDesTextEdit* m_textEditor;

signals:
    void signalShowBandHeaders();
    void signalUpdateRules();

public:

    bool showGrid();
    void setShowGrid(bool val);
    bool alignGrid();
    void setAlignGrid(bool val);
    int sizeGrid();
    void setSizeGrid(int val);
    QTypeUnit typeRuleMeasure();
    void setTypeRuleMeasure(QTypeUnit val);
    bool editAfterInsert();
    void setEditAfterInsert(bool val);
    bool showBandHeaders();
    void setShowBandHeaders(bool val);
    QStandardItemModel *Variables(){return m_Variables;}
    QStandardItemModel *Fields(){return m_Fields;}
    QStandardItemModel *SysVars(){return m_SysVars;}
    QStandardItemModel * Tables(){return m_Tables;}
    QStandardItemModel *Functions(){return m_Functions;}
    QObject * GetEditor(QWidget  *parent, QDesType type);
};

extern DesSettings * FormSettings;

#endif // DESSETTINGS_H
