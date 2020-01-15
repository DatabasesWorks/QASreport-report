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


#ifndef DESPROPINSPECTOR_H
#define DESPROPINSPECTOR_H

#include <QStandardItemModel>
#include <QPainter>
#include <QItemDelegate>
#include <QTableView>
#include <QTreeView>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QMetaProperty>
#include <QDebug>
#include <QFontDatabase>
#include "descontrols.h"
#include "despropeditor.h"
#include "dessettings.h"


class DesPropInspector;
class QDesControl;


//-----------------------------------------------------
class DesPropDelegate: public QItemDelegate
{
public:
    DesPropDelegate(QObject *parent);

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const;

    QWidget *createEditor(
            QWidget *parent,
            const QStyleOptionViewItem &option,
            const QModelIndex &index) const;

    void  setModelData(
            QWidget *editor,
            QAbstractItemModel *model,
            const QModelIndex& index) const;

 QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;

 void updateEditorGeometry(QWidget *editor,
                           const QStyleOptionViewItem &option,
                           const QModelIndex &index) const;


private:
 DesPropInspector * m_propInspector;





};

//-----------------------------------------------------------
class DesPropInspector : public QObject
{
    Q_OBJECT
public:
    DesPropInspector(QObject* parent = 0);
    virtual ~DesPropInspector() {}

private:
QStandardItemModel *m_model;

QTreeView * m_treeView;

bool m_canCnange;

QList<QDesControl*> m_selList;

DesPropDelegate * m_desPropDelegate;

void UpdatePropsList(QStandardItem *parentitem,QObject*control,bool expprop = true);
bool PropExistInSel(QString propname);
QVariant ExpectValue(QString,QVariant value);
void UpdatePropValue(QObject* control,QString propname,QVariant value,QStandardItem * parentitem = 0);
QMetaProperty seekProperty(QObject*control, QString propname);
void clearModel();
void UpdatePropsValues(QStandardItem *parentitem,QObject*control, bool expprop = true);


private slots:
void slotItemChanged(QStandardItem * item);

public slots:
void slotReadProps(QList<QDesControl*>list);
void slotCloseProps();
void slotUpdatePropValues();
void slotChangeFont();
void slotCallEditor();
void slotCallFormatEditor();

void slotValueChanged(const QString &text);
void slotStateChanged(int state);


signals:
void signalReadToolBarValues();

public:
bool isEnumType(QString propname,QString parentname = "");
QMetaEnum getEnum(QString propname,QString parentname = "");
QMetaProperty getProperty(QString propname,QString parentname);
void SetTreeView(QTreeView *);
void updateObjectPropItems(QObject *,QStandardItem *);
bool paintDelegate(QPainter *painter,const QStyleOptionViewItem &option,const QModelIndex &index);

};







#endif // DESPROPINSPECTOR_H
