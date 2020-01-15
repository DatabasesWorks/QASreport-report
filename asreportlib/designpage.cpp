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


#include "designpage.h"


//---------------------------------------------------------------
QDesBackInd::QDesBackInd(QWidget *parent, IndType type):QWidget(parent)
{
m_type =  type;

if(m_type == itHor) resize(20,2);
if(m_type == itVer) resize(2,20);

setVisible(false);
}

void QDesBackInd::paintEvent(QPaintEvent *pe)
{
QPainter painter(this);

QBrush brush;
brush.setColor(QColor(100,100,225));//Qt::gray
brush.setStyle(Qt::SolidPattern);

//painter.setCompositionMode(QPainter::RasterOp_SourceOrNotDestination);

painter.setBrush(brush);

painter.fillRect(rect(),brush);

}

//----------------------------------------------------------------
QDesRuler::QDesRuler(QWidget *parent, rulerType type):QWidget(parent)
{
m_type =  type;

//m_pxm = GetPxOnMil();
}

void QDesRuler::paintEvent(QPaintEvent *pe)
{
QPainter painter(this);

QPen pen;
QBrush brush;

pen.setColor(Qt::darkGray);
pen.setStyle(Qt::SolidLine);

brush.setColor(Qt::white);
brush.setStyle(Qt::SolidPattern);

painter.setBrush(brush);
painter.setPen(pen);

painter.drawPixmap(0, 0,m_pixmap);
}

void QDesRuler::DrawRuler()
{
    m_pixmap = QPixmap(this->size());

    QPainter painter;
    QPen pen;
    QBrush brush;
    qreal pxmx = GetPxOnUnitX(FormSettings->typeRuleMeasure()),
          pxmy = GetPxOnUnitY(FormSettings->typeRuleMeasure());

    QFont font = QFont("Times", 7, QFont::Normal);

    pen.setColor(Qt::gray);
    pen.setStyle(Qt::SolidLine);

    brush.setColor(Qt::white);
    brush.setStyle(Qt::SolidPattern);

    qreal  k = 0., h;
    int c = 0;
    QPointF p1, p2;
    QRectF  strrect;
    QString str;

    painter.begin(&m_pixmap);

    painter.setBrush(brush);
    painter.setPen(pen);
    painter.setFont(font);

    painter.fillRect(m_pixmap.rect(),brush);

    if(m_type == rtHor)
    {
    while(k < m_pixmap.width())
    {
     k = c * pxmx;//m_pxm;

     h = 3;
     if(!(c % 5)) h = 5;
     if(!(c % 10)) h = 10;

     p1.setX(k);
     p1.setY(m_pixmap.height() - h);
     p2.setX(k);
     p2.setY(m_pixmap.height());

     painter.drawLine(p1,p2);

     if(!(c % 10))
     {
         str = QString::number(c/10);
         strrect = painter.boundingRect(strrect,str);

         p1.setX(p1.x() - strrect.width()/2);

         painter.drawText(p1,str);
     }

     ++c;
    }


} else
    {
     while(k < m_pixmap.height())
     {
      k = c * pxmy;//m_pxm;

      h = 3;
      if(!(c % 5)) h = 5;
      if(!(c % 10)) h = 10;

      p1.setY(k);
      p1.setX(m_pixmap.width() - h);
      p2.setY(k);
      p2.setX(m_pixmap.width());

      painter.drawLine(p1,p2);

      if(!(c % 10))
      {
          str = QString::number(c/10);
          strrect = painter.boundingRect(strrect,str);

          p1.setY(p1.y() + strrect.width()/2);

          painter.save();
          painter.translate(p1.x(),p1.y());
          painter.rotate(-90);
          painter.drawText(QPoint(0,0),str);
          painter.restore();
      }

      ++c;
     }

 }

painter.end();
}

void QDesRuler::rePaint()
{
 DrawRuler();
 update();
}

void QDesRuler::resizeEvent(QResizeEvent *event)
{
  DrawRuler();
}


//----------------------------------------------------------------
QDesPageBack::QDesPageBack(QWidget *parent):QWidget(parent)
{
m_visibleind = false;

m_rulespos.setX(2);
m_rulespos.setY(2);

m_horRule = new QDesRuler(this,QDesRuler::rtHor);
m_verRule = new QDesRuler(this,QDesRuler::rtVer);

m_horRule->show();
m_verRule->show();

m_horind = new QDesBackInd(this,QDesBackInd::itHor);
m_verind = new QDesBackInd(this,QDesBackInd::itVer);
}

void QDesPageBack::slotSetBackInd(QPoint p,bool visible)
{
m_visibleind = visible;

m_horind->setVisible(m_visibleind);
m_verind->setVisible(m_visibleind);

if(m_visibleind)
{
 m_horind->move(m_rulespos.x(),p.y() + PAGEPOS);
 m_verind->move(p.x() + PAGEPOS,m_rulespos.y());
 m_horind->raise();
 m_verind->raise();
}
}

void QDesPageBack::moveEvent(QMoveEvent *event)
{
m_rulespos = event->pos();
m_rulespos.setX(m_rulespos.x() * -1);
m_rulespos.setY(m_rulespos.y() * -1);

m_horRule->move(PAGEPOS, m_rulespos.y());
m_verRule->move(m_rulespos.x(), PAGEPOS);
m_horRule->raise();
m_verRule->raise();

m_horind->move(m_rulespos.x(),m_horind->y());
m_verind->move(m_verind->x(),m_rulespos.y());
m_horind->raise();
m_verind->raise();

}

void QDesPageBack::resizeEvent(QResizeEvent * event)
{
UpdatePosPage(0);
}

void QDesPageBack::UpdatePosPage(QDesignPage*page)
{
 if(page)
 {
 resize(page->width() + PAGEPOS,page->height() + PAGEPOS);
 page->move(PAGEPOS,PAGEPOS);
 }

 m_horRule->resize(width() - PAGEPOS,20);
 m_verRule->resize(20,height() - PAGEPOS);
}

void QDesPageBack::UpdateRules()
{
 m_horRule->rePaint();
 m_verRule->rePaint();
}

void QDesPageBack::paintEvent(QPaintEvent *pe)
{
}


//-----------------------------------------------------
QSelCursor::QSelCursor(QWidget *parent):QWidget(parent)
{
   setFocusPolicy(Qt::NoFocus);
   setAttribute(Qt::WA_TransparentForMouseEvents);

   resize(160,24);
}

void QSelCursor::paintEvent(QPaintEvent *pe)
{
QPainter painter(this);
QPen pen;
QRect r = rect();

pen.setColor(Qt::black);
pen.setStyle(Qt::SolidLine);
pen.setWidth(2);

painter.setPen(pen);
painter.drawRect(r);
}

//------------------------------------------------------------------

QSelectControl::QSelectControl(QWidget *parent):QWidget(parent)
{
resize(1,1);

setVisible(false);

setAttribute(Qt::WA_TransparentForMouseEvents, true);
}

void QSelectControl::paintEvent(QPaintEvent *pe)
{
QPainter painter(this);

QPen pen;
QBrush brush;
QRect r = rect();

r.setRight(r.right() - 1);
r.setBottom(r.bottom() - 1);

brush.setColor(QColor(200,200,250,60));

pen.setWidth(1);
pen.setColor(Qt::blue);
pen.setStyle(Qt::SolidLine);

brush.setStyle(Qt::SolidPattern);

painter.setBrush(brush);
painter.setPen(pen);

painter.fillRect(r,brush);
painter.drawRect(r);
}

