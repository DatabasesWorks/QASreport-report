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

#include "descontrols.h"



//------------------------------------------------------------------
QFocusControl::QFocusControl(QWidget *parent, QDesControl *control):QWidget(parent)
{
m_Control =  control;

setMouseTracking(true);

m_psize = 5;

UpdateGeometry();

m_TypeSizing = tsNon;
}

void QFocusControl::UpdateGeometry()
{
QRect drect, r = m_Control->rect(),regrect;

resize(r.width() + m_psize,r.height() + m_psize);

drect = rect();

m_rects[tsTopLeft] = QRect(0,0,m_psize,m_psize);
m_rects[tsMidLeft] = QRect(0, drect.height()/2 - m_psize/2,m_psize,m_psize);
m_rects[tsBotLeft] = QRect(0,drect.height() - m_psize,m_psize,m_psize);
m_rects[tsMidTop] = QRect(drect.width()/2 - m_psize/2, 0,m_psize,m_psize);
m_rects[tsMidBottom] = QRect(drect.width()/2 - m_psize/2,drect.height() - m_psize,m_psize,m_psize);
m_rects[tsTopRight] = QRect(drect.width() - m_psize,0,m_psize,m_psize);
m_rects[tsMidRight] = QRect(drect.width() - m_psize,drect.height()/2 - m_psize/2,m_psize,m_psize);
m_rects[tsBotRight] = QRect(drect.width() - m_psize,drect.height() - m_psize,m_psize,m_psize);

regrect = rect();

regrect.setLeft(m_psize);
regrect.setRight(regrect.right() - m_psize);
regrect.setTop(m_psize);
regrect.setBottom(regrect.bottom() - m_psize);

setMask(QRegion(rect()) - QRegion(regrect));

m_hideonmove = false;
}

void QFocusControl::paintEvent(QPaintEvent *pe)
{
QPainter painter(this);
QPen pen;
QBrush brush;

pen.setColor(Qt::black);
pen.setStyle(Qt::NoPen);

brush.setColor(QColor(10,10,130));
brush.setStyle(Qt::SolidPattern);

painter.setPen(pen);
painter.setBrush(brush);

painter.setCompositionMode(QPainter::RasterOp_SourceOrNotDestination);

if(!m_hideonmove)
{
painter.drawRect(m_rects[tsTopLeft]);
painter.drawRect(m_rects[tsBotLeft]);
painter.drawRect(m_rects[tsTopRight]);
painter.drawRect(m_rects[tsBotRight]);

if(!m_MultSel)
{
painter.drawRect(m_rects[tsMidLeft]);
painter.drawRect(m_rects[tsMidTop]);
painter.drawRect(m_rects[tsMidBottom]);
painter.drawRect(m_rects[tsMidRight]);
}
}

}

void QFocusControl::mousePressEvent(QMouseEvent *event)
{
if(event->button() == Qt::LeftButton)
  {
    QPoint p =  event->pos();
    bool ctr = false, shift = false;
    Qt::KeyboardModifiers modif = event->modifiers();
     ctr = Qt::ControlModifier & modif;
     shift = Qt::ShiftModifier & modif;

     m_begpoint = event->pos();

     m_TypeSizing = tsNon;

     if(m_rects[tsTopLeft].contains(p))
         m_TypeSizing = tsTopLeft;

     if(m_rects[tsMidLeft].contains(p))
         m_TypeSizing = tsMidLeft;

     if(m_rects[tsBotLeft].contains(p))
         m_TypeSizing = tsBotLeft;


     if(m_rects[tsMidTop].contains(p))
         m_TypeSizing = tsMidTop;


     if(m_rects[tsMidBottom].contains(p))
         m_TypeSizing = tsMidBottom;


     if(m_rects[tsTopRight].contains(p))
         m_TypeSizing = tsTopRight;


     if(m_rects[tsMidRight].contains(p))
         m_TypeSizing = tsMidRight;

     if(m_rects[tsBotRight] .contains(p))
         m_TypeSizing = tsBotRight;


     if(shift)
         emit m_Control->signalUnSelectControl(m_Control);

     if(ctr)
     {
         QPoint pm = mapTo(parentWidget(), event->pos());
         emit m_Control->signalBeginSelection(pm);
     }else
         emit m_Control->signalSaveUndo(uaEdit,"Width,Height,Left,Top");

    }
}

