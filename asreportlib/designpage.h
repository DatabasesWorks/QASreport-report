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


#ifndef DESIGNPAGE_H
#define DESIGNPAGE_H

#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include "desdialogs.h"
#include "despropinspector.h"
#include "asreport_p.h"
#include "descontrols.h"
#include "desdefdata.h"
#include  "desutils.h"
#include "dessettings.h"

class QSelCursor;
class QDesControl;
class QFocusControl;
class QSelectControl;
class QDesBand;
class QDesignPage;

typedef QList<QDesControl*> QListControls;

//------------------------------------------------------------------------------

class QDesBackInd: public QWidget
{
    Q_OBJECT
public:
enum IndType{itHor,itVer};

    QDesBackInd(QWidget *parent,IndType type);
    virtual ~QDesBackInd() {}

private:

IndType m_type;


protected:
virtual void paintEvent(QPaintEvent *pe);

public:

IndType Type(){return m_type;}

};

//--------------------------------------------------
class QDesRuler: public QWidget
{
    Q_OBJECT
public:
enum rulerType{rtHor,rtVer};

    QDesRuler(QWidget *parent, rulerType type);
    virtual ~QDesRuler() {}

private:

rulerType m_type;
QPixmap m_pixmap;

qreal m_pxm;

void DrawRuler();

protected:
virtual void paintEvent(QPaintEvent *pe);
virtual void resizeEvent(QResizeEvent *);

public:
void rePaint();


};


//--------------------------------------------------
class QDesPageBack: public QWidget
{
    Q_OBJECT
public:
    QDesPageBack(QWidget *parent = 0);
    virtual ~QDesPageBack() {}

private:
bool m_visibleind;

QDesBackInd *m_horind;
QDesBackInd *m_verind;

QDesRuler *m_horRule;
QDesRuler *m_verRule;

QPoint m_rulespos;

private slots:

public slots:
void slotSetBackInd(QPoint,bool);

protected:
virtual void paintEvent(QPaintEvent *pe);
virtual void resizeEvent(QResizeEvent *);
virtual void moveEvent(QMoveEvent *);

public:
void UpdatePosPage(QDesignPage*);
void UpdateRules();

};


//-----------------------------------------------------
class QSelCursor: public QWidget
{
    Q_OBJECT

public:
    explicit QSelCursor(QWidget *parent=0);

protected:
virtual void paintEvent(QPaintEvent *pe);

};


//------------------------------------------

class QSelectControl: public QWidget
{
    Q_OBJECT
public:
    QSelectControl(QWidget *parent=0);
    virtual ~QSelectControl() {}

private:

protected:
virtual void paintEvent(QPaintEvent *pe);

signals:
void signalStopSelection();


public:

};