//---------------------------------------------------
QDesignPage::QDesignPage(QWidget* parent, int index):QWidget(parent)
{
    // setMouseTracking(true);

    m_Index = index;

    m_orientation = QPrinter::Portrait;
    SetPapeSize(QPrinter::A4);

    m_marginLeft = 0;
    m_marginRight = 0;
    m_marginTop = 0;
    m_marginBottom = 0;

    m_SelCursor = 0;

    m_selPlace = false;

    m_SelectControl = new QSelectControl(this);

    m_Selection = false;

    m_createBand = btNone;

    CreateActions();

    m_modelTreeObj = new QStandardItemModel(this);

    QStandardItem * item = new QStandardItem("Page " +  QString::number(m_Index));
    //item->setData((int)this,Qt::UserRole + 2);
    item->setData(QVariant::fromValue(this),Qt::UserRole + 2);

    m_modelTreeObj->insertRow(0,item);
    m_ParentItem = m_modelTreeObj->item(0);

    m_ParentItem->setIcon(QPixmap(":/new/designer/images/new.png"));

    m_sizeMM.setHeight(0);
    m_sizeMM.setWidth(0);

    m_serviceMenu = 0;    

    m_pxmx = GetPxOnUnitX(FormSettings->typeRuleMeasure());
    m_pxmy = GetPxOnUnitY(FormSettings->typeRuleMeasure());
}

void QDesignPage::SetPapeSize(QPrinter::PaperSize size)
{
m_papeSize =  size;
m_pageSize = GetPageSize(m_papeSize,m_orientation,m_sizeMM.width(),m_sizeMM.height());

resize(m_pageSize);

((QDesPageBack*)parent())->UpdatePosPage(this);
}

void QDesignPage::slotUpdateRules()
{
((QDesPageBack*)parent())->UpdateRules();
}

void QDesignPage::CreateActions()
{
    int statval = 0, val;

    statval = 1 << 16;
    statval |= tsNon << 8;

    QAction *controlact = new QAction(this);
    val = statval | kmUp;
    controlact->setData(val);
    controlact->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Up));//tr("Ctrl+Up")
    connect(controlact, SIGNAL(triggered()), this, SLOT(slotKeyMoveControl()));
    this->addAction(controlact);

    controlact = new QAction(this);
    val = statval | kmDown;
    controlact->setData(val);
    controlact->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Down));
    connect(controlact, SIGNAL(triggered()), this, SLOT(slotKeyMoveControl()));
    this->addAction(controlact);

    controlact = new QAction(this);
    val = statval | kmLeft;
    controlact->setData(val);
    controlact->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Left));
    connect(controlact, SIGNAL(triggered()), this, SLOT(slotKeyMoveControl()));
    this->addAction(controlact);

    controlact = new QAction(this);
    val = statval |  kmRight;
    controlact->setData(val);
    controlact->setShortcut(QKeySequence(Qt::CTRL +Qt::Key_Right));
    connect(controlact, SIGNAL(triggered()), this, SLOT(slotKeyMoveControl()));
    this->addAction(controlact);


    statval = 5 << 16;
    statval |= tsNon << 8;

    controlact = new QAction(this);
    val = statval | kmUp;
    controlact->setData(val);
    controlact->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_Up));
    connect(controlact, SIGNAL(triggered()), this, SLOT(slotKeyMoveControl()));
    this->addAction(controlact);

    controlact = new QAction(this);
    val = statval | kmDown;
    controlact->setData(val);
    controlact->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_Down));
    connect(controlact, SIGNAL(triggered()), this, SLOT(slotKeyMoveControl()));
    this->addAction(controlact);

    controlact = new QAction(this);
    val = statval | kmLeft;
    controlact->setData(val);
    controlact->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_Left));
    connect(controlact, SIGNAL(triggered()), this, SLOT(slotKeyMoveControl()));
    this->addAction(controlact);

    controlact = new QAction(this);
    val = statval |  kmRight;
    controlact->setData(val);
    controlact->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT +Qt::Key_Right));
    connect(controlact, SIGNAL(triggered()), this, SLOT(slotKeyMoveControl()));
    this->addAction(controlact);


    statval = 1 << 16;
    statval |= tsMidBottom << 8;

    controlact = new QAction(this);
    val = statval | kmUp;
    controlact->setData(val);
    controlact->setShortcut(QKeySequence(Qt::SHIFT + Qt::Key_Up));
    connect(controlact, SIGNAL(triggered()), this, SLOT(slotKeyMoveControl()));
    this->addAction(controlact);

    controlact = new QAction(this);
    val = statval | kmDown;
    controlact->setData(val);
    controlact->setShortcut(QKeySequence(Qt::SHIFT + Qt::Key_Down));
    connect(controlact, SIGNAL(triggered()), this, SLOT(slotKeyMoveControl()));
    this->addAction(controlact);

    statval = 1 << 16;
    statval |= tsMidRight << 8;

    controlact = new QAction(this);
    val = statval | kmLeft;
    controlact->setData(val);
    controlact->setShortcut(QKeySequence(Qt::SHIFT + Qt::Key_Left));
    connect(controlact, SIGNAL(triggered()), this, SLOT(slotKeyMoveControl()));
    this->addAction(controlact);

    controlact = new QAction(this);
    val = statval |  kmRight;
    controlact->setData(val);
    controlact->setShortcut(QKeySequence(Qt::SHIFT +Qt::Key_Right));
    connect(controlact, SIGNAL(triggered()), this, SLOT(slotKeyMoveControl()));
    this->addAction(controlact);

 //--
    controlact = new QAction(this);
    val = 1;
    controlact->setData(val);
    controlact->setShortcut(QKeySequence(Qt::Key_Right));
    connect(controlact, SIGNAL(triggered()), this, SLOT(slotKeyChangeSelect()));
    this->addAction(controlact);

    controlact = new QAction(this);
    val = -1;
    controlact->setData(val);
    controlact->setShortcut(QKeySequence(Qt::Key_Left));
    connect(controlact, SIGNAL(triggered()), this, SLOT(slotKeyChangeSelect()));
    this->addAction(controlact);

    controlact = new QAction(this);
    val = -1;
    controlact->setData(val);
    controlact->setShortcut(QKeySequence(Qt::Key_Up));
    connect(controlact, SIGNAL(triggered()), this, SLOT(slotKeyChangeSelect()));
    this->addAction(controlact);

    controlact = new QAction(this);
    val = 1;
    controlact->setData(val);
    controlact->setShortcut(QKeySequence(Qt::Key_Down));
    connect(controlact, SIGNAL(triggered()), this, SLOT(slotKeyChangeSelect()));
    this->addAction(controlact);

}



