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

#ifndef ASREPORT_P_H
#define ASREPORT_P_H

#include <QObject>
#include <QtSql>
#include <QDomDocument>
#include "asreport.h"
#include "asreportview.h"
#include "desdefdata.h"
#include "asrepdesigner.h"
#include "barcode/barcode.h"
#include "aswaitdialog.h"
#include  "desutils.h"
#include "utils/calcutils.h"
#include "asdatasource.h"


class QWidget;
class QPainter;
class QPrinter;
class QDesktopWidget;
class QApplication;
class QASReportView;
class QASRepDesigner;
//class QASReport;

struct RepFunc
{
int dataindex;
int bandindex;
QString func;
QString param;
double value;
int count;
};

struct GroupHeader
{
QDomNode BandHeader;
QDomNode BandFooter;
QVariant value;
};

class QASReportPrivate: public QObject
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(QASReport)

public:
   explicit QASReportPrivate(QObject *parent = 0);

~QASReportPrivate();


enum TypeExport{PDF};

private:
QDomDocument *m_xmlDoc;
QPainter m_Painter;
QPrinter *m_Printer;

QASReportView * m_ReportView;

QWidget * m_parentWidget;

int m_curPage;
int m_curReport;
int m_margLeft;
int m_margTop;
int m_margBottom;
int m_margRight;
int m_heightHeader;
int m_heightFooter;
int m_YBand;

QPrinter::PaperSize m_papeSize;
QSize  m_pageSize;
QPrinter::Orientation m_curOrient;

QDomNode m_pageNode;
QMap<QString,QDesDataSource*> m_MapModels;
QList<RepFunc> m_ListFunc;

int m_DrawIndex;

QList<QDomNode> m_ListBands;

int m_globCountRec;
int m_curLine;

QASRepDesigner * m_Designer;

bool m_ispaint;
bool m_firstprint;

int m_correctBandHeight;

QString  m_reportName;

void newPage();
void processReport();
void prepareReport();
void drawBand(QDomNode);
void drawField(QDomNode node, int yband,int bandtop = 0,bool listdata = false);
QDomNode getBand(QDomNode parent,QBandType);
QDomNode getBand(QBandType type,int beg, int end);
QList<QDomNode> getBandList(QBandType type,int beg, int end);
bool expectNextPage(int calcheight = 0);
void drawDataBand(QBandType type,int beg, int end);
QString getValue(QString invalue,QString format = "");
QVariant getVarValue(QString invalue);
void drawBandByType(QDomNode parent,QBandType type);
int getBackBr(QString value,int beg,QChar,QChar);
QVariant getDBValue(QString param);
bool isExpression(QString param);
QVariant getExpressValue(QString expression);
QVariant getParamValue(QString param);
double calcValue(QChar action,double basevalue, double calcvalue);
QVariant getResultFunc(QString,QString);
void updateAggregates();
void calcAgregate(QDomNode band);
void resetAgregate(int bandindex);
void updateBandList();
int getDataIndex(QDomNode node);
int corrStretchBandHeight(QDomNode);
QString formatValue(QVariant varvalue, QString format);
void createDesigner();
void sortNodeList(QList<QDomNode> &nodelist,QString prop);
QDesDataSource * getDataSource(QString name,bool addonabsent = false);

public:
QASReport * q_ptr;

void reportView();
void reportPrint(bool show = true);
void exportTo(TypeExport type);
QPrinter * printer(){return m_Printer;}
QList<QString> models();
QObject * getModel(QString name,bool addonabsent = false);
void addModel(QString,QObject*);
void clearModels();
int pageCount();
bool load(QString);
bool load(QIODevice * dev);
bool load(QByteArray & buffer);

void runDesigner();
void edit(QString);
void edit(QIODevice * dev);
void edit(QByteArray & buffer);

QString  reportName(){return m_reportName;}
void setReportName(QString value){m_reportName = value;}

signals:
void signalGetValue(QString paramName, QVariant &paramValue);
void signalPrepearReport();
void signalBeginReport();
void signalEndReport();
void signalSaveToStream(QDomDocument *);
void signalModelOnNextRec(QObject *);
void signalModelOnFirstRec(QObject *);
void signalNewPage(int);


private slots:
void slotPreviewClose();
void slotDesignerClose();
void slotCancelPrintProcess();

public slots:
void PaintPreview(QPrinter *printer);



};



#endif // ASREPORT_P_H