//-----------------------------------------------------
class QDesignPage : public QWidget
{
    Q_OBJECT

public:
    QDesignPage(QWidget* parent, int index);
    virtual ~QDesignPage();

private:

QSize m_defSize;
int m_marginLeft;
int m_marginRight;
int m_marginTop;
int m_marginBottom;

bool m_selPlace;

QSize  m_pageSize;
QPrinter::PaperSize m_papeSize;
QPrinter::Orientation m_orientation;

QSelCursor *m_SelCursor;
QSelectControl * m_SelectControl;

bool m_Selection;
QPoint m_begpoint;

QDesType m_typeObject;

QListControls m_SelectControls;
QListControls m_Controls;

QBandType m_createBand;

QStandardItemModel *m_modelTreeObj;
QStandardItem * m_ParentItem;

int m_Index;

QList<QUndoRec*> m_UndoBuffer;
QList<QUndoRec*> m_RedoBuffer;



QSize m_sizeMM;

QMenu *m_serviceMenu;

qreal m_pxmx;
qreal m_pxmy;

QDesControl * CreateControl(QPoint point,bool undocreate = false,bool load = false);
void ClearSelection();
void AddSelection(QDesControl*);
QListControls GetChildsBand(QDesControl*);
void KeyMoveControl(QKeyMoveType kmt,QTypeSizing ts,int step = 1);
void CreateActions();
QString GetControlName(QString nametype);
QDesControl *controlByName(QString name);
void updateControlParentBand(QDesControl *control);
QStandardItem *GetItemByControl(QStandardItem * ,QDesControl *);
void MoveRowInTree(QStandardItem *, QStandardItem *);
QListControls GetChildsByParentBand(QDesControl*band);
void DeleteRefRec(QUndoRec*);
void ClearUndoBuffer(QTypeUndoRec typerec);
void AddUndoAction(QTypeUndoAction,QString);
void FillPropValues(QUndoRec *,QString);
void ReadProps(QUndoRec *undorect);
void WriteProps(QUndoRec *undorect);
void ExpectUndoBuffer(QTypeUndoRec type);
QUndoRec* CopyUndoRec(QUndoRec* undorect);
void DeleteControl(QDesControl *control);
void ZReordControl(QDesControl * control,QTypeZorder type);
QList<QDesControl *> GetBands();
QList<QDesControl *> GetBandControls(QDesControl *band = 0);
void SaveControlProps(QDesControl *control,QDomElement elem);
QDesControl * LoadControl(QDomElement);
void AlighPointByGrid(QPoint &mp);
void ClearCopyControls();

private slots:
void slotSelectControl(QDesControl*control,bool mult);
void slotUnSelectControl(QDesControl*control);
void slotlBeginSelection(QPoint);
void slotStopSelection();
void slotStartMove();
void slotEndMove();
void slotSaveUndo(QTypeUndoAction,QString);
void slotUpdateRules();



public slots:
void slotKeyMoveControl();
void slotKeyChangeSelect();


protected:
virtual void paintEvent(QPaintEvent *pe);
virtual void mousePressEvent(QMouseEvent *event);
virtual void mouseReleaseEvent(QMouseEvent * event);
virtual void mouseMoveEvent(QMouseEvent *event);
virtual void leaveEvent(QEvent * event);
virtual void enterEvent(QEvent * event);
virtual void resizeEvent(QResizeEvent *);

signals:
void signalEndMouseProcess();
void signalBeginMouseProcess();
void signalSetMultSel(bool);
void signalReadProps(QList<QDesControl*>);
void signalCloseProps();
void signalUpdatePropValues();
void signalEndCreateControl();
void signalSetBackInd(QPoint,bool);
void signalSetExpaded(QModelIndex  index);
void signalUpdateBandMenu();
void signalSelectInTree(QModelIndex);
void signalClearSelectInTree();
void signalUpdateMenuUndo();
void signalChanging();
void signalResized();

public:
QSize PageSize(){return  m_pageSize;}
QPrinter::PaperSize PapeSize(){return m_papeSize;}
void SetPapeSize(QPrinter::PaperSize size);
QPrinter::Orientation Orientation(){return m_orientation;}
void SetOrientation(QPrinter::Orientation value){m_orientation = value;}
int MarginLeft(){return m_marginLeft;}
void SetMarginLeft(int val){ m_marginLeft = val;}
int MarginRight(){return m_marginRight;}
void SetMarginRight(int val){m_marginRight = val;}
int MarginTop(){return m_marginTop;}
void SetMarginTop(int val){m_marginTop = val;}
int MarginBotom(){return m_marginBottom;}
void SetMarginBotom(int val){m_marginBottom = val;}
QSize SizePageMM(){return m_sizeMM;}
void SetSizeMM(QSize size){m_sizeMM = size;}
void SetServiceMenu(QMenu * menu){m_serviceMenu = menu;}


void SetIndex(int index){m_Index = index; if(m_ParentItem)m_ParentItem->setText("Page " +  QString::number(m_Index));}
QStandardItemModel * ModelTreeObj(){return m_modelTreeObj;}
void ZReordSelControl(QTypeZorder type);
void Undo(QTypeUndoRec);
int UndoCount(QTypeUndoRec type) {if(type == urUndo) return m_UndoBuffer.count(); else return m_RedoBuffer.count();}
void BeginCreateControl(int type);
void MoveControl(QDesControl *,QTypeSizing type,QPoint begpoint,QPoint point);
bool SelPlace(){return m_selPlace;}
QDesType SelObject(){return m_typeObject;}
QSelCursor *SelCursor() {return m_SelCursor;}
void SelectAll();
int SelectCount(){return m_SelectControls.count();}
void DeleteSel();
void SelectChildsBand(QDesControl*band);
void SetCreateBand(QBandType type){m_createBand = type;}
bool CanCreateBand(QBandType bandtype);
bool IsBandUnique(int);
QListControls SelectControls(){return m_SelectControls;}
void AlignmentSel(QTypeAlign type);
void SelectControlByModelIndex(QModelIndex);
QDesControl * GetControlByIndex(QModelIndex index);
void Save(QDomDocument*,QDomElement );
void ClearAll();
void Load(QDomElement report);
void PastControls(QList<QCopyRec*>);



};




#endif // QDESIGNPAGE_H