void QDesignPage::paintEvent(QPaintEvent *pe)
{
    QPainter painter(this);

    QBrush brush;
    QPen pen;

    brush.setColor(Qt::white);
    brush.setStyle(Qt::SolidPattern);
    pen.setColor(Qt::NoPen);

    painter.setBrush(brush);
    painter.setPen(pen);

    painter.fillRect(this->rect(), brush);

    if(FormSettings->showGrid()){
    QString file = "textgrid" + QString::number(FormSettings->sizeGrid());

    brush.setTexture(QPixmap(":/new/textures/images/" + file + ".png"));

    painter.setBrush(brush);
    painter.fillRect(this->rect(), brush);
}

    brush.setStyle(Qt::NoBrush);
    pen.setColor(Qt::gray);

    painter.setBrush(brush);
    painter.setPen(pen);

    painter.drawRect(m_marginLeft * m_pxmx,m_marginTop * m_pxmy,width() - m_marginRight * m_pxmx - m_marginLeft * m_pxmx,height() - m_marginTop * m_pxmy - m_marginBottom * m_pxmy);
}

void QDesignPage::BeginCreateControl(int type)
{
    if(type == -1)
    {
        m_selPlace = false;

        if(m_SelCursor)
            m_SelCursor->setVisible(false);

        setCursor(Qt::ArrowCursor);

        emit signalEndMouseProcess();
    } else
    {
        m_selPlace = true;

        m_typeObject =  (QDesType)type;

        emit signalBeginMouseProcess();

        setMouseTracking(true);

        if(!m_SelCursor)
            m_SelCursor = new QSelCursor(this); //

      QCursor cursor;
      QSize size;

        switch (type) {
        case dtText:
        case dtPicture:
        case dtBarCode:
        case dtFigure:   size = QSize(150,24); cursor = Qt::CrossCursor;   break;
        case dtBand: size = QSize(width(),35); cursor = Qt::SizeAllCursor; break;
        case dtLine: size = QSize(150,24); cursor = Qt::CrossCursor; /*cursor = QCursor(QPixmap(":/new/cursors/images/pencilbw.png"));*/break;
        }

        if(type != dtLine)
        {
            m_SelCursor->resize(size.width(),size.height());
            m_SelCursor->raise();
        }

        setCursor(cursor);
    }
}

QDesControl * QDesignPage::CreateControl(QPoint point, bool undocreate,bool load)
{
    if(!undocreate && !m_selPlace)return 0;

    if(m_typeObject == -1) return 0;

    QDesControl *descontrol;
    QString nametype;

    if(m_typeObject == dtText)
    {
        descontrol = new QDesText(this);

        descontrol->move(point.x(),point.y());
        descontrol->show();

        descontrol->standardItem()->setIcon(QPixmap(":/new/designer/images/text.png"));

        nametype = "Text";
    }

    if(m_typeObject == dtBand)
    {
    if(m_createBand == btNone) return 0;

    descontrol = new QDesBand(this, m_createBand);

    descontrol->move(0, point.y());
    descontrol->lower();
    descontrol->show();

    descontrol->standardItem()->setIcon(QPixmap(":/new/designer/images/band.png"));

    nametype = "Band";
    }

    if(m_typeObject == dtLine)
    {
     descontrol = new QDesLine(this);

     descontrol->move(point.x(),point.y());
     descontrol->show();

     descontrol->standardItem()->setIcon(QPixmap(":/new/designer/images/Line.png"));

     nametype = "Line";
    }

    if(m_typeObject == dtFigure)
    {
     descontrol = new QDesFigure(this);

     descontrol->move(point.x(),point.y());
     descontrol->show();

     descontrol->standardItem()->setIcon(QPixmap(":/new/designer/images/figure.png"));

     nametype = "Figure";
    }

    if(m_typeObject == dtPicture)
    {
    descontrol = new QDesImage(this);

    descontrol->move(point.x(),point.y());
    descontrol->show();

    descontrol->standardItem()->setIcon(QPixmap(":/new/designer/images/image.png"));

    nametype = "Picture";
    }

    if(m_typeObject == dtBarCode)
    {
     descontrol = new QDesBarCode(this);

     descontrol->move(point.x(),point.y());
     descontrol->show();

     descontrol->standardItem()->setIcon(QPixmap(":/new/designer/images/barcode.png"));

     nametype = "Barcode";
    }

    descontrol->setObjectName(GetDesControlName(parentWidget(),nametype));

    connect(this,SIGNAL(signalBeginMouseProcess()),descontrol,SLOT(slotBeginMouseProcess()));
    connect(this,SIGNAL(signalEndMouseProcess()),descontrol,SLOT(slotEndMouseProcess()));
    connect(descontrol,SIGNAL(signalSelectControl(QDesControl*,bool)),this,SLOT(slotSelectControl(QDesControl*,bool)));
    connect(this,SIGNAL(signalSetMultSel(bool)),descontrol,SLOT(slotSetMultSel(bool)));
    connect(descontrol,SIGNAL(signalUnSelectControl(QDesControl*)),this,SLOT(slotUnSelectControl(QDesControl*)));
    connect(descontrol,SIGNAL(signalBeginSelection(QPoint)),this,SLOT(slotlBeginSelection(QPoint)));
    connect(descontrol,SIGNAL(signalStartMove()),this,SLOT(slotStartMove()));
    connect(descontrol,SIGNAL(signalEndMove()),this,SLOT(slotEndMove()));
    connect(descontrol,SIGNAL(signalSaveUndo(QTypeUndoAction,QString)),this,SLOT(slotSaveUndo(QTypeUndoAction,QString)));
    connect(this,SIGNAL(signalResized()),descontrol,SLOT(slotParentResized()));

    descontrol->SetServiceMenu(m_serviceMenu);

    slotSelectControl(descontrol, false);

    if(m_SelCursor)
        m_SelCursor->setVisible(false);

    m_Controls.append(descontrol);

    emit signalEndCreateControl();

    emit signalEndMouseProcess();

    if(!load)
        updateControlParentBand(descontrol);

    if(descontrol->Type() == dtBand && IsBandUnique(((QDesBand*)descontrol)->bandType()))
    emit signalUpdateBandMenu();

    emit signalChanging();

    return descontrol;
}

void QDesignPage::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        if(m_selPlace)
        {
            CreateControl(event->pos());

            setMouseTracking(false);

            emit m_SelectControls.at(0)->signalSaveUndo(uaCreate,"");
        }else
        {
            if(m_SelectControls.count())                
                ClearSelection();

            slotlBeginSelection(event->pos());
        }
    }


  if(m_serviceMenu && event->button() == Qt::RightButton)
  {
      if(m_SelectControls.count())
          ClearSelection();

    QPoint p = mapToGlobal(event->pos());
    m_serviceMenu->popup(p);
  }

}

void QDesignPage::mouseReleaseEvent(QMouseEvent * event)
{
 if(m_selPlace)
 {
     m_selPlace = false;

     setCursor(Qt::ArrowCursor);

//     emit signalEndMouseProcess();

     if(m_SelectControls.count() && FormSettings->editAfterInsert()) m_SelectControls.at(0)->CallEditor();

 }else
 if(m_Selection)
   emit slotStopSelection();

}