void QFocusControl::mouseReleaseEvent(QMouseEvent * event)
{

 //if(m_Control->Type() == dtLine &&
if(m_Control->Selected())
   emit m_Control->signalEndMove();

  m_hideonmove = false;
  update();
}

void QFocusControl::mouseMoveEvent(QMouseEvent *event)
{
    bool press = Qt::LeftButton & event->buttons();

    QPoint p =  event->pos();

    QCursor c = Qt::ArrowCursor;

    if(!m_MultSel)
    {
        if(m_rects[tsTopLeft].contains(p))
            c = Qt::SizeFDiagCursor;

        if(m_rects[tsMidLeft].contains(p))
            c = Qt::SizeHorCursor;

        if(m_rects[tsBotLeft].contains(p))
            c = Qt::SizeBDiagCursor;

        if(m_rects[tsMidTop].contains(p))
            c = Qt::SizeVerCursor;

        if(m_rects[tsMidBottom].contains(p))
            c = Qt::SizeVerCursor;

        if(m_rects[tsTopRight].contains(p))
            c = Qt::SizeBDiagCursor;

        if(m_rects[tsMidRight].contains(p))
            c = Qt::SizeHorCursor;

        if(m_rects[tsBotRight].contains(p))
            c = Qt::SizeFDiagCursor;
    }

    if(!press)
         setCursor(c);

    bool ctr = false;
    Qt::KeyboardModifiers modif = event->modifiers();

    ctr = Qt::ControlModifier & modif;

    if(press && !ctr)
    {
    ((QDesignPage*)parentWidget())->MoveControl(m_Control,m_TypeSizing,m_begpoint,event->pos());

     m_hideonmove = true;
    }
}

//------------------------------------------------------------------
QDesControl::QDesControl(QWidget *parent):QWidget(parent)
{
resize(150,24);

m_Selected = false;
m_MultSel = false;

m_parentBand = 0;

m_focusControl = new QFocusControl(parent, this);

setFocusPolicy(Qt::StrongFocus);

m_standardItem = new QStandardItem();
//m_standardItem->setData((int)this,Qt::UserRole + 2);
m_standardItem->setData(QVariant::fromValue(this),Qt::UserRole + 2);


connect(this,SIGNAL(objectNameChanged(QString)),this,SLOT(slotObjectNameChanged(QString)));

//m_editProps = "Name,Width,Height,Left,Top";

m_serviceMenu = 0;
}

void QDesControl::slotObjectNameChanged(QString name)
{
m_standardItem->setText(name);
}

void QDesControl::paintEvent(QPaintEvent *pe)
{

}

void QDesControl::DrawCorner()
{
    int corL = 4;
    QPen pen;

    pen.setColor(Qt::black);
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(1);

    m_Painter.setPen(pen);

    QRect r = rect();
    r.setBottom(r.height() - 1);
    r.setRight(r.width() - 1);

    m_Painter.drawLine(QPoint(r.left(),r.top() + corL),QPoint(r.left(),r.top()));
    m_Painter.drawLine(QPoint(r.left(),r.top()),QPoint(r.left() + corL,r.top()));
    m_Painter.drawLine(QPoint(r.right() - corL,r.top()),QPoint(r.right(),r.top()));
    m_Painter.drawLine(QPoint(r.right(),r.top()),QPoint(r.right(),r.top() + 3));
    m_Painter.drawLine(QPoint(r.right(),r.bottom() - corL),QPoint(r.right(),r.bottom()));
    m_Painter.drawLine(QPoint(r.right(),r.bottom()),QPoint(r.right() - corL,r.bottom()));
    m_Painter.drawLine(QPoint(r.left(),r.bottom() - corL),QPoint(r.left(),r.bottom()));
    m_Painter.drawLine(QPoint(r.left(),r.bottom()),QPoint(r.left() + corL,r.bottom()));
}

