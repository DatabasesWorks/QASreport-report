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

#ifndef DESEDITORS_H
#define DESEDITORS_H

#include <QtWidgets>
#include "desdefdata.h"
#include "dessettings.h"


//------------------------------------------------------
class QDesAlignment : public QObject
{
    Q_OBJECT
    Q_PROPERTY(elignmentHor Horizontal READ horizontal WRITE setHorizontal)
    Q_PROPERTY(elignmentVer Vertical READ vertical WRITE setVertical)

    Q_ENUMS(elignmentHor)
    Q_ENUMS(elignmentVer)

public:
QDesAlignment() {m_horizontal = AlignLeft; m_vertical = AlignVCenter;}
QDesAlignment(const QDesAlignment &elign):QObject(){m_horizontal = elign.horizontal();m_vertical = elign.vertical(); }

virtual ~QDesAlignment() {}

enum elignmentHor{AlignLeft = 0x0001, AlignRight = 0x0002, AlignHCenter = 0x0004, AlignJustify = 0x0008};
enum elignmentVer{AlignTop = 0x0020,AlignBottom = 0x0040,AlignVCenter = 0x0080};

private:
elignmentHor m_horizontal;
elignmentVer m_vertical;
Qt::Alignment m_alignment;

public:
elignmentHor horizontal()const{return m_horizontal;}
void setHorizontal(elignmentHor val){m_horizontal = val;}
elignmentVer vertical()const{return m_vertical;}
void setVertical(elignmentVer val){m_vertical = val;}
void setAlignment(elignmentHor hor,elignmentVer ver){m_horizontal = hor;m_vertical = ver;}
void setAlignment(Qt::Alignment);
Qt::Alignment alignment()const{return (Qt::Alignment)(m_horizontal | m_vertical);}
QString GetValues();

QDesAlignment& operator =(QDesAlignment &elign){m_horizontal = elign.horizontal();m_vertical = elign.vertical(); return *this;}

bool operator == (QDesAlignment &elign){
if(m_alignment != elign.alignment()) return false;
return true;
}

bool operator != (QDesAlignment &elign){
if(m_alignment != elign.alignment()) return true;
return false;
}
};

Q_DECLARE_METATYPE(QDesAlignment)

//----------------------------------------------------------------
class QDesFont: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString FontFamily READ fontFamily WRITE SetFontFamily)
    Q_PROPERTY(int FontSize READ fontSize WRITE SetFontSize)
    Q_PROPERTY(bool FontBold READ fontBold WRITE SetFontBold)
    Q_PROPERTY(bool FontUnderline READ fontUnderline WRITE SetFontUnderline)
    Q_PROPERTY(bool FontItalic READ fontItalic WRITE SetFontItalic)
    Q_PROPERTY(QColor FontColor READ fontColor WRITE SetFontColor)

public:
    QDesFont(){m_fontFamily = "Arial"; m_fontSize = 9;m_fontBold = false, m_fontItalic = false,
               m_fontItalic = false, m_fontColor = Qt::black;}


    QDesFont(const QDesFont &font):QObject(){m_fontFamily = font.fontFamily();m_fontSize = font.fontSize();
                                   m_fontBold = font.fontBold();m_fontUnderline = font.fontUnderline();
                                   m_fontItalic = font.fontItalic(); m_fontColor = font.fontColor();}

    virtual ~QDesFont() {}


private:
QString m_fontFamily;
int m_fontSize;
bool m_fontBold;
bool m_fontUnderline;
bool m_fontItalic;
QColor m_fontColor;

public:
QString fontFamily()const{return m_fontFamily;}
void SetFontFamily(const QString value){m_fontFamily = value;}
int fontSize()const{return m_fontSize;}
void SetFontSize(int value){m_fontSize = value;}
bool fontBold()const{return m_fontBold;}
void SetFontBold(bool value){m_fontBold = value;}
bool fontUnderline()const{return m_fontUnderline;}
void SetFontUnderline(bool value){m_fontUnderline = value;}
bool fontItalic()const{return m_fontItalic;}
void SetFontItalic(bool value){m_fontItalic = value;}
QColor fontColor()const{return m_fontColor;}
void SetFontColor(QColor color){m_fontColor = color;}

QDesFont& operator = (QDesFont &font){
    m_fontFamily = font.fontFamily();m_fontSize = font.fontSize();
                                   m_fontBold = font.fontBold();m_fontUnderline = font.fontUnderline();
                                   m_fontItalic = font.fontItalic(); m_fontColor = font.fontColor();
      return *this;
}

bool operator == (QDesFont &font){
    if(m_fontFamily != font.fontFamily() ||  m_fontSize != font.fontSize() ||
    m_fontBold != font.fontBold() ||  m_fontUnderline != font.fontUnderline() ||
    m_fontItalic != font.fontItalic() ||  m_fontColor != font.fontColor())
    return false;

    return true;
}

bool operator != (QDesFont &font){
    if(m_fontFamily != font.fontFamily() ||  m_fontSize != font.fontSize() ||
    m_fontBold != font.fontBold() ||  m_fontUnderline != font.fontUnderline() ||
    m_fontItalic != font.fontItalic() ||  m_fontColor != font.fontColor())
    return true;

    return false;
}

};


Q_DECLARE_METATYPE(QDesFont)

//---------------------------------------------------------
class QDesLinePanel: public QWidget
{
    Q_OBJECT
public:
    QDesLinePanel(QWidget *parent,int linestyle);
    virtual ~QDesLinePanel() {}

private:
int m_linestyle;

int m_arr_styleline[5] = {Qt::SolidLine,
                         Qt::DashLine,
                         Qt::DotLine,
                         Qt::DashDotLine,
                         Qt::DashDotDotLine};


QPixmap DrawLine(int style);

private slots:
void clickedButton();

signals:
void signalSelectLine(int);

public:
};

//---------------------------------------------------------
class QDesColorPanel: public QWidget
{
    Q_OBJECT

public:
    QDesColorPanel(QWidget *parent,QColor color);
    virtual ~QDesColorPanel() {}

protected:

private:
QColor m_color;

QColor m_arrcol[16] = {Qt::white,Qt::black,Qt::darkMagenta ,Qt::darkGreen,
                     Qt::darkYellow,Qt::darkBlue,Qt::darkRed,Qt::darkCyan,
                     Qt::darkGray,Qt::gray,Qt::red,Qt::green,
                     Qt::yellow,Qt::blue,Qt::magenta,Qt::lightGray};


QPixmap DrawColor(QColor col);

private slots:
void clickedButton();

signals:
void signalSelectColor(QColor);

public:

};
//--------------------------------------------------------

class QDesTreeSelect: public QWidget
{
    Q_OBJECT
public:
    QDesTreeSelect(QWidget *parent);
    virtual ~QDesTreeSelect() {}


private:
QTreeView * m_tree;
bool m_expandall;

private slots:
void  slotTreeClicked(const QModelIndex &index);

signals:
void signalSelectValue(QString);

public:
void setExpandAll(bool value){m_expandall = value;}
void SetModel(QStandardItemModel * model);

};








#endif // DESEDITORS_H