void QDesignPage::mouseMoveEvent(QMouseEvent *event)
{
    if(m_selPlace)
    {
        QPoint point = event->pos();

        if(m_typeObject == dtBand)
            point.setX(0);

        if(m_typeObject != dtLine)
            m_SelCursor->move(point.x(),point.y());

        if(m_typeObject == dtLine)
        {
         if(event->buttons() & Qt::LeftButton)
          {
           if(!m_SelectControls.count())return;

           QDesLine *line = (QDesLine *)m_SelectControls.at(0);
           QRect rg = line->geometry();

           if(rg.width() > rg.height())
           rg.setRight(event->x());
           else
           rg.setBottom(event->y());

           line->setGeometry(rg);
          }
        }

    }
    else
        if(m_Selection)
        {
         int x,x1,y,y1;

         x = m_begpoint.x() <= event->x()?m_begpoint.x():event->x();
         y = m_begpoint.y() <= event->y()?m_begpoint.y():event->y();
         x1 = m_begpoint.x() <= event->x()?event->x():m_begpoint.x();
         y1 = m_begpoint.y() <= event->y()?event->y():m_begpoint.y();

         m_SelectControl->setGeometry(x,y,x1 - x,y1 - y);
        }
}


void QDesignPage::enterEvent(QEvent * event)
{
    if(m_selPlace && m_typeObject != dtLine)
    {
     if(m_SelCursor)
      m_SelCursor->setVisible(true);
    }
}

void QDesignPage::resizeEvent(QResizeEvent *event)
{
emit signalResized();
}

void QDesignPage::leaveEvent(QEvent * event)
{
    if(m_selPlace)
    {
      m_SelCursor->setVisible(false);
    }
}

void QDesignPage::ClearSelection()
{
QDesControl *contr;

for (int i = 0; i < m_SelectControls.count(); ++i) {
    contr = m_SelectControls.at(i);

    contr->SetSelect(false);    
}

m_SelectControls.clear();

emit signalCloseProps();

emit signalSetBackInd(QPoint(0,0), false);

emit signalClearSelectInTree();
}

void QDesignPage::AddSelection(QDesControl*control)
{
if(m_SelectControls.indexOf(control) != -1) return;

m_SelectControls.append(control);
}

void QDesignPage::slotSelectControl(QDesControl*control,bool mult)
{
    if(!mult && m_SelectControls.count())
    {
        ClearSelection();
    }

    AddSelection(control);

    control->SetSelect(true);

    emit signalSetMultSel(m_SelectControls.count() > 1);

    emit signalReadProps(m_SelectControls);

    emit signalSetBackInd(m_SelectControls.at(0)->pos(), true);

}

void QDesignPage::slotUnSelectControl(QDesControl*control)
{
    int index;

    if((index = m_SelectControls.indexOf(control)) == -1) return;

    control->SetSelect(false);
    m_SelectControls.removeAt(index);

    emit signalSetMultSel(m_SelectControls.count() > 1);

    if(m_SelectControls.count())
        emit signalReadProps(m_SelectControls);
    else
        emit signalCloseProps();

}


QListControls QDesignPage::GetChildsBand(QDesControl*band)
{
QListControls list;
QDesControl *control;

for (int i = 0; i < m_Controls.count(); ++i){

    control = m_Controls.at(i);

    if(control == band) continue;
    if(control->Type() == dtBand) continue;

    if(band->geometry().contains(control->geometry(),false))
        list.append(control);
}

return  list;
}

QListControls QDesignPage::GetChildsByParentBand(QDesControl*band)
{
QListControls list;
QDesControl *control;

for (int i = 0; i < m_Controls.count(); ++i){

    control = m_Controls.at(i);

    if(control == band) continue;
    if(control->Type() == dtBand) continue;

    if(control->ParentBand() == band)
        list.append(control);
}

return  list;
}


void QDesignPage::SelectChildsBand(QDesControl*band)
{
QListControls childs;
QDesControl *control;

if(!band->Selected())return;

childs = GetChildsBand(band);

if(childs.count())
{
  ClearSelection();

  slotSelectControl(band,false);

  for (int i = 0; i < childs.count(); ++i) {
      control = childs.at(i);

      slotSelectControl(control,true);
  }
}
}

void QDesignPage::AlighPointByGrid(QPoint &mp)
{
 int ys, xs, sizegrid = FormSettings->sizeGrid();

 ys = abs(mp.y()) % sizegrid, xs = abs(mp.x()) % sizegrid;

 ys = ys <= sizegrid/2?-ys:(sizegrid - ys);
 xs = xs <= sizegrid/2?-xs:(sizegrid - xs);

 mp.setX(mp.x() + xs);
 mp.setY(mp.y() + ys);
}

void QDesignPage::MoveControl(QDesControl *incontrol,QTypeSizing type,QPoint begpoint,QPoint point)
{
    QPoint p = point, mp;
 //   static int statX = point.x();
    QDesControl *control;
//    int inc;
    QRect rect;

    if(type == tsNon)
    {
      if(incontrol->Type() == dtBand)
        {
         //inc = statX == point.x()?0:(statX > point.x()?-4:4);
         //p.setX(begpoint.x() + inc);
         p.setX(begpoint.x());

        }

      foreach (control, m_SelectControls){
          mp = control->pos() + p;

          mp -= begpoint;

          if(control->Type() == dtBand)
              mp.setX(0);

          if(FormSettings->alignGrid())
              AlighPointByGrid(mp);

          control->move(mp);
        }

        emit signalSetBackInd(incontrol->pos(), true);

      //  statX = point.x();

        emit signalUpdatePropValues();

        return;
    }

    if(m_SelectControls.count() > 1) return;

foreach (control, m_SelectControls){

    mp = control->pos() + p;

    if(FormSettings->alignGrid())
        AlighPointByGrid(mp);

    rect = control->geometry();

    if(type == tsTopLeft)
    {
      if(control->Type() != dtBand)
      rect.setLeft(mp.x());

      rect.setTop(mp.y());
    }

    if(type == tsMidLeft)
      if(control->Type() != dtBand)
     rect.setLeft(mp.x());


    if(type == tsBotLeft)
    {
        rect.setBottom(mp.y());

        if(control->Type() != dtBand)
            rect.setLeft(mp.x());
    }

    if(type == tsMidTop)
        rect.setTop(mp.y());

    if(type == tsMidBottom)
        rect.setBottom(mp.y());

    if(type == tsTopRight)
    {
        if(control->Type() != dtBand)
        rect.setRight(mp.x());

        rect.setTop(mp.y());
    }

    if(type == tsMidRight)
        if(control->Type() != dtBand)
        rect.setRight(mp.x());

    if(type == tsBotRight)
    {
        if(control->Type() != dtBand)
        rect.setRight(mp.x());

        rect.setBottom(mp.y());
    }

if(rect.width() < 1) rect.setWidth(1);
if(rect.height() < 1) rect.setHeight(1);

control->setGeometry(rect);
}

emit signalSetBackInd(incontrol->pos(), true);
emit signalUpdatePropValues();
}


void QDesignPage::slotlBeginSelection(QPoint point)
{
m_begpoint = point;

m_Selection = true;

emit signalBeginMouseProcess();

m_SelectControl->move(m_begpoint.x(),m_begpoint.y());
m_SelectControl->resize(1,1);
m_SelectControl->raise();
m_SelectControl->show();

}

