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


#ifndef DESTYPES_H
#define DESTYPES_H

#include <QString>
#include <QVariant>


enum QTypeSizing{tsTopLeft,tsMidLeft,tsBotLeft,tsMidTop,tsMidBottom,tsTopRight,tsMidRight,tsBotRight,tsNon};

enum QKeyMoveType{kmUp = 0,kmDown = 1, kmLeft = 2,kmRight = 3};

enum QDesType{dtText = 0,dtBand = 1,dtPicture = 2,dtLine = 3,dtFigure = 4,dtBarCode = 5};

enum QFieldType{ftText = 0,ftPicture = 2,ftLine = 3,ftFigure = 4,ftBarCode = 5};

enum QBandType{btReportTitle = 0, btReportSummary = 1,
               btPageHeader = 2, btPageFooter = 3,
               btMasterHeader = 4, btMasterData = 5, btMasterFooter = 6,
               btDetailHeader = 7, btDetailData = 8, btDetailFooter = 9,
               btOverlay = 10,btColumnHeader = 11, btColumnFooter = 12,
               btGroupHeader = 13, btGroupFooter = 14, btNone};

#define PAGEPOS 22

const QString bandNames[] = {"Report Title", "Report Summary",
                             "Page Header", "Page Footer",
                             "Master Header", "Master Data", "Master Footer",
                             "Detail Header", "Detail Data", "Detail Footer",
                             "Overlay","Column Header", "Column Footer",
                             "Group Header", "Group Footer"};

const QBandType BandUnic[] = {btReportTitle,btReportSummary,btPageHeader,btPageFooter,btOverlay,btColumnHeader,btColumnFooter};

enum QTypeAlign{taBottom = 0,taTop = 1,taLeft = 2,taRight = 3,taCenter = 4,taMiddle = 5,taEvenlyWidth = 6,taEvenlyHeight = 7,taCenterPageHor = 8,taCenterPageVer = 9};

enum QTypeUndoRec{urUndo,urRedo};

enum QTypeUndoAction{uaCreate,uaDelete,uaEdit,uaRise,uaLow};

enum QTypeZorder{zoRise,zoLow};

enum QTypeLoad{tlFile, tlStream};

#define MAXUNDOREC 100

struct QUndoRec
{
QObject *object;
int tag;
QTypeUndoAction type;
QMap<QString,QVariant> propValues;
QUndoRec *refRecs;
};

struct QCopyRec
{
QDesType type;
QMap<QString,QVariant> propValues;
};

enum QTypeUnit{rmMill = 0, rmInch = 1};

#define UNDEFINED "undefined"

#define PROPNAMES "BackColor,FontColor,BorderColor,BorderWidth,Border,BorderStyle,TextAlignment,FontFamily,FontSize,FontBold,FontUnderline,FontItalic,WordWrap,Stretchable,AngleType"

#define PAPESIZES "A4 210 x 297 mm,B5 176 x 250 mm,Letter 215.9 x 279.4 mm,Legal  215.9 x 355.6 mm,Executive 190.5 x 254 mm,A0 841 x 1189 mm,A1 594 x 841 mm,A2 420 x 594 mm,A3 297 x 420 mm, A5 148 x 210 mm,A6 105 x 148 mm,A7 74 x 105 mm,A8 52 x 74 mm,A9 37 x 52 mm,B0 1000 x 1414 mm,B1 707 x 1000 mm,B10 31 x 44 mm,B2 500 x 707 mm,B3 353 x 500 mm,B4 250 x 353 mm,B6 125 x 176 mm,B7 88 x 125 mm,B8 62 x 88 mm, B9 44 x 62 mm,C5E 163 x 229 mm,Comm10E 105 x 241 mm,DLE 105 x 241 mm,Folio 210 x 330 mm,Ledger 431.8 x 279.4 mm,Tabloid 279.4 x 431.8 mm,Custom"

#define COLORLIST "white,black,darkMagenta,darkGreen,darkYellow,darkBlue,darkRed,darkCyan,darkGray,gray,red,green,yellow,blue,magenta,lightGray"

#define AGREGATEFUNCS "SUM,COUNT,AVG,MAX,MIN"
#define SYSVARIABLES "PAGE#,DATE#,TIME#,LINETHROUGH#,LINE#,COLUMN#,CURRENT#,TOTALPAGES#"

#define BARCODEANGLES "0,90,180,270"

#define PAGEBOTTMARG 30 //40 120

enum QTypeValue{tvText = 0,tvNumber = 1,tvDate = 2,tvTime = 3,tvBoolean = 4};


#define LISTNUMS "1234,5%1234,50%1 234,5%1 2345,50%1,234500e+03"
#define LISTDATES "05.01.2014%05.01.14%5 jan 2014%5 january 2014%other"
#define LISTTIMES "02:41:25%2:41:25%02:41%2:41%other"
#define LISTLOGICS "0;1%No;Yes%False;True%other"
#define FORMATDATES "dd.MM.yyyy%dd.MM.yy%d MMM yyyy%d MMMM yyyy"
#define FORMATTIMES "HH:mm:ss%H:mm:ss%HH:mm%H:mm"

#define VERSION "1.02"

#define SHOWWAITDIALOG

#define EXTREPFILE ".frm"

#endif // DESTYPES_H
