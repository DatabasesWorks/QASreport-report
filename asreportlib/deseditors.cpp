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


#include "deseditors.h"

//-----------------------------------------------------------------------
QString QDesAlignment::GetValues()
{
QString horvalue, vervalue;

switch (m_horizontal) {
case AlignLeft: horvalue = "AlignLeft";  break;
case AlignRight: horvalue = "AlignRight";  break;
case AlignHCenter: horvalue = "AlignHCenter"; break;
}

switch (m_vertical){
case AlignTop: vervalue = "AlignTop";  break;
case AlignBottom: vervalue = "AlignBottom";  break;
case AlignVCenter: vervalue = "AlignVCenter"; break;
}

return horvalue + "|" + vervalue;
}

void QDesAlignment::setAlignment(Qt::Alignment value)
{
if(value & AlignLeft) m_horizontal =  AlignLeft;
if(value & AlignRight) m_horizontal =  AlignRight;
if(value & AlignHCenter) m_horizontal =  AlignHCenter;
if(value & AlignJustify) m_horizontal =  AlignJustify;

if(value & AlignTop) m_vertical =  AlignTop;
if(value & AlignBottom) m_vertical =  AlignBottom;
if(value & AlignVCenter) m_vertical =  AlignVCenter;

m_alignment = value;
}

//------------------------------------------------------------------------------
QDesLinePanel::QDesLinePanel(QWidget *parent,int linestyle):QWidget(parent)
{
QPushButton* button;
QVBoxLayout * layout;

setWindowFlags(windowFlags() | Qt::Popup);
setAttribute( Qt::WA_DeleteOnClose, true);

resize(120,90);

m_linestyle =  linestyle;

layout = new QVBoxLayout(this);

for (int r = 0; r < 5; ++r){
    button = new QPushButton(this);
    button->setObjectName("but" + QString::number(r));

    button->setCheckable(true);

    layout->addWidget(button);

    button->setIconSize(QSize(100,10));

    button->setIcon(QIcon(DrawLine(m_arr_styleline[r])));

    if(m_arr_styleline[r] == m_linestyle){ button->setChecked(true);}

    connect(button,SIGNAL(clicked()),this,SLOT(clickedButton()));
}

layout->setSpacing(1);
layout->setMargin(4);
setLayout(layout);
}

void QDesLinePanel::clickedButton()
{
QObject* button = QObject::sender();
QString name = button->objectName();
int index;

name = name.mid(3);
index = name.toInt();

m_linestyle = m_arr_styleline[index];

emit signalSelectLine(m_linestyle);

close();
}

QPixmap QDesLinePanel::DrawLine(int style)
{
QPainter painter;
QPixmap pixmap(QSize(100,2));
QRect rect = pixmap.rect();

painter.begin(&pixmap);

QBrush brush;
QPen pen;

pen.setColor(Qt::black);
pen.setStyle((Qt::PenStyle)style);
pen.setWidth(2);

brush.setColor(Qt::white);
brush.setStyle(Qt::SolidPattern);

painter.setPen(pen);
painter.setBrush(brush);

painter.fillRect(rect,brush);
painter.drawLine(QPoint(0,pixmap.height()/2),QPoint(pixmap.width(),pixmap.height()/2));

painter.end();

return pixmap;
}


//----------------------------------------------------------------------------
QDesColorPanel::QDesColorPanel(QWidget *parent,QColor color):QWidget(parent)
{
QPushButton* button;
QGridLayout * layout;
bool defcolor = false;

setWindowFlags(windowFlags() | Qt::Popup);
setAttribute( Qt::WA_DeleteOnClose, true);

resize(120,120);

m_color = color;

layout = new QGridLayout(this);

for (int r = 0, col = 0; r < 4; ++r)
    for (int c = 0; c < 4; ++c){

        button = new QPushButton(this);
        button->setObjectName("but" + QString::number(col));

        button->setCheckable(true);

        layout->addWidget(button,r,c);

        button->setIcon(QIcon(DrawColor(m_arrcol[col])));

        if(m_arrcol[col] == m_color){ button->setChecked(true);defcolor = true;}

        connect(button,SIGNAL(clicked()),this,SLOT(clickedButton()));

        ++col;
}

   button = new QPushButton("transparent");
   button->setCheckable(true);
   button->setObjectName("transparent");
   if(m_color == Qt::transparent) {button->setChecked(true); defcolor = true;}

   connect(button,SIGNAL(clicked()),this,SLOT(clickedButton()));

   layout->addWidget(button,4,0,1,4);


   button = new QPushButton("select..");
   button->setCheckable(true);
   button->setObjectName("selcolor");

   if(!defcolor) {button->setChecked(true); button->setIcon(QIcon(DrawColor(m_color)));}

   connect(button,SIGNAL(clicked()),this,SLOT(clickedButton()));

   layout->addWidget(button,5,0,1,4);

   layout->setSpacing(1);
   layout->setMargin(4);
   setLayout(layout);
}

void QDesColorPanel::clickedButton()
{
QObject* button = QObject::sender();
QString name = button->objectName();
int index;
QColor color, dialogcolor;

if(name == "transparent")
 m_color = Qt::transparent;
else
if(name == "selcolor"){
    dialogcolor = m_color;

    if(m_color == Qt::transparent) dialogcolor = Qt::white;

    color = QColorDialog::getColor(dialogcolor);

    if(color.isValid())
            m_color = color;
}
else
{
 name =  name.mid(3);
 index = name.toInt();

 m_color =  m_arrcol[index];
}

emit signalSelectColor(m_color);

close();
}

QPixmap QDesColorPanel::DrawColor(QColor col)
{
QPainter painter;
QPixmap pixmap(QSize(50,40));

painter.begin(&pixmap);

QBrush brush;
QPen pen;
QRect rect = pixmap.rect();

pen.setColor(Qt::gray);
pen.setStyle(Qt::NoPen);
pen.setWidth(1);

brush.setColor(col);
brush.setStyle(Qt::SolidPattern);

painter.setPen(pen);
painter.setBrush(brush);

painter.drawRect(rect);

painter.end();

return pixmap;
}

//-----------------------------------------------------------------------
QDesTreeSelect::QDesTreeSelect(QWidget *parent):QWidget(parent)
{
QVBoxLayout * layout;

m_tree = new QTreeView(this);
m_tree->setEditTriggers(QAbstractItemView::NoEditTriggers);
m_tree->setHeaderHidden(true);

connect(m_tree,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(slotTreeClicked(QModelIndex)));

setWindowFlags(windowFlags() | Qt::Popup);
setAttribute( Qt::WA_DeleteOnClose, true);

layout = new  QVBoxLayout();
layout->setMargin(2);

layout->addWidget(m_tree);

setLayout(layout);

m_expandall = true;
}

void QDesTreeSelect::SetModel(QStandardItemModel * model)
{
 m_tree->setModel(model);

 if(m_expandall)
        m_tree->expandAll();
}

void QDesTreeSelect::slotTreeClicked(const QModelIndex &index)
{
QString value;
QStandardItem *item;

if(!m_tree->model()) return;
if(!m_tree->model()->rowCount()) return;

item = ((QStandardItemModel*)m_tree->model())->itemFromIndex(index);

if(item->hasChildren()) return;

value = item->text();

emit signalSelectValue(value);

close();
}