void QDesignPage::slotStopSelection()
{
QRect contrrect, rsel;
QDesControl* control;

rsel = m_SelectControl->geometry();

for (int i = 0; i < m_Controls.count(); ++i){

control = m_Controls.at(i);

if(control->Type() == dtBand) continue;

contrrect = control->geometry();

if(rsel.intersects(contrrect))
{
control->SetSelect(true);

AddSelection(control);
}
}

emit signalSetMultSel(m_SelectControls.count() > 1);

m_Selection = false;

m_SelectControl->setVisible(false);

emit signalEndMouseProcess();

if(m_SelectControls.count())
{
    emit signalReadProps(m_SelectControls);

    emit signalSetBackInd(m_SelectControls.at(0)->pos(), true);
}


}

void QDesignPage::SelectAll()
{
QDesControl*control;

    for (int i = 0; i < m_Controls.count(); ++i) {

        control = m_Controls.at(i);

        control->SetSelect(true);

        AddSelection(control);
    }

emit signalSetMultSel(m_SelectControls.count() > 1);

    if(m_SelectControls.count())
    {
        emit signalReadProps(m_SelectControls);

    }
}

void QDesignPage::slotStartMove()
{
QDesControl *control;

for (int i = 0; i < m_SelectControls.count(); ++i) {

    control = m_SelectControls.at(i);
    control->SetHideSelection(true);
}

}

void QDesignPage::slotEndMove()
{
    QDesControl *control;

    for (int i = 0; i < m_SelectControls.count(); ++i) {

        control = m_SelectControls.at(i);
        control->SetHideSelection(false);

       updateControlParentBand(control);
    }    
}

void QDesignPage::DeleteControl(QDesControl *control)
{
QDesControl *child;
int index = 0;

index =  m_Controls.indexOf(control);

if(index == -1) return;

m_Controls.removeAt(index);

if(control->Type() == dtBand)
{
    foreach (child, m_Controls){
        if(child->Type() == dtBand) continue;

        if(child->ParentBand() == control){
            child->SetParentBand(0);
            MoveRowInTree(child->standardItem(), m_ParentItem);
        }
    }

    if(IsBandUnique(((QDesBand*)control)->bandType()))
    emit signalUpdateBandMenu();
}

m_modelTreeObj->removeRow(control->standardItem()->row(),control->standardItem()->parent()->index());

delete  control;

emit signalChanging();
}

void QDesignPage::DeleteSel()
{
QDesControl *control;

emit m_SelectControls.at(0)->signalSaveUndo(uaDelete,"");

while (m_SelectControls.count()){
    control = m_SelectControls.at(0);

    m_SelectControls.removeAt(0);

    DeleteControl(control);
}

emit signalCloseProps();
}

bool QDesignPage::IsBandUnique(int intype)
{
int size = sizeof(BandUnic)/sizeof(int);
QBandType type;

for (int i = 0; i < size; ++i){
    type = BandUnic[i];

    if(intype == type){return true; break;}
}

return false;
}

bool QDesignPage::CanCreateBand(QBandType bandtype)
{
 if(!IsBandUnique(bandtype)) return  true;

 foreach (QDesControl * control, m_Controls){

     if(control->Type() != dtBand) continue;

     if(((QDesBand*)control)->bandType() == bandtype)return false;
 }

 return true;
}

QList<QDesControl *> QDesignPage::GetBands()
{
QList<QDesControl *> list;
    foreach(QDesControl *control, m_Controls){

        if(control->Type() == dtBand)
            list << control;
    }

    return list;
}

QList<QDesControl *> QDesignPage::GetBandControls(QDesControl *band)
{
QList<QDesControl *> list;

foreach(QDesControl *control, m_Controls){
    if(band)
    {
    if(control->ParentBand() == band)
        list << control;
    }else
    {
      if(control->Type() == dtBand) continue;

    if(!control->ParentBand())
        list << control;
    }
}

return list;
}

void QDesignPage::KeyMoveControl(QKeyMoveType kmt,QTypeSizing ts,int step)
{
QPoint p;
int x = 0, y = 0;
QDesControl *control;
QRect rect;

switch (kmt) {
case kmUp: y = -1 * step;    break;
case kmDown: y = 1 * step;    break;
case kmLeft: x = -1 * step;    break;
case kmRight: x = 1 * step;    break;
}

emit m_SelectControls.at(0)->signalSaveUndo(uaEdit,"Width,Height,Left,Top");

foreach (control, m_SelectControls){
    rect = control->geometry();

    if(ts == tsNon){
        p = control->pos() + QPoint(0,0) + QPoint(x,y);

        control->move(p);
    }

    if(ts == tsMidBottom){
        p = control->pos() + QPoint(rect.width()/2 - 1,rect.height() - 1) + QPoint(x,y);

        rect.setBottom(p.y());
        if(rect.height() < 1) rect.setHeight(1);

        control->setGeometry(rect);
    }

    if(ts == tsMidRight){
        p = control->pos() + QPoint(rect.width() - 1,rect.height()/2 - 1) + QPoint(x,y);

        rect.setRight(p.x());
        if(rect.width() < 1)rect.setWidth(1);

        control->setGeometry(rect);
    }
}

updateControlParentBand(control);

emit signalSetBackInd(m_SelectControls.at(0)->pos(), true);
emit signalUpdatePropValues();
}

void QDesignPage::slotKeyMoveControl()
{
QAction *action;
QKeyMoveType kmt;
QTypeSizing ts;
int step, val;

if(!m_SelectControls.count()) return;

action = (QAction *)QObject::sender();

val = (QKeyMoveType)action->data().toInt();

kmt = (QKeyMoveType)(val & 0xff);
ts = (QTypeSizing)(val >> 8 & 0xff);
step = val >> 16 &  0xff;

KeyMoveControl(kmt,ts,step);


}


void QDesignPage::slotKeyChangeSelect()
{
int index, val;
QDesControl *control;
QAction *action;

if(m_SelectControls.count() != 1) return;

action = (QAction *)QObject::sender();

control = m_SelectControls.at(0);

index = m_Controls.indexOf(control);

val = action->data().toInt();

index += val;

if(index < 0) index = m_Controls.count() - 1;
if(index > m_Controls.count() - 1) index = 0;

control = m_Controls.at(index);

slotSelectControl(control, false);
}


QDesControl *QDesignPage::controlByName(QString name)
{
QDesControl *control;

for (int i = 0; i < m_Controls.count(); ++i){

    control = m_Controls.at(i);

    if(control->Name() == name){return control;}
}

return Q_NULLPTR;
}

QString QDesignPage::GetControlName(QString nametype)
{
QString name;
int index = 1;
QDesControl *control = 0;

do{
  name =  nametype + QString::number(index);

  control = controlByName(name);

  ++index;

}while(control);


return name;
}

void QDesignPage::ZReordControl(QDesControl * selcontrol,QTypeZorder type)
{
if(m_Controls.indexOf(selcontrol) == -1) return;

int index;

if(type == zoLow)
{
if(selcontrol->Type() == dtBand)
   ((QDesBand*)selcontrol)->Lower();
   else
   {
      selcontrol->lower();

      index = m_Controls.indexOf(selcontrol);
       m_Controls.move(index, 0);

       foreach (QDesControl *control, m_Controls){
           if(control->Type() != dtBand) continue;

           ((QDesBand*)control)->StackUnder(selcontrol);
       }
   }
}else
{
    if(selcontrol->Type() != dtBand)
    {
        selcontrol->raise();
        selcontrol->FocusControl()->raise();

        index = m_Controls.indexOf(selcontrol);
        m_Controls.move(index, m_Controls.count() - 1);
    }else
    {
        foreach (QDesControl *control, m_Controls){
            if(control->Type() != dtBand) continue;
            if(control->Selected()) continue;

            ((QDesBand*)control)->StackUnder(selcontrol);
        }
    }
}
}