void QDesControl::mousePressEvent(QMouseEvent *event)
{

if(event->button() == Qt::LeftButton)
{
bool ctr = false, shift = false;
ctr = Qt::ControlModifier & event->modifiers();
shift = Qt::ShiftModifier & event->modifiers();

 m_begpoint = event->pos();


 if(ctr)
 {
     QPoint pm = mapTo(parentWidget(), event->pos());
     emit signalBeginSelection(pm);
 }else
     if(!m_Selected)
     {
         emit signalSelectControl(this, shift);
         emit signalSaveUndo(uaEdit,"Width,Height,Left,Top");
     }else
     {
         if(shift)
             emit signalUnSelectControl(this);
         else{
             emit signalStartMove();
             emit signalSaveUndo(uaEdit,"Width,Height,Left,Top");
         }
     }
}

if(m_serviceMenu && event->button() == Qt::RightButton)
{
 QPoint p;

    if(!m_Selected)
    {
        emit signalSelectControl(this, false);
        emit signalSaveUndo(uaEdit,"Width,Height,Left,Top");
    }

  p = mapToGlobal(event->pos());
  m_serviceMenu->popup(p);
}

}

void QDesControl::mouseReleaseEvent(QMouseEvent *event)
{
 if(m_Selected)
   emit signalEndMove();
}

void QDesControl::mouseMoveEvent(QMouseEvent *event)
{
    bool ctr = false;

    if(event->buttons() & Qt::LeftButton)
    {
        ctr = Qt::ControlModifier & event->modifiers();

        if(m_Selected && !ctr)
        {
            ((QDesignPage*)parentWidget())->MoveControl(this,tsNon,m_begpoint,event->pos());

            m_focusControl->m_hideonmove = true;
         }

        event->accept();

    }
}

void QDesControl::moveEvent(QMoveEvent * event)
{
m_focusControl->move(geometry().left() - m_focusControl->m_psize/2,geometry().top() - m_focusControl->m_psize/2);

emit ((QDesignPage*)parent())->signalChanging();
}

void QDesControl::resizeEvent(QResizeEvent * event)
{
m_focusControl->UpdateGeometry();

emit ((QDesignPage*)parent())->signalChanging();
}

void QDesControl::slotBeginMouseProcess()
{
setAttribute(Qt::WA_TransparentForMouseEvents, true);
}

void QDesControl::slotEndMouseProcess()
{
setAttribute(Qt::WA_TransparentForMouseEvents, false);


}


void QDesControl::slotSetMultSel(bool mult)
{
m_focusControl->SetMultSel(mult);

m_MultSel = mult;
}

void QDesControl::SetSelect(bool sel)
{
    if(sel)
    {
        m_Selected = true;

        m_focusControl->setVisible(true);
/*
        if(m_Type == dtBand)
        {
            m_focusControl->lower();
            this->stackUnder(m_focusControl);
        }else*/
            m_focusControl->raise();

        m_focusControl->show();

    }else
    {
        m_focusControl->setVisible(false);
        m_Selected = false;

        setCursor(Qt::ArrowCursor);
    }

    emit ((QDesignPage*)parent())->signalSelectInTree(m_standardItem->index());
}

void QDesControl::SetHideSelection(bool val)
{
  m_focusControl->m_hideonmove = val;
  m_focusControl->update();
}

void QDesControl::mouseDoubleClickEvent(QMouseEvent *event)
{
if(m_MultSel) return;

if(CallEditor())
{
emit ((QDesignPage*)parent())->signalUpdatePropValues();
emit ((QDesignPage*)parent())->signalChanging();
}

SetHideSelection(false);
}

void QDesControl::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Return)
    {
        if(m_MultSel)  return;

        if(CallEditor())
        {
        emit ((QDesignPage*)parent())->signalUpdatePropValues();
        emit ((QDesignPage*)parent())->signalChanging();
        }
    }
}

void QDesControl::slotParentResized()
{
UpdateSize();
}

QDesControl::~QDesControl()
{
delete m_focusControl;

}



