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


#ifndef DESPROPEDITOR_H
#define DESPROPEDITOR_H

#include <QComboBox>
#include <QPainter>
#include <QPushButton>
#include <QColorDialog>
#include <QItemDelegate>
#include <QLayout>
#include <QStandardItemModel>
#include <QCheckBox>
#include <QEvent>
#include <QFontDialog>
#include "descontrols.h"
#include "deseditors.h"


//-------------------------------------------------------
class QPropButton: public QWidget
{
    Q_OBJECT
public:
    QPropButton(QWidget *parent);
    virtual ~QPropButton() {}

private:
QPushButton * m_button;
bool m_change;
QVariant m_value;

signals:
void signalClicked();

private slots:
void clickedButton();

public:
QVariant Value(){return m_value;}
void setValue(QVariant val){m_value = val; m_change = true;}
bool change(){return m_change;}


};



//---------------------------------------------------------
class QPropCheckBox: public QWidget
{
    Q_OBJECT
public:
    QPropCheckBox(QWidget *parent);
    virtual ~QPropCheckBox() {}

private:
bool m_checked;
bool m_pressed;

protected:
virtual void paintEvent(QPaintEvent *pe);
virtual void mousePressEvent(QMouseEvent *);
virtual void mouseReleaseEvent(QMouseEvent *);
virtual void keyPressEvent(QKeyEvent *);
virtual void keyReleaseEvent(QKeyEvent *);

private slots:
void slotStateChanged(int state);

signals:
void stateChanged(int state);

public:
bool isChecked(){return m_checked;}
void setChecked(bool value){m_checked = value;}

};
//-----------------------------------------------------------
class ColorDelegate: public QItemDelegate
{
public:
    ColorDelegate(QObject *parent):QItemDelegate(parent){}

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const;



 void updateEditorGeometry(QWidget *editor,
                           const QStyleOptionViewItem &option,
                           const QModelIndex &index) const;



private:

};

//---------------------------------------------
class QColorComboBox: public QWidget
{
    Q_OBJECT
public:
    QColorComboBox(QWidget* parent = 0);
    virtual ~QColorComboBox() {}


private:
QColor m_color;

QComboBox *m_combobox;
QPushButton * m_button;

protected:
virtual bool eventFilter(QObject*, QEvent*);
virtual void paintEvent(QPaintEvent *pe);

private slots:
void slotCurrentIndexChanged(QString);
void clickedButton();

signals:
void stateChanged(int state);
public:

QColor Color(){return m_color;}
void SetColor(QColor color){m_color = color; m_combobox->update();}


};















#endif // DESPROPEDITOR_H
