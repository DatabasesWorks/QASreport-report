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


#include "despropeditor.h"

//---------------------------------------------------------
QPropButton::QPropButton(QWidget *parent):QWidget(parent)
{
    QLayout *layout = new QGridLayout();
    layout->setMargin(0);

    m_button = new QPushButton(this);
    m_button->setMaximumSize(18,18);
    m_button->setMinimumSize(18,18);
    m_button->setText("...");
    connect(m_button,SIGNAL(clicked()),this,SLOT(clickedButton()));

    ((QGridLayout*)layout)->addWidget(m_button,0,1,Qt::AlignRight);

    setLayout(layout);
    resize(100,20);

    m_change = false;
}

void QPropButton::clickedButton()
{
emit signalClicked();
}

//---------------------------------------------------------
QPropCheckBox::QPropCheckBox(QWidget *parent):QWidget(parent)
{
m_checked = false;

m_pressed = false;

resize(100,20);
}

void QPropCheckBox::paintEvent(QPaintEvent *pe)
{
    QPainter painter(this);

    QRect r = rect();

    r.setRight(r.right() - 1);

    QPen pen;
    QBrush brush;


    brush.setColor(palette().color(QPalette::Highlight));
    brush.setStyle(Qt::SolidPattern);

    painter.fillRect(r,brush);

    QStyleOptionButton option;
    option.initFrom(this);
    option.state = (m_pressed?QStyle::State_Sunken:QStyle::State_None) | QStyle::State_Active | QStyle::State_Enabled | (m_checked?QStyle::State_On:QStyle::State_Off) | QStyle::State_HasFocus;
    option.text = "";
    option.rect = r;

    QRect recti = style()->subElementRect(QStyle::SE_CheckBoxIndicator,&option,this);

    painter.translate(r.width()/2 - recti.width()/2,r.top());

    style()->drawControl(QStyle::CE_CheckBox,&option, &painter, this);
}


void QPropCheckBox::slotStateChanged(int state)
{
   emit stateChanged(state);
}

void QPropCheckBox::keyPressEvent(QKeyEvent * event)
{
    if(event->key() == Qt::Key_Space){

            m_checked = !m_checked;

            emit stateChanged(1);

        m_pressed = true;

        repaint();
    }

    QWidget::keyPressEvent(event);
}

void QPropCheckBox::keyReleaseEvent(QKeyEvent * event)
{
    if(m_pressed){
        m_pressed = false;
        repaint();
    }

    QWidget::keyReleaseEvent(event);
}


void QPropCheckBox::mousePressEvent(QMouseEvent *event)
{
    if(event->button() ==  Qt::LeftButton){

        m_checked = !m_checked;

            emit stateChanged(1);

            m_pressed = true;

            repaint();
        }

        QWidget::mousePressEvent(event);
}

void QPropCheckBox::mouseReleaseEvent(QMouseEvent *event)
{
    if(m_pressed){
        m_pressed = false;
        repaint();
    }

    QWidget::mouseReleaseEvent(event);
}

//---------------------------------------------
void ColorDelegate::paint(QPainter *painter,
           const QStyleOptionViewItem &option,
           const QModelIndex &index) const
{
QPen pen;
QBrush brush;
QRect r;

r = option.rect;

r.setLeft(r.left() + 4); //2
r.setRight(r.right() -4);//2
r.setTop(r.top() + 2);
r.setBottom(r.bottom() - 2);

pen.setColor(Qt::gray);
pen.setStyle(Qt::SolidLine);
pen.setWidth(1);
brush.setColor(QColor(index.data().toString()));
brush.setStyle(Qt::SolidPattern);
painter->setPen(pen);
painter->setBrush(brush);
painter->drawRect(r);
}

void ColorDelegate::updateEditorGeometry(QWidget *editor,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

//---------------------------------------------------------
QColorComboBox::QColorComboBox(QWidget* parent):QWidget(parent)
{
QLayout *layout = new QHBoxLayout(this);

m_combobox = new QComboBox(this);

m_button = new QPushButton(this);
m_button->setMaximumSize(18,18);
m_button->setMinimumSize(18,18);
m_button->setText("...");

layout->addWidget(m_combobox);
layout->addWidget(m_button);

layout->setMargin(0);
layout->setSpacing(1);

setLayout(layout);

QStringList list = QString(COLORLIST).split(',');

m_combobox->addItems(list);//QColor::colorNames()
m_combobox->setItemDelegate(new ColorDelegate(this));

m_combobox->installEventFilter(this);

m_color = Qt::white;

connect(m_combobox,SIGNAL(currentIndexChanged(QString)),this,SLOT(slotCurrentIndexChanged(QString)));
connect(m_button,SIGNAL(clicked()),this,SLOT(clickedButton()));

resize(150,20);
}

void QColorComboBox::slotCurrentIndexChanged(QString color)
{
m_color.setNamedColor(color);

m_combobox->update();

emit stateChanged(0);
}

void QColorComboBox::clickedButton()
{
QColor color, dialogcolor = m_color;

if(m_color == Qt::transparent) dialogcolor = Qt::white;

color = QColorDialog::getColor(dialogcolor,this);

if(color.isValid()){
    m_color = color;
    m_combobox->update();

    emit stateChanged(0);
}
}

void QColorComboBox::paintEvent(QPaintEvent *pe)
{
QPainter painter(this);
QBrush brush;
QRect r = this->rect();
QPen pen;

pen.setStyle(Qt::NoPen);

brush.setColor(palette().color(QPalette::Highlight));//brush.setColor(Qt::gray);
brush.setStyle(Qt::SolidPattern);

//r.setLeft(r.left() + 2);
//r.setRight(r.right() - 5);
//r.setTop(r.top() + 5);
//r.setBottom(r.bottom() - 5);

painter.setPen(pen);
painter.setBrush(brush);
painter.fillRect(r,brush);
}

bool QColorComboBox::eventFilter(QObject* obj, QEvent* pe)
{
    if(obj == m_combobox)
        if(pe->type() == QEvent::Paint)
        {
            QComboBox *combo = (QComboBox*)obj;
            QPainter painter(combo);
            QPen pen;
            QBrush brush;
            QRect r = combo->rect();

            r.setLeft(r.left() + 4);//2
            r.setRight(r.right() - 1);//1
            r.setTop(r.top() + 4);
            r.setBottom(r.bottom() - 5);

            pen.setColor(Qt::gray);
            pen.setStyle(Qt::SolidLine);
            pen.setWidth(1);
            brush.setColor(m_color);
            brush.setStyle(Qt::SolidPattern);
            painter.setPen(pen);
            painter.setBrush(brush);
            painter.drawRect(r);

            return true;
        }

return false;
}
