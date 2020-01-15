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

#ifndef DESCONTROLS_H
#define DESCONTROLS_H

#include <QPainter>
#include <QWidget>
#include "designpage.h"
#include "asreport_p.h"
#include "desdefdata.h"
#include "destextedit.h"
#include "desdialogs.h"
#include "despropeditor.h"
#include "deseditors.h"
#include "dessettings.h"
#include "../barcode/barcode.h"

class QDesControl;
class QDesBand;




//------------------------------------------
class QFocusControl: public QWidget
{
    Q_OBJECT

friend class QDesControl;

public:
QFocusControl(QWidget *parent, QDesControl *control);
virtual ~QFocusControl(){}

private:
QDesControl *m_Control;

int m_psize;
bool m_MultSel;

QPoint m_begpoint;

QTypeSizing m_TypeSizing;

QRect  m_rects[8];

bool m_hideonmove;

protected:
virtual void paintEvent(QPaintEvent *pe);
virtual void mousePressEvent(QMouseEvent *event);
virtual void mouseMoveEvent(QMouseEvent *event);
virtual void mouseReleaseEvent(QMouseEvent *);


public:

void UpdateGeometry();
void SetMultSel(bool mult){m_MultSel = mult;}

private slots:

signals:

};

//------------------------------------------
class QDesControl: public QWidget
{
    Q_OBJECT

public:
    QDesControl(QWidget *parent=0);
    virtual ~QDesControl();

   Q_FLAGS(BorderType)

    enum BorderFlag{Left = 1,Right = 2,Top = 4,Bottom = 8};
    Q_DECLARE_FLAGS(BorderType, BorderFlag)

private:
QString m_Name;
QDesType m_Type;

QPoint m_begpoint;
bool m_Selected;
bool m_MultSel;

QFocusControl * m_focusControl;

QDesControl *m_parentBand;

protected:
QPainter m_Painter;

QString m_subProps;

QStandardItem *m_standardItem;
QMenu *m_serviceMenu;

virtual void paintEvent(QPaintEvent *pe);
virtual void mousePressEvent(QMouseEvent *event);
virtual void mouseReleaseEvent(QMouseEvent *);
virtual void mouseMoveEvent(QMouseEvent *event);
virtual void moveEvent(QMoveEvent *);
virtual void resizeEvent(QResizeEvent *);
virtual void mouseDoubleClickEvent(QMouseEvent *);
virtual void keyPressEvent(QKeyEvent *);
virtual void UpdateSize(){}

void DrawCorner();

signals:
void signalEditField();
void signalSelectControl(QDesControl*control,bool mult);
void signalUnSelectControl(QDesControl*control);
void signalBeginSelection(QPoint);
void signalStartMove();
void signalEndMove();
void signalSaveUndo(QTypeUndoAction,QString);

private slots:
void slotEndMouseProcess();
void slotBeginMouseProcess();
void slotSetMultSel(bool);
void slotObjectNameChanged(QString name);

public slots:
void slotParentResized();


public:
virtual bool CallEditor(){return false;}

QString Name(){return m_Name;}
void SetName(QString name){m_Name = name;}
void SetWidth(int val){resize(val,height());}
void SetHeight(int val){resize(width(),val);}
void SetLeft(int val){move(val,y());}
void SetTop(int val){move(x(),val);}
QDesType Type(){return m_Type;}
void setType(int type ){m_Type = (QDesType)type;}
bool Selected(){return m_Selected;}
void SetSelect(bool);
QPoint BegPoint(){return m_begpoint;}
void SetHideSelection(bool);
QFocusControl * FocusControl(){return m_focusControl;}
QDesControl *ParentBand(){return m_parentBand;}
void SetParentBand(QDesControl *band){m_parentBand = band;}
QStandardItem *standardItem(){return m_standardItem;}
QString subProps(){return m_subProps;}
void SetServiceMenu(QMenu * menu){m_serviceMenu = menu;}

};

//-----------------------------------------

class QDesText : public QDesControl
{
    Q_OBJECT