//--------------------------------------------------------------------
QDesText::QDesText(QWidget *parent):QDesControl(parent)
{
setType(dtText);

m_backColor = Qt::white;
m_fontColor = Qt::black;
m_borderColor = Qt::black;

m_alignment = Qt::AlignVCenter | Qt::AlignLeft;

m_border = 0;
m_borderstyle = Qt::SolidLine;
m_borderwidth = 1;

m_fontFamily = "Arial";
m_fontSize = 9;
m_fontBold = false;
m_fontUnderline = false;
m_fontItalic = false;

m_Text = "";

m_subProps = "type,FontFamily,FontSize,FontBold,FontUnderline,FontItalic,FontColor,TextAlignment,TypeValue";

m_wordwrap = true;
m_stretchable = false;
m_format = "";
m_texttypeangle = Horizontal;
}

void QDesText::paintEvent(QPaintEvent *pe)
{
m_Painter.begin(this);

QBrush brush;
QPen pen;
QFont font = QFont(m_fontFamily, m_fontSize, QFont::Normal);
QColor color;

brush.setStyle(Qt::SolidPattern);

color = m_backColor;

if(color == Qt::transparent)
    color = Qt::white;

brush.setColor(color);

m_Painter.setBrush(brush);

m_Painter.fillRect(rect(), brush);

DrawCorner();

font.setBold(m_fontBold);
font.setUnderline(m_fontUnderline);
font.setItalic(m_fontItalic);

color = m_borderColor;

pen.setColor(color);
pen.setWidth(m_borderwidth);
pen.setStyle((Qt::PenStyle)m_borderstyle);

m_Painter.setFont(font);
m_Painter.setPen(pen);

qreal b = m_borderwidth,w = width(), h = height();

if(m_border & 0x01)
    m_Painter.drawLine(QPointF(b/2,b/2),QPointF(b/2,h - b/2));

if(m_border >> 1 & 0x01)
    m_Painter.drawLine(QPointF(w - b/2,b/2),QPointF(w - b/2,h - b/2));

if(m_border >> 2 & 0x01)
    m_Painter.drawLine(QPointF(b/2,b/2),QPointF(w - b/2,b/2));

if(m_border >> 3 & 0x01)
    m_Painter.drawLine(QPointF(b/2, h - b/2), QPointF(w - b/2, h - b/2));

color = m_fontColor;

if(color == Qt::transparent)
    color = Qt::black;

pen.setColor(color);
pen.setStyle(Qt::SolidLine);

m_Painter.setPen(pen);

if(m_texttypeangle == Horizontal){
    m_Painter.setClipRect(0, 0, width(), height());
    m_Painter.setClipping(true);
    m_Painter.drawText(b, 0, width() - b, height(), m_alignment | (m_wordwrap?Qt::TextWordWrap:0), m_Text);
    m_Painter.setClipping(false);
}else
{
    m_Painter.save();
    m_Painter.translate(b,height());
    m_Painter.rotate(-90);

    m_Painter.setClipRect(0, 0, height(), width());
    m_Painter.setClipping(true);
    m_Painter.drawText(b, 0,  height() - b,width(), m_alignment | (m_wordwrap?Qt::TextWordWrap:0), m_Text);
    m_Painter.setClipping(false);

    m_Painter.restore();
}


m_Painter.end();
}

bool QDesText::CallEditor()
{
QDesTextEdit * txtedit = (QDesTextEdit*)FormSettings->GetEditor((QWidget*)parent(),dtText);

txtedit->SetTextEdit(m_Text);

txtedit->exec();

if(txtedit->result() == QDialog::Accepted)
{
 SetText(txtedit->TextEdit());

 return true;
}

return false;
}

//------------------------------------------------------------------------
QDesLine::QDesLine(QWidget *parent):QDesControl(parent)
{
setType(dtLine);

m_borderColor = Qt::black;
m_borderwidth = 1;
m_borderstyle = Qt::SolidLine;

resize(10,m_borderwidth);

m_begpoint = QPoint(0,0);
m_endpoint = QPoint(10,0);

m_subProps = "type";
}

void QDesLine::paintEvent(QPaintEvent *pe)
{
m_Painter.begin(this);

QBrush brush;
QPen pen;

brush.setStyle(Qt::NoBrush);

pen.setColor(m_borderColor);
pen.setStyle((Qt::PenStyle)m_borderstyle);
pen.setWidth(m_borderwidth);

m_Painter.setBrush(brush);
m_Painter.setPen(pen);

m_Painter.drawLine(m_begpoint, m_endpoint);

m_Painter.end();
}