void QDesignPage::ZReordSelControl(QTypeZorder type)
{
if(m_SelectControls.count())
  emit m_SelectControls.at(0)->signalSaveUndo(type == zoRise?uaRise:uaLow,"Width,Height,Left,Top");

foreach (QDesControl * selcontrol, m_SelectControls)
                            ZReordControl(selcontrol,type);

}

void QDesignPage::updateControlParentBand(QDesControl *control)
{
QDesControl *controlband, *refcontrol;
QPoint p;

if(control->Type() == dtBand)
{
    if(control->standardItem()->parent() != m_ParentItem)
        MoveRowInTree(control->standardItem(), m_ParentItem);

    foreach(refcontrol, m_Controls)
    {
        if(refcontrol == control) continue;
        if(refcontrol->Selected()) continue;
        if(refcontrol->Type() == dtBand) continue;

        if(control->geometry().contains(refcontrol->geometry()) && refcontrol->ParentBand() != control)
        {
         refcontrol->SetParentBand(control);
         MoveRowInTree(refcontrol->standardItem(), control->standardItem());
        }

        if(!control->geometry().contains(refcontrol->geometry()) && refcontrol->ParentBand() == control)
        {
         refcontrol->SetParentBand(0);
         MoveRowInTree(refcontrol->standardItem(), m_ParentItem);
        }

    }
    return;
}

p = control->pos();
p.setX(0);

controlband = (QDesControl *)childAt(p);

if(!controlband){
    control->SetParentBand(0);

    if(control->standardItem()->parent() != m_ParentItem)
        MoveRowInTree(control->standardItem(), m_ParentItem);

    return;
}

if(controlband->Type() != dtBand) return;

if(controlband->geometry().contains(control->geometry()))
{
    if(control->ParentBand() != controlband)
    {
       control->SetParentBand(controlband);

       MoveRowInTree(control->standardItem(), controlband->standardItem());
    }
}else
{
    control->SetParentBand(0);

    if(control->standardItem()->parent() != m_ParentItem)
        MoveRowInTree(control->standardItem(), m_ParentItem);
}
}

void QDesignPage::MoveRowInTree(QStandardItem * item, QStandardItem *moveparent)
{
QList<QStandardItem *> list;
QStandardItem * parentitem;

parentitem = item->parent();

if(!parentitem)
{
    parentitem = m_ParentItem;
    parentitem->appendRow(item);
}

if(parentitem == moveparent)return;

list = parentitem->takeRow(item->row());
moveparent->appendRow(list);

emit signalSetExpaded(moveparent->index());
}


QStandardItem *QDesignPage::GetItemByControl(QStandardItem * parent,QDesControl *seekcontrol)
{
QStandardItem * item;
QDesControl *control;

    for (int i = 0; i < parent->rowCount(); ++i){

        item = parent->child(i);

        //control = (QDesControl*)item->data(Qt::UserRole + 2).toInt();
        control = item->data(Qt::UserRole + 2).value<QDesControl*>();

        if(!control) continue;
        if(control == seekcontrol) return item;

        if(item->hasChildren())
        {
            item = GetItemByControl(item, seekcontrol);

            if(item) return item;
        }
    }

    return Q_NULLPTR;
}


QDesControl * QDesignPage::GetControlByIndex(QModelIndex index)
{
foreach(QDesControl * control,m_Controls)
{
    if(control->standardItem()->index() == index)
      return control;
}

return Q_NULLPTR;
}

void QDesignPage::SelectControlByModelIndex(QModelIndex index)
{
QDesControl *findcontrol = GetControlByIndex(index);

if(findcontrol)
   slotSelectControl(findcontrol,false);

}

void QDesignPage::AlignmentSel(QTypeAlign type)
{
QDesControl *control;
QRect rect;
int val = 0, valmax = 0, c = 1, x, y, cb = 0;
QDesControl *begcontrol = 0, *endcontrol = 0;

if(!(type == taCenterPageHor || type == taCenterPageVer) && m_SelectControls.count() < 2) return;

switch (type){
case taBottom:  val =  m_SelectControls.at(0)->geometry().bottom() + 1;  break;
case taTop:     val =  m_SelectControls.at(0)->geometry().top();    break;
case taLeft:    val =  m_SelectControls.at(0)->geometry().left();    break;
case taRight:   val =  m_SelectControls.at(0)->geometry().right() + 1;  break;
case taCenter:  val =  m_SelectControls.at(0)->geometry().x() + m_SelectControls.at(0)->geometry().width()/2 ;   break;
case taMiddle:  val =  m_SelectControls.at(0)->geometry().y() + m_SelectControls.at(0)->geometry().height()/2 ; break;
case taEvenlyWidth:
    val  = m_SelectControls.at(0)->x(); begcontrol = m_SelectControls.at(0);

    foreach (control, m_SelectControls){
        if(control->x() < val) {val = control->x(); begcontrol = control;}
        if(control->x() > valmax) {valmax = control->x(); endcontrol = control;}
    }

    val = (valmax - val)/(m_SelectControls.count() - 1);
    cb = begcontrol->x();
    break;
case taEvenlyHeight:
    val  = m_SelectControls.at(0)->y(); begcontrol = m_SelectControls.at(0);

    foreach (control, m_SelectControls){
        if(control->y() < val) {val = control->y(); begcontrol = control;}
        if(control->y() > valmax) {valmax = control->y(); endcontrol = control;}
    }

    val = (valmax - val)/(m_SelectControls.count() - 1);
    cb = begcontrol->y();
    break;
case taCenterPageHor:   val = geometry().width()/2; break;
case taCenterPageVer:   val = geometry().height()/2;    break;
}

if(m_SelectControls.count())
emit m_SelectControls.at(0)->signalSaveUndo(uaEdit,"Width,Height,Left,Top");

foreach (control, m_SelectControls)
{
    rect = control->geometry();

    switch (type) {
    case taBottom:        x = rect.x(); y = val - rect.height();    break;
    case taTop:           x = rect.x(); y = val;     break;
    case taLeft:          x = val; y = rect.y();        break;
    case taRight:         x = val - rect.width(); y = rect.y();  break;
    case taCenter:        x = val - rect.width()/2; y = rect.y();  break;
    case taMiddle:        x = rect.x(); y = val - rect.height()/2;  break;
    case taEvenlyWidth:   if(control == begcontrol || control == endcontrol) continue;
                          x = cb + val * c; y = rect.y();  break;
    case taEvenlyHeight:    if(control == begcontrol || control == endcontrol)continue;
                           x = rect.x(); y = cb + val * c;        break;
    case taCenterPageHor:  x = val - rect.width()/2; y = rect.y();      break;
    case taCenterPageVer:  x = rect.x() ; y = val - rect.height()/2;     break;
    }

    if(control->Type() == dtBand)
    control->move(0,y);
    else
    control->move(x,y);

    ++c;
}
}

