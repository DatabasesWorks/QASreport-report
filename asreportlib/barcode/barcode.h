/********************************************
*         QBarcode ver.1.0                  *
*           Barcode class                   *
*                                           *
*  Copyright (c) 2015-2016  Sergey Zgukov   *
*  E-mail:  serg_@ukr.net                   *
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

#ifndef QBARCODE_H
#define QBARCODE_H

#include <QWidget>
#include <QPainter>
#include <QMessageBox>
#include <QApplication>
#include "barcodedata.h"



//--------------------------------------------------------
class QBarcode : public QObject
{
    Q_OBJECT
public:
    explicit QBarcode(QObject *parent = 0);

enum BarcodeType{bcCode_2_5_interleaved,
                 bcCode_2_5_industrial,
                 bcCode_2_5_matrix,
                 bcCode39,
                 bcCode39Extended,
                 bcCode128A,
                 bcCode128B,
                 bcCode128C,
                 bcCode93,
                 bcCode93Extended,
                 bcCodeMSI,
                 bcCodePostNet,
                 bcCodeCodabar,
                 bcCodeEAN8,
                 bcCodeEAN13};

enum BarLineType{white, black, black_half};


private:
int m_height;
QString m_text;
int m_module;
double m_ratio;
BarcodeType m_type;
bool m_checkSum;
bool m_showText;
short m_modules[4];
int m_txtheight;

QPainter m_Painter;
QPixmap m_pixmap;


void  OneBarProps(char code, int &w, BarLineType &lt);
void DoLines(QString data);
QString Code_2_5_interleaved();
QString Code_2_5_industrial();
QString Code_2_5_matrix();
QString Code_39();
QString Code_39Extended();
QString Code_128();
QString Code_93();
QString Code_93Extended();
QString Code_MSI();
QString Code_PostNet();
QString Code_Codabar();
QString Code_EAN8();
QString Code_EAN13();
QString GetTypeText();
void MakeModules();

protected:
virtual void Draw();
QString MakeData();

signals:

public slots:    

public:
int Height(){return m_height;}
void setHeight(int value){m_height = value;}
QString Text(){return m_text;}
void setText(QString value){m_text = value;}
int Module(){return m_module;}
void setModule(int value){if (value >= 1 && value < 50)m_module = value;}
double Ratio(){return m_ratio;}
void setRatio(double value){m_ratio = value;}
BarcodeType Type(){return m_type;}
void setType(BarcodeType value){m_type = value;}
bool Checksum(){return m_checkSum;}
void setChecksum(bool value){m_checkSum = value;}
bool ShowText(){return m_showText;}
void setShowText(bool value){m_showText = value;}
int GetWidth();
QPixmap GetImage();
};

QString Convert(QString s);
int quersumme(int x);
QString getEAN(QString nr);
int FindIdx(char z);
int Find_Code128AB(char c,int type);
int Find_Code93(char c);
int Find_Codabar(char c);




#endif // QBARCODE_H