void QDesLine::UpdateGeometry()
{
    QRect r;

    if(width() > height())
    {
        r = geometry();

        r.setHeight(m_borderwidth);

        setGeometry(r);

        m_endpoint = QPoint(width(),0);

    } else
    {
        r = geometry();

        r.setWidth(m_borderwidth);

        setGeometry(r);

        m_endpoint = QPoint(0,height());
    }
}

void QDesLine::resizeEvent(QResizeEvent *event)
{
UpdateGeometry();

QDesControl::resizeEvent(event);
}

//--------------------------------------------------------------
QDesFigure::QDesFigure(QWidget *parent):QDesControl(parent)
{
setType(dtFigure);

m_figureType = Square;

m_backColor = Qt::white;
m_borderColor = Qt::black;
m_borderstyle = Qt::SolidLine;
m_borderwidth = 1;

resize(100,100);

m_subProps = "type";

}

void QDesFigure::paintEvent(QPaintEvent *pe)
{
    m_Painter.begin(this);

    QBrush brush;
    QPen pen;
    QRectF r = QRectF(rect());
    QPainterPath path;
    qreal b = m_borderwidth;
    QColor color;

    r.setLeft(b/2);
    r.setTop(b/2);
    r.setBottom(r.bottom() - b/2);
    r.setRight(r.right() - b/2);

    m_Painter.setRenderHint(QPainter::HighQualityAntialiasing);//Antialiasing

    r.setWidth(r.width() - 1);
    r.setHeight(r.height() - 1);

    color = m_backColor;

    if(color == Qt::transparent)
        color = Qt::white;

    brush.setColor(color);
    brush.setStyle(Qt::SolidPattern);

    pen.setColor(m_borderColor);
    pen.setStyle((Qt::PenStyle)m_borderstyle);
    pen.setWidth(m_borderwidth);

    m_Painter.setBrush(brush);
    m_Painter.setPen(pen);

    if(m_figureType == Square)
        m_Painter.drawRect(r);

    if(m_figureType == Triangle)
    {
        path.moveTo(b/2,r.height());
        path.lineTo(r.width()/2,b/2);
        path.lineTo(r.width(),r.height());
        path.closeSubpath();

        m_Painter.drawPath(path);
    }

    if(m_figureType == Ellipse)
    {
        path.addEllipse(b/2,b/2,r.width(),r.height());
        path.closeSubpath();

        m_Painter.drawPath(path);
    }

    if(m_figureType == RoundedRect)
    {
        path.addRoundedRect(b/2, b/2, r.width(), r.height(), 10, 10);
        path.closeSubpath();

        m_Painter.drawPath(path);
    }



    m_Painter.end();
}

bool QDesFigure::CallEditor()
{
QDesFigureEdit editor((QWidget*)parent());

editor.SetType(m_figureType);
editor.exec();

    if(editor.result() == QDialog::Accepted)
    {
     m_figureType = (TypeFigure)editor.Type();
     update();

     return true;
    }

return false;
}

//-----------------------------------------------------------------
QDesImage::QDesImage(QWidget *parent):QDesControl(parent)
{
    setType(dtPicture);

    m_borderColor = Qt::black;
    m_borderstyle = Qt::SolidLine;
    m_borderwidth = 1;
    m_border = 0;

    resize(120,120);

    m_subProps = "type,Source";
}

void QDesImage::paintEvent(QPaintEvent *pe)
{
    m_Painter.begin(this);

    QBrush brush;
    QPen pen;
    QRect r = rect();

    r.setWidth(r.width() - 1);
    r.setHeight(r.height() - 1);

    brush.setColor(Qt::white);
    brush.setStyle(Qt::SolidPattern);

    m_Painter.setBrush(brush);
    m_Painter.fillRect(r,brush);

    DrawCorner();

    m_Painter.drawPixmap(r, m_pixmap);

    pen.setColor(m_borderColor);
    pen.setStyle((Qt::PenStyle)m_borderstyle);
    pen.setWidth(m_borderwidth);

    m_Painter.setPen(pen);

    qreal b = m_borderwidth,w = width(), h = height();

    if(m_border & 0x01)
        m_Painter.drawLine(QPointF(b/2,b/2),QPointF(b/2,h - b/2));

    if(m_border >> 1 & 0x01)
        m_Painter.drawLine(QPointF(w - b/2,b/2),QPointF(w - b/2,h - b/2));

    if(m_border >> 2 & 0x01)
        m_Painter.drawLine(QPointF(b/2,b/2),QPointF(w - b/2,b/2));

    if(m_border >> 3 & 0x01)
        m_Painter.drawLine(QPointF(b/2, h - b/2), QPointF(w - b/2, h - b/2));

    m_Painter.end();
}