void QDesignPage::DeleteRefRec(QUndoRec* undorect)
{
   if(!undorect) return;

    if(undorect->refRecs)
        DeleteRefRec(undorect->refRecs);

    delete undorect;
    undorect = 0;
}

void QDesignPage::ClearUndoBuffer(QTypeUndoRec typerec)
{
QList<QUndoRec*> *listrec = typerec == urUndo?&m_UndoBuffer:&m_RedoBuffer;
QUndoRec *undorect;

while (listrec->count()){
    undorect = listrec->at(0);

    if(undorect->refRecs)
            DeleteRefRec(undorect->refRecs);
    delete undorect;
    listrec->removeAt(0);
}
}

void QDesignPage::slotSaveUndo(QTypeUndoAction type,QString props)
{
  AddUndoAction(type,props);
}

void QDesignPage::ReadProps(QUndoRec *undorect)
{
if(undorect->refRecs)
            ReadProps(undorect->refRecs);

if(m_Controls.indexOf((QDesControl*)undorect->object) == -1)
                                                  return;

const QMetaObject* metaObject = undorect->object->metaObject();
QMetaProperty property;
int index;
QVariant propvalue;
QString propname;
QList<QString>list = undorect->propValues.keys();

foreach (propname, list){
index = metaObject->indexOfProperty(propname.toLocal8Bit().data());

if(index == -1) continue;

propvalue = undorect->propValues.value(propname);

property = metaObject->property(index);
property.write(undorect->object, propvalue);
}

((QDesControl*)undorect->object)->update();
}

void QDesignPage::WriteProps(QUndoRec *undorect)
{
    if(undorect->refRecs)
                WriteProps(undorect->refRecs);

    if(m_Controls.indexOf((QDesControl*)undorect->object) == -1)
                                                      return;

    const QMetaObject* metaObject = undorect->object->metaObject();
    QMetaProperty property;
    int index;
    QString propname;
    QList<QString>list = undorect->propValues.keys();

    foreach (propname, list){
    index = metaObject->indexOfProperty(propname.toLocal8Bit().data());

    if(index == -1) continue;

    property = metaObject->property(index);
    undorect->propValues[property.name()] = property.read(undorect->object);
    }
}

QUndoRec* QDesignPage::CopyUndoRec(QUndoRec* undorect)
{
    QUndoRec* refrec  = new QUndoRec;

    *refrec = *undorect;

    if(undorect->refRecs) refrec->refRecs = CopyUndoRec(undorect->refRecs);
    else
      refrec->refRecs = 0;

    return refrec;
}

//-------------Undo-----------
void QDesignPage::Undo(QTypeUndoRec type)
{
QList<QUndoRec*> *list = type == urUndo?&m_UndoBuffer:&m_RedoBuffer;
QUndoRec *undorect, *refrect, *temprec;

if(!list->count()) return;

undorect = list->last();
list->removeLast();

refrect = CopyUndoRec(undorect);
WriteProps(undorect);

switch (undorect->type){
case uaCreate:
    ClearSelection();

    temprec = undorect;
    while(temprec)
    {
        DeleteControl((QDesControl*)temprec->object);
        temprec = temprec->refRecs;
    }

    undorect->type = uaDelete;
    break;

case uaDelete:
    temprec = undorect;
    while(temprec)
    {
        m_typeObject = (QDesType)temprec->tag;
        temprec->object = CreateControl(QPoint(temprec->propValues.value("Left").toInt(),temprec->propValues.value("Top").toInt()),true);
        temprec = temprec->refRecs;
    }

    undorect->type = uaCreate;

    ReadProps(undorect);
    ClearSelection();
    break;

case uaEdit:
    ReadProps(refrect);
    break;

case uaRise:
    temprec = undorect;
    while(temprec)
    {
      ZReordControl((QDesControl*)temprec->object,zoLow);

      temprec = temprec->refRecs;
      undorect->type = uaLow;
    }
    break;

case uaLow:
    temprec = undorect;
    while(temprec)
    {
      ZReordControl((QDesControl*)temprec->object,zoRise);

      temprec = temprec->refRecs;
      undorect->type = uaRise;
    }
    break;
}

ExpectUndoBuffer(type == urUndo?urRedo:urUndo);

(type == urUndo?m_RedoBuffer:m_UndoBuffer).append(undorect);

emit signalUpdateMenuUndo();

DeleteRefRec(refrect);
}

void QDesignPage::FillPropValues(QUndoRec *undorect,QString props)
{
const QMetaObject* metaObject = undorect->object->metaObject();
QMetaProperty property;
QStringList proplist = props.split(',');
int index;

if(!props.isEmpty())
{
     foreach (QString propname, proplist){
      if(propname == "Name")continue;

      index = metaObject->indexOfProperty(propname.toLocal8Bit().data());

      if(index == -1) continue;

      property = metaObject->property(index);

      undorect->propValues[property.name()] = property.read(undorect->object);
     }
}else
    for(int i = metaObject->propertyOffset(); i < metaObject->propertyCount(); ++i){
        property = metaObject->property(i);

        if(!strcmp(property.name(), "Name"))continue;

        undorect->propValues[property.name()] = property.read(undorect->object);
    }
}



void QDesignPage::ExpectUndoBuffer(QTypeUndoRec type)
{
QList<QUndoRec*> *list = type == urUndo?&m_UndoBuffer:&m_RedoBuffer;
QUndoRec *undorect;

if(list->count() >= MAXUNDOREC){
    undorect = list->at(0);
    delete undorect;
    list->removeAt(0);
}
}

void QDesignPage::AddUndoAction(QTypeUndoAction type,QString props)
{
QUndoRec *prevrec, *undorect;
QDesControl* control;

if(!m_SelectControls.count()) return;

ExpectUndoBuffer(urUndo);

control = m_SelectControls.at(0);

prevrec = new QUndoRec;
prevrec->type = type;
prevrec->object = control;
prevrec->tag = control->Type();

m_UndoBuffer.append(prevrec);

FillPropValues(prevrec,props);

if(m_SelectControls.count() > 1)
for (int i = 1; i < m_SelectControls.count(); ++i)
{
    control = m_SelectControls.at(i);

    undorect = new QUndoRec;
    undorect->type = type;
    undorect->object = control;
    undorect->tag = control->Type();
    prevrec->refRecs = undorect;

    FillPropValues(undorect,props);
    prevrec = undorect;
}

prevrec->refRecs = 0;

emit signalUpdateMenuUndo();
}

void QDesignPage::SaveControlProps(QDesControl * control,QDomElement elem)
{
const QMetaObject* metaObject;
int index;
QVariant propvalue;
QString propname;

metaObject = control->metaObject();

for(int i = metaObject->propertyOffset(); i < metaObject->propertyCount(); ++i)
{
    propname = QString::fromLatin1(metaObject->property(i).name());

    index = metaObject->indexOfProperty(propname.toLocal8Bit().data());

    if(metaObject->property(index).type() == QVariant::UserType) continue;

    propvalue = metaObject->property(index).read(control);

    if(propvalue.type() == QVariant::Pixmap)
    {
      QByteArray byteArray;
      QBuffer buffer(&byteArray);
      buffer.open(QIODevice::WriteOnly);
      QPixmap pix = propvalue.value<QPixmap>();

      pix.save(&buffer,"PNG");
      elem.setAttribute(propname,QString(byteArray.toBase64()));

      continue;
    }

    elem.setAttribute(propname,propvalue.toString());
}
}