    Q_PROPERTY(QString Name READ objectName WRITE setObjectName)
    Q_PROPERTY(int type READ Type)
    Q_PROPERTY(int Width READ width WRITE SetWidth)
    Q_PROPERTY(int Height READ height WRITE SetHeight)
    Q_PROPERTY(int Left READ x WRITE SetLeft)
    Q_PROPERTY(int Top READ y WRITE SetTop)
    Q_PROPERTY(QString Text READ Text WRITE SetText)
    Q_PROPERTY(QColor BackColor READ backColor WRITE SetBackColor)
    Q_PROPERTY(QColor FontColor READ fontColor WRITE SetFontColor)
    Q_PROPERTY(QColor BorderColor READ borderColor WRITE SetBorderColor)
    Q_PROPERTY(int BorderWidth READ borderWidth WRITE SetBorderWidth)
    Q_PROPERTY(BorderType Border READ border WRITE SetBorder)
    Q_PROPERTY(Qt::PenStyle BorderStyle READ borderStyle WRITE SetBorderStyle)
    Q_PROPERTY(int TextAlignment READ textAlignment WRITE SetTextAlignment)
    Q_PROPERTY(QDesAlignment Alignment READ desAlignment WRITE SetDesAlignment)
    Q_PROPERTY(QString FontFamily READ fontFamily WRITE SetFontFamily)
    Q_PROPERTY(int FontSize READ fontSize WRITE SetFontSize)
    Q_PROPERTY(bool FontBold READ fontBold WRITE SetFontBold)
    Q_PROPERTY(bool FontUnderline READ fontUnderline WRITE SetFontUnderline)
    Q_PROPERTY(bool FontItalic READ fontItalic WRITE SetFontItalic)
    Q_PROPERTY(QDesFont Font READ font WRITE SetFont)
    Q_PROPERTY(bool WordWrap READ wordWrap WRITE SetWordWrap)
    Q_PROPERTY(bool Stretchable READ stretchable WRITE SetStretchable)
    Q_PROPERTY(QString Format READ Format WRITE SetFormat)
    Q_PROPERTY(TypeAngle AngleType READ AngleType WRITE setAngleType)

public:
    QDesText(QWidget *parent=0);
    virtual ~QDesText(){}

    Q_ENUMS(TypeAngle)
    enum TypeAngle{Horizontal = 0,Vertical = 1};

private:
QString m_Text;

QColor m_backColor;
QColor m_fontColor;
QColor m_borderColor;

Qt::Alignment m_alignment;

BorderType m_border;
Qt::PenStyle m_borderstyle;
int m_borderwidth;

QString m_fontFamily;
int m_fontSize;
bool m_fontBold;
bool m_fontUnderline;
bool m_fontItalic;

bool m_wordwrap;
bool m_stretchable;

QString m_format;

QDesFont m_font;
QDesAlignment m_desalignment;
TypeAngle m_texttypeangle;

protected:
virtual void paintEvent(QPaintEvent *pe);

public:
bool CallEditor();

QColor backColor(){return m_backColor;}
void SetBackColor(QColor color){m_backColor = color;}
QColor fontColor(){return m_fontColor;}
void SetFontColor(QColor color){m_fontColor = color;m_font.SetFontColor(m_fontColor);}
int borderWidth(){return m_borderwidth;}
void SetBorderWidth(int value){m_borderwidth = value;}
QColor borderColor(){return m_borderColor;}
void SetBorderColor(QColor color){m_borderColor = color;}
int textAlignment(){return m_alignment;}
void SetTextAlignment(int value){m_alignment = (Qt::Alignment)value; m_desalignment.setAlignment((Qt::Alignment)value);}
BorderType border(){return m_border;}
void SetBorder(BorderType value){m_border = value;}
Qt::PenStyle borderStyle()const{return m_borderstyle;}
void SetBorderStyle(const Qt::PenStyle value){m_borderstyle = value;}
QString fontFamily(){return m_fontFamily;}
void SetFontFamily(QString value){m_fontFamily = value; m_font.SetFontFamily(m_fontFamily);}
int fontSize(){return m_fontSize;}
void SetFontSize(int value){m_fontSize = value;m_font.SetFontSize(m_fontSize);}
bool fontBold(){return m_fontBold;}
void SetFontBold(bool value){m_fontBold = value;m_font.SetFontBold(m_fontBold);}
bool fontUnderline(){return m_fontUnderline;}
void SetFontUnderline(bool value){m_fontUnderline = value;m_font.SetFontUnderline(m_fontUnderline);}
bool fontItalic(){return m_fontItalic;}
void SetFontItalic(bool value){m_fontItalic = value;m_font.SetFontItalic(m_fontItalic);}
QString Text(){return m_Text;}
void SetText(QString value){m_Text = value; update();}
bool wordWrap(){return m_wordwrap;}
void SetWordWrap(bool val){m_wordwrap = val; update();}
bool stretchable(){return m_stretchable;}
void SetStretchable(bool val){m_stretchable = val;}
QString Format(){return m_format;}
void SetFormat(QString val){m_format = val; update();}
QDesFont &font(){return m_font;}
void SetFont(QDesFont &font){m_font = font;m_fontFamily = m_font.fontFamily();m_fontSize = m_font.fontSize();m_fontUnderline = m_font.fontUnderline();
                            m_fontItalic = m_font.fontItalic();m_fontBold = m_font.fontBold();m_fontColor = m_font.fontColor();}
QDesAlignment &desAlignment(){return m_desalignment;}
void SetDesAlignment(QDesAlignment &value){m_desalignment = value;m_alignment = m_desalignment.alignment();}
TypeAngle AngleType(){return m_texttypeangle;}
void setAngleType(TypeAngle type){m_texttypeangle = type;}


};
//-------------------------------------------------------------------