bool QDesImage::CallEditor()
{
    QDesImageEdit editor((QWidget*)parent());

    editor.SetImage(m_pixmap);
    editor.setSource(m_source);

    editor.exec();

    if(editor.result() == QDialog::Accepted){
        m_pixmap = editor.Image();
        m_source = editor.source();

        update();

        return true;
    }

    return false;
}


//---------------------------------------------------------------
QHeadBand::QHeadBand(QWidget *parent,QDesBand*band):QWidget(parent)
{
resize(180,16);

m_Band = band;
}

void QHeadBand::paintEvent(QPaintEvent *pe)
{
QPainter painter(this);

QPen pen;
QRect rect = this->rect();
QFont font = QFont("Times", 8, QFont::Normal);
QLinearGradient gradient(width(), 0, width(), height());

gradient.setColorAt(0, QColor(220,220,220));
gradient.setColorAt(0.3, QColor(240,240,240));
gradient.setColorAt(0.7, QColor(240,240,240));
gradient.setColorAt(1, QColor(220,220,220));

pen.setColor(Qt::gray);
pen.setStyle(Qt::NoPen);

painter.setPen(pen);
painter.setBrush(gradient);
painter.setFont(font);

QPolygon polygon;

polygon << QPoint(0,rect.height()) << QPoint(5,0) << QPoint(rect.width() - 5,0) << QPoint(rect.width(),rect.height());

painter.drawPolygon(polygon);

pen.setColor(Qt::darkBlue);
pen.setStyle(Qt::SolidLine);

painter.setPen(pen);

painter.drawText(0, 0, rect.width(), rect.height(), Qt::AlignVCenter | Qt::AlignHCenter,m_caption);

}

void QHeadBand::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_begpoint = event->pos();

        emit m_Band->signalSelectControl(m_Band, false);

        emit m_Band->signalStartMove();
    }
}

void QHeadBand::mouseMoveEvent(QMouseEvent *event)
{
QPoint mp;

if(event->buttons() & Qt::LeftButton)
{
mp = pos() + event->pos();
mp -= m_begpoint;
mp.setX(0);

move(mp);
}

}

void QHeadBand::moveEvent(QMoveEvent *event)
{
QPoint p = QPoint(0, pos().y() + height());

//if(((QDesignPage*)parent())->SelectCount() == 1)
//         ((QDesignPage*)parent())->SelectChildsBand(m_Band);

m_Band->move(p);

m_Band->SetHideSelection(true);
}

void QHeadBand::mouseReleaseEvent(QMouseEvent *event)
{
m_Band->SetHideSelection(false);

emit m_Band->signalEndMove();
}

void QHeadBand::slotBeginMouseProcess()
{
setAttribute(Qt::WA_TransparentForMouseEvents, true);
}

void QHeadBand::slotEndMouseProcess()
{
setAttribute(Qt::WA_TransparentForMouseEvents, false);
}


//---------------------------------------------------------------
QDesBand::QDesBand(QWidget *parent, QBandType type):QDesControl(parent)
{
setType(dtBand);

m_BandType =  type;

m_showheader = true;

resize(parent->width(),30);

m_HeadBand = new QHeadBand(parent, this);
m_HeadBand->SetCaption(bandNames[m_BandType]);
m_HeadBand->lower();

slotShowBandHeaders();

connect(this->parent(),SIGNAL(signalBeginMouseProcess()),m_HeadBand,SLOT(slotBeginMouseProcess()));
connect(this->parent(),SIGNAL(signalEndMouseProcess()),m_HeadBand,SLOT(slotEndMouseProcess()));
connect(this,SIGNAL(objectNameChanged(QString)),this,SLOT(slotObjectNameChanged(QString)));
connect(FormSettings,SIGNAL(signalShowBandHeaders()),this,SLOT(slotShowBandHeaders()));

m_subProps = "BandType,type,Source";

m_stretchable = false;
}