void QDesignPage::Save(QDomDocument*xmldoc,QDomElement report)
{
QDomElement contrelem, bandelem;
QList<QDesControl *> controls, bands;

report.setAttribute("pageNo",m_Index);
report.setAttribute("marginLeft",m_marginLeft);
report.setAttribute("marginRight",m_marginRight);
report.setAttribute("marginTop",m_marginTop);
report.setAttribute("marginBottom",m_marginBottom);
report.setAttribute("orientation",m_orientation);
report.setAttribute("papesize",m_papeSize);
report.setAttribute("widthMM",m_sizeMM.width());
report.setAttribute("heightMM",m_sizeMM.height());

bands = GetBands();
foreach (QDesControl * band, bands){
    bandelem = xmldoc->createElement("Band");
    report.appendChild(bandelem);

    SaveControlProps(band, bandelem);

    controls = GetBandControls(band);
    foreach(QDesControl * control, controls){
        contrelem = xmldoc->createElement("Field");
        bandelem.appendChild(contrelem);

        SaveControlProps(control, contrelem);
    }
}

controls = GetBandControls();
foreach(QDesControl * control, controls){
    contrelem = xmldoc->createElement("Field");
    report.appendChild(contrelem);

    SaveControlProps(control, contrelem);
}
}

void QDesignPage::ClearAll()
{
QDesControl *contr;
QStandardItem * item;

ClearSelection();

ClearUndoBuffer(urRedo);
ClearUndoBuffer(urUndo);

while (m_Controls.count()) {
     contr = m_Controls.at(0);

     delete contr; contr = 0;
     m_Controls.removeAt(0);
}

m_modelTreeObj->clear();

item = new QStandardItem("Page " +  QString::number(m_Index));
//item->setData((int)this,Qt::UserRole + 2);
item->setData(QVariant::fromValue(this),Qt::UserRole + 2);

m_modelTreeObj->insertRow(0,item);
m_ParentItem = m_modelTreeObj->item(0);

emit signalUpdateMenuUndo();
}

QDesControl * QDesignPage::LoadControl(QDomElement elem)
{
QDesControl * control;
QPoint point;
QString tagname;
const QMetaObject* metaObject;
QVariant propvalue;
QString propname;
int index, x, y;
QMetaProperty prop;

tagname = elem.tagName();

if(tagname.isEmpty()) return 0;

if(!(tagname == "Band" || tagname == "Field")) return 0;

m_typeObject = (QDesType)elem.attribute("type").toInt();

if(tagname == "Band")
{
    m_typeObject = dtBand;
    m_createBand = (QBandType)elem.attribute("BandType").toInt();

    x = 0;
    y = elem.attribute("Top").toInt();
}else{
    x = elem.attribute("Left").toInt();
    y = elem.attribute("Top").toInt();
}

point.setX(x);
point.setY(y);

control = CreateControl(point, true,true);

metaObject = control->metaObject();

for(int i = metaObject->propertyOffset(); i < metaObject->propertyCount(); ++i)
{
    prop = metaObject->property(i);

    propname = QString::fromLatin1(prop.name());

    index = metaObject->indexOfProperty(propname.toLocal8Bit().data()); 

    if(metaObject->property(index).type() == QVariant::UserType) continue;

    propvalue = elem.attribute(propname);

    if(metaObject->property(index).type() == QVariant::Pixmap)
    {
      QByteArray byteArray =  QByteArray::fromBase64(elem.attribute(propname).toLatin1());
      QPixmap pixmap = QPixmap::fromImage(QImage::fromData(byteArray, "PNG"));

      propvalue = pixmap;

      metaObject->property(index).write(control,propvalue);

      continue;
    }

   if(prop.isEnumType())
         propvalue = propvalue.toInt();

    metaObject->property(index).write(control,propvalue);
}

return  control;
}

void QDesignPage::Load(QDomElement report)
{
QDomElement band, elem;
QDesControl *control, *bandcontrol;

m_selPlace = false;

m_orientation = (QPrinter::Orientation)report.attribute("orientation").toInt();

m_marginRight  = report.attribute("marginRight").toInt();
m_marginLeft = report.attribute("marginLeft").toInt();
m_marginTop = report.attribute("marginTop").toInt();
m_marginBottom = report.attribute("marginBottom").toInt();
m_Index = report.attribute("pageNo").toInt();
m_sizeMM.setWidth(report.attribute("widthMM").toInt());
m_sizeMM.setHeight(report.attribute("heightMM").toInt());

if(report.attribute("papesize").isNull())
SetPapeSize(QPrinter::A4);
else
SetPapeSize((QPrinter::PageSize)report.attribute("papesize").toInt());

for (int i = 0; i < report.childNodes().count(); ++i){

    elem = report.childNodes().at(i).toElement();

    if(elem.tagName() == "Band")
    {
        band = elem;

        bandcontrol = LoadControl(band);
        MoveRowInTree(bandcontrol->standardItem(), m_ParentItem);

        for (int c = 0; c < band.childNodes().count(); ++c){
            elem = band.childNodes().at(c).toElement();

            if(elem.tagName() == "Field")
            {
                control = LoadControl(elem);
                control->SetParentBand(bandcontrol);
                MoveRowInTree(control->standardItem(), bandcontrol->standardItem());
            }
        }
    }else
        if(elem.tagName() == "Field")
        {
            control = LoadControl(elem);
            MoveRowInTree(control->standardItem(), m_ParentItem);
        }
}

}

void QDesignPage::PastControls(QList<QCopyRec*> list)
{
QDesControl *control;
const QMetaObject* metaObject;
QVariant propvalue;
int index;
QString propname;
QList<QString>listkeys;
QMetaProperty property;
QList<QDesControl *> controlist;

foreach (QCopyRec *copyRec, list){
m_typeObject = copyRec->type;

if(m_typeObject == dtBand)
{
    if(!CanCreateBand((QBandType)copyRec->propValues.value("BandType").toInt()))
        continue;
    else
        m_createBand = (QBandType)copyRec->propValues.value("BandType").toInt();
}

control = CreateControl(QPoint(0,0),true,true);

metaObject = control->metaObject();

listkeys = copyRec->propValues.keys();

foreach (propname, listkeys){
    if(propname == "Name") continue;

    index = metaObject->indexOfProperty(propname.toLocal8Bit().data());
    if(index  == -1) continue;

    property = metaObject->property(index);
    propvalue = copyRec->propValues.value(propname);

    if(propname == "Top")
        propvalue = propvalue.toInt() + 4;

    if(propname == "Left")
        if(m_typeObject != dtBand)
           propvalue = propvalue.toInt() + 4;

    property.write(control, propvalue);
}

control->update();

controlist << control;

updateControlParentBand(control);
}

   ClearSelection();

   foreach (control, controlist)
       slotSelectControl(control,true);



}

QDesignPage::~QDesignPage()
{
  ClearUndoBuffer(urRedo);
  ClearUndoBuffer(urUndo);
}