class QDesLine: public QDesControl
{
    Q_OBJECT

    Q_PROPERTY(QString Name READ objectName WRITE setObjectName)
    Q_PROPERTY(int type READ Type)
    Q_PROPERTY(int Width READ width WRITE SetWidth)
    Q_PROPERTY(int Height READ height WRITE SetHeight)
    Q_PROPERTY(int Left READ x WRITE SetLeft)
    Q_PROPERTY(int Top READ y WRITE SetTop)
    Q_PROPERTY(QColor BorderColor READ borderColor WRITE SetBorderColor)
    Q_PROPERTY(int BorderWidth READ borderWidth WRITE SetBorderWidth)
    Q_PROPERTY(Qt::PenStyle BorderStyle READ borderStyle WRITE SetBorderStyle)

public:
    QDesLine(QWidget *parent = 0);
    virtual ~QDesLine() {}

private:
QColor m_borderColor;
int m_borderwidth;
Qt::PenStyle m_borderstyle;

QPoint m_begpoint;
QPoint m_endpoint;

void UpdateGeometry();


protected:
virtual void paintEvent(QPaintEvent *pe);
virtual void resizeEvent(QResizeEvent *);

public:

QColor borderColor(){return m_borderColor;}
void SetBorderColor(QColor color){m_borderColor = color;}
int borderWidth(){return m_borderwidth;}
void SetBorderWidth(int val){m_borderwidth = val; UpdateGeometry();}
Qt::PenStyle borderStyle()const{return m_borderstyle;}
void SetBorderStyle(const Qt::PenStyle value){m_borderstyle = value;}

};


//-------------------------------------------------------------------
class QDesFigure: public QDesControl
{
    Q_OBJECT

    Q_PROPERTY(QString Name READ objectName WRITE setObjectName)
    Q_PROPERTY(int type READ Type)
    Q_PROPERTY(int Width READ width WRITE SetWidth)
    Q_PROPERTY(int Height READ height WRITE SetHeight)
    Q_PROPERTY(int Left READ x WRITE SetLeft)
    Q_PROPERTY(int Top READ y WRITE SetTop)
    Q_PROPERTY(QColor BorderColor READ borderColor WRITE SetBorderColor)
    Q_PROPERTY(int BorderWidth READ borderWidth WRITE SetBorderWidth)
    Q_PROPERTY(Qt::PenStyle BorderStyle READ borderStyle WRITE SetBorderStyle)
    Q_PROPERTY(QColor BackColor READ backColor WRITE SetBackColor)
    Q_PROPERTY(TypeFigure FigureType READ figureType WRITE SetFigureType)

public:
    QDesFigure(QWidget *parent);
    virtual ~QDesFigure() {}