void QDesBand::paintEvent(QPaintEvent *pe)
{
m_Painter.begin(this);

QBrush brush;
QPen pen;
QRect rect = this->rect(), headrect;


pen.setColor(QColor(230,230,230));//240
pen.setStyle(Qt::SolidLine);
pen.setWidth(2);

brush.setStyle(Qt::SolidPattern);
brush.setColor(QColor(220,220,220));

m_Painter.setPen(pen);
m_Painter.setBrush(brush);

m_Painter.drawRect(rect);

if(!m_showheader)
{
    headrect = QRect(0,0,180,16);

    QFont font = QFont("Times", 8, QFont::Normal);
    QLinearGradient gradient(headrect.width(), 0, headrect.width(), headrect.height());
    QPolygon polygon;

    gradient.setColorAt(0, QColor(220,220,220));
    gradient.setColorAt(0.3, QColor(240,240,240));
    gradient.setColorAt(0.7, QColor(240,240,240));
    gradient.setColorAt(1, QColor(220,220,220));

    m_Painter.setBrush(gradient);
    m_Painter.setFont(font);

    pen.setStyle(Qt::NoPen);
    m_Painter.setPen(pen);
//    m_Painter.drawRect(headrect);

    polygon << QPoint(headrect.width(),0) << QPoint(headrect.width() - 5,headrect.height()) << QPoint(5,headrect.height()) << QPoint(0,0);

    m_Painter.drawPolygon(polygon);

    pen.setColor(Qt::darkBlue);
    pen.setStyle(Qt::SolidLine);
    m_Painter.setPen(pen);

    m_Painter.drawText(0, 0, headrect.width(), headrect.height(), Qt::AlignVCenter | Qt::AlignHCenter,m_HeadBand->Caption());
}

m_Painter.end();
}

void QDesBand::moveEvent(QMoveEvent * event)
{
QDesControl::moveEvent(event);

m_HeadBand->move(0, pos().y() - m_HeadBand->height());

}

void QDesBand::mouseMoveEvent(QMouseEvent *event)
{
 QDesControl::mouseMoveEvent(event);

 if(event->buttons() & Qt::LeftButton)
 {
     if(((QDesignPage*)parent())->SelectCount() == 1)
         ((QDesignPage*)parent())->SelectChildsBand(this);
 }

}

void QDesBand::StackUnder(QDesControl*control)
{
stackUnder(control);
m_HeadBand->stackUnder(control);
}

void QDesBand::Lower()
{
lower();
m_HeadBand->lower();
}

void QDesBand::slotObjectNameChanged(QString name)
{
 m_HeadBand->SetCaption(bandNames[m_BandType] + ": " + name);
 m_HeadBand->update();

}

void QDesBand::UpdateSize()
{
resize(parentWidget()->width(),height());
}

void QDesBand::slotShowBandHeaders()
{
 SetShowheader(FormSettings->showBandHeaders());

}

bool QDesBand::CallEditor()
{
if(!(m_BandType == btMasterData || m_BandType == btDetailData || m_BandType == btGroupHeader)) return false;

if(m_BandType == btMasterData || m_BandType == btDetailData)
{
QDesSelDataSource selsource((QWidget*)parent());

    selsource.SetTablesModel(FormSettings->Tables());

    selsource.setSource(m_source);
    selsource.exec();

    if(selsource.result() == QDialog::Accepted)
    {
     m_source = selsource.Source();

     return true;
    }
}

if(m_BandType == btGroupHeader){
 QDesSelDataField selfield((QWidget*)parent());

    selfield.setVarEnabled(false);
    selfield.setGroupField(m_source);

    selfield.exec();

    if(selfield.result() == QDialog::Accepted)
    {
     m_source = selfield.groupField();

     return true;
    }
}


return false;
}

QDesBand::~QDesBand()
{
 delete m_HeadBand;
}

