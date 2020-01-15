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


#include "desutils.h"

QSize GetPageSize(QPrinter::PaperSize size,QPrinter::Orientation orientation,int widthmm,int heightmm)
{
QPrinter printer;
QRect rect;

if(size == QPrinter::Custom && widthmm && heightmm)
{
 printer.setPageSizeMM(QSizeF(widthmm,heightmm));
}else
{
if(size == QPrinter::Custom)
    size = QPrinter::A4;

printer.setPaperSize(size);
printer.setOrientation(orientation);
}

rect = printer.pageRect(); //paperRect()

return QSize(rect.width(),rect.height());
}

qreal GetPxOnMil()
{
QScreen *screen = QApplication::primaryScreen();
qreal di = screen->physicalDotsPerInch();

return  di / MILINCH;
}

qreal GetPxOnUnitX(QTypeUnit type, bool phis)
{
QScreen *screen = QApplication::primaryScreen();
qreal di = phis?screen->physicalDotsPerInchX():screen->logicalDotsPerInchX();

if(type == rmMill)
return di / MILINCH;
else
return di / 10.;
}

qreal GetPxOnUnitY(QTypeUnit type,bool phis)
{
QScreen *screen = QApplication::primaryScreen();
qreal di = phis?screen->physicalDotsPerInchY():screen->logicalDotsPerInchY();

if(type == rmMill)
return di / MILINCH;
else
return di / 10.;
}

QString GetDesControlName(QWidget *parent, QString nametype)
{
QString name;
int index = 1;
QWidget* obj = 0;

if(!parent)return "";

do{
   name = nametype + QString::number(index);

   obj = parent->findChild<QWidget*>(name);

   ++index;

}while(obj);

return name;
}