    Q_ENUMS(TypeFigure)
    enum TypeFigure{Square = 0,Triangle = 1,Ellipse = 2,RoundedRect = 3};

private:
TypeFigure  m_figureType;
QColor m_borderColor;
QColor m_backColor;
int m_borderwidth;
Qt::PenStyle m_borderstyle;

private slots:


protected:
virtual void paintEvent(QPaintEvent *pe);

public:
bool CallEditor();
void SetFigureType(TypeFigure type){m_figureType = type;}
TypeFigure figureType(){return m_figureType;}
QColor borderColor(){return m_borderColor;}
void SetBorderColor(QColor color){m_borderColor = color;}
int borderWidth(){return m_borderwidth;}
void SetBorderWidth(int val){m_borderwidth = val;}
Qt::PenStyle borderStyle()const{return m_borderstyle;}
void SetBorderStyle(const Qt::PenStyle value){m_borderstyle = value;}
QColor backColor(){return m_backColor;}
void SetBackColor(QColor color){m_backColor = color;}
};

//------------------------------------------------------
class QDesImage : public QDesControl
{
    Q_OBJECT

    Q_PROPERTY(QString Name READ objectName WRITE setObjectName)
    Q_PROPERTY(int type READ Type)
    Q_PROPERTY(int Width READ width WRITE SetWidth)
    Q_PROPERTY(int Height READ height WRITE SetHeight)
    Q_PROPERTY(int Left READ x WRITE SetLeft)
    Q_PROPERTY(int Top READ y WRITE SetTop)
    Q_PROPERTY(QColor BorderColor READ borderColor WRITE SetBorderColor)
    Q_PROPERTY(int BorderWidth READ borderWidth WRITE SetBorderWidth)
    Q_PROPERTY(Qt::PenStyle BorderStyle READ borderStyle WRITE SetBorderStyle)
    Q_PROPERTY(BorderType Border READ border WRITE SetBorder)
    Q_PROPERTY(QPixmap Picture READ picture WRITE setPicture)
    Q_PROPERTY(QString Source READ source WRITE setSource)

public:
    QDesImage(QWidget *parent);
    virtual ~QDesImage() {}

private:
QPixmap m_pixmap;

QColor m_borderColor;
int m_borderwidth;
Qt::PenStyle m_borderstyle;
BorderType m_border;
QString m_source;

protected:
virtual void paintEvent(QPaintEvent *pe);

public:
bool CallEditor();

QColor borderColor(){return m_borderColor;}
void SetBorderColor(QColor color){m_borderColor = color;}
int borderWidth(){return m_borderwidth;}
void SetBorderWidth(int val){m_borderwidth = val;}
Qt::PenStyle borderStyle()const{return m_borderstyle;}
void SetBorderStyle(const Qt::PenStyle value){m_borderstyle = value;}
BorderType border(){return m_border;}
void SetBorder(BorderType value){m_border = value;}
QPixmap picture(){return m_pixmap;}
void setPicture(QPixmap pix){m_pixmap = pix;}
QString source(){return m_source;}
void setSource(QString value){m_source = value;}


};

//--------------------------------------------------------------------
class QHeadBand : public QWidget
{
    Q_OBJECT
public:
    QHeadBand(QWidget *parent,QDesBand*band);
    virtual ~QHeadBand() {}

private:

QDesBand *m_Band;
QString m_caption;

QPoint m_begpoint;

protected:
virtual void paintEvent(QPaintEvent *pe);
virtual void mousePressEvent(QMouseEvent *event);
virtual void mouseMoveEvent(QMouseEvent *event);
virtual void moveEvent(QMoveEvent *);
virtual void mouseReleaseEvent(QMouseEvent *);

private slots:
void slotEndMouseProcess();
void slotBeginMouseProcess();

public:
void SetCaption(QString val){m_caption = val;}
QString Caption(){return m_caption;}

};




//------------------------------------------
class QDesBand : public QDesControl
{
    Q_OBJECT