//------------------------------------------------------------
QDesBarCode::QDesBarCode(QWidget *parent):QDesControl(parent)
{
setType(dtBarCode);

m_subProps = "type";

m_Barcode = new QBarcode(this);

m_barcodetype = CodeEAN13;
m_Text = "1111111111111";
m_checkSum = false;
m_showText = true;
m_angle = 0;
m_ratio = 2;
m_pixheight = 50;

m_Barcode->setType((QBarcode::BarcodeType)m_barcodetype);
m_Barcode->setChecksum(m_checkSum);
m_Barcode->setShowText(m_showText);
m_Barcode->setHeight(m_pixheight);
m_Barcode->setRatio(m_ratio);

setText(m_Text);

resize(100,m_pixheight);
}



void QDesBarCode::paintEvent(QPaintEvent *pe)
{
m_Painter.begin(this);

QPen pen;
QBrush brush;
QRect r = rect();
QPoint center;

r.setRight(r.right() - 1);
r.setBottom(r.bottom() - 1);

if(m_angle == 0 || m_angle == 180)
            center = QPoint(m_pixwidth/2, m_pixheight/2);

if(m_angle == 90)
            center = QPoint(m_pixheight/2, m_pixheight/2);

if(m_angle == 270)
            center = QPoint(m_pixwidth/2, m_pixwidth/2);

brush.setColor(Qt::white);
brush.setStyle(Qt::SolidPattern);

m_Painter.setBrush(brush);
m_Painter.fillRect(r,brush);

DrawCorner();

m_Painter.save();

m_Painter.translate(center);
m_Painter.rotate(m_angle);
m_Painter.translate(-center);

m_Painter.drawPixmap(0,0, m_pixmap);

m_Painter.restore();


pen.setColor(m_borderColor);
pen.setStyle((Qt::PenStyle)m_borderstyle);
pen.setWidth(m_borderwidth);

m_Painter.setPen(pen);

qreal b = m_borderwidth,w = width(), h = height();

if(m_border & 0x01)
    m_Painter.drawLine(QPointF(b/2,b/2),QPointF(b/2,h - b/2));

if(m_border >> 1 & 0x01)
    m_Painter.drawLine(QPointF(w - b/2,b/2),QPointF(w - b/2,h - b/2));

if(m_border >> 2 & 0x01)
    m_Painter.drawLine(QPointF(b/2,b/2),QPointF(w - b/2,b/2));

if(m_border >> 3 & 0x01)
    m_Painter.drawLine(QPointF(b/2, h - b/2), QPointF(w - b/2, h - b/2));

m_Painter.end();
}

void QDesBarCode::rePaint()
{
m_pixmap = m_Barcode->GetImage();

update();
}

void QDesBarCode::resizeEvent(QResizeEvent *event)
{
 if(m_angle == 0 || m_angle == 180)
     m_pixheight = rect().height();
 else
     m_pixheight = rect().width();

 m_Barcode->setHeight(m_pixheight);

 rePaint();

 QDesControl::resizeEvent(event);
}

void QDesBarCode::setText(QString value)
{
m_Text =  value;

m_Barcode->setText(m_Text);

m_pixwidth = m_Barcode->GetWidth();

rePaint();
}

void QDesBarCode::setChecksum(bool value)
{
    m_checkSum = value;
    m_Barcode->setChecksum(m_checkSum);

    rePaint();
}

void QDesBarCode::setShowText(bool value)
{
    m_showText = value;
    m_Barcode->setShowText(m_showText);

    rePaint();
}

void QDesBarCode::setBarcodeType(BarcodeType value)
{
    m_barcodetype = value;
    m_Barcode->setType((QBarcode::BarcodeType)m_barcodetype);

rePaint();
}

void QDesBarCode::setAngle(double value)
{
  m_angle =  value;

 repaint();
}

void QDesBarCode::setRatio(double value)
{
    m_ratio = value;

    m_Barcode->setRatio(m_ratio);

    rePaint();
}

bool QDesBarCode::CallEditor()
{
QDesBarCodeEditor editor(this);

editor.setValue(m_Text);

editor.exec();

if(editor.result() == QDialog::Accepted)
{
  m_Text =  editor.Value();
  setText(m_Text);

  return true;
}

return false;
}