    Q_PROPERTY(QString Name READ objectName WRITE setObjectName)
    Q_PROPERTY(int Height READ height WRITE SetHeight)
    Q_PROPERTY(int Top READ y WRITE SetTop)
    Q_PROPERTY(int BandType READ bandType)
    Q_PROPERTY(QString Source READ source WRITE setSource)
    Q_PROPERTY(bool Stretchable READ stretchable WRITE SetStretchable)

friend class QHeadBand;

public:
    QDesBand(QWidget *parent, QBandType type);
    virtual ~QDesBand();

private:
QBandType m_BandType;

QHeadBand *m_HeadBand;

bool m_stretchable;
bool m_showheader;

QString m_source;

private slots:
void slotObjectNameChanged(QString);
void slotShowBandHeaders();

protected:
virtual void paintEvent(QPaintEvent *pe);
virtual void moveEvent(QMoveEvent *);
virtual void mouseMoveEvent(QMouseEvent *event);
virtual void UpdateSize();

public:
bool CallEditor();
void SetShowheader(bool val){m_showheader = val; m_HeadBand->setVisible(m_showheader); update();}
QBandType bandType(){return m_BandType;}
void StackUnder(QDesControl*);
void Lower();
bool stretchable(){return m_stretchable;}
void SetStretchable(bool val){m_stretchable = val;}
QString source(){return m_source;}
void setSource(QString value){m_source = value;}

};

//---------------------------------------------------------------------
class QDesBarCode: public QDesControl
{
    Q_OBJECT

    Q_PROPERTY(QString Name READ objectName WRITE setObjectName)
    Q_PROPERTY(int type READ Type)
    Q_PROPERTY(int Width READ width WRITE SetWidth)
    Q_PROPERTY(int Height READ height WRITE SetHeight)
    Q_PROPERTY(int Left READ x WRITE SetLeft)
    Q_PROPERTY(int Top READ y WRITE SetTop)
    Q_PROPERTY(BarcodeType BarcodeType READ barcodeType WRITE setBarcodeType)
    Q_PROPERTY(QString Text READ Text WRITE setText)
    Q_PROPERTY(bool Checksum READ Checksum WRITE setChecksum)
    Q_PROPERTY(bool ShowText READ ShowText WRITE setShowText)
    Q_PROPERTY(double BarcodeAngle READ Angle WRITE setAngle)
    Q_PROPERTY(double Ratio READ Ratio WRITE setRatio)
    Q_PROPERTY(QColor BorderColor READ borderColor WRITE SetBorderColor)
    Q_PROPERTY(int BorderWidth READ borderWidth WRITE SetBorderWidth)
    Q_PROPERTY(Qt::PenStyle BorderStyle READ borderStyle WRITE SetBorderStyle)
    Q_PROPERTY(BorderType Border READ border WRITE SetBorder)

public:
    QDesBarCode(QWidget *parent);
    virtual ~QDesBarCode() {}

    enum BarcodeType{Code_2_5_interleaved,
                     Code_2_5_industrial,
                     Code_2_5_matrix,
                     Code39,
                     Code39Extended,
                     Code128A,
                     Code128B,
                     Code128C,
                     Code93,
                     Code93Extended,
                     CodeMSI,
                     CodePostNet,
                     CodeCodabar,
                     CodeEAN8,
                     CodeEAN13};

    Q_ENUMS(BarcodeType)


private:
QPixmap m_pixmap;
QBarcode *m_Barcode;

QString m_Text;
bool m_checkSum;
bool m_showText;
BarcodeType m_barcodetype;
double m_angle;
double m_ratio;
int m_pixwidth;
int m_pixheight;

QColor m_borderColor;
int m_borderwidth;
Qt::PenStyle m_borderstyle;
BorderType m_border;

protected:
virtual void paintEvent(QPaintEvent *pe);
virtual void resizeEvent(QResizeEvent *);

public:
bool CallEditor();
void rePaint();

QString Text(){return m_Text;}
void setText(QString value);
bool Checksum(){return m_checkSum;}
void setChecksum(bool value);
bool ShowText(){return m_showText;}
void setShowText(bool value);
BarcodeType barcodeType(){return m_barcodetype;}
void setBarcodeType(BarcodeType value);
double Angle(){return m_angle;}
void setAngle(double value);
double Ratio(){return m_ratio;}
void setRatio(double value);

QColor borderColor(){return m_borderColor;}
void SetBorderColor(QColor color){m_borderColor = color;}
int borderWidth(){return m_borderwidth;}
void SetBorderWidth(int val){m_borderwidth = val;}
Qt::PenStyle borderStyle()const{return m_borderstyle;}
void SetBorderStyle(const Qt::PenStyle value){m_borderstyle = value;}
BorderType border(){return m_border;}
void SetBorder(BorderType value){m_border = value;}






};










#endif // DESCONTROLS_H
