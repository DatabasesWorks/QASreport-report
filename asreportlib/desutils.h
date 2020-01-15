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


#ifndef DESUTILS_H
#define DESUTILS_H

#include <QApplication>
#include <QScreen>
#include <QWidget>
#include <QPrinter>
#include <math.h>
#include "desdefdata.h"

#define MILINCH 25.4

QSize GetPageSize(QPrinter::PaperSize size,QPrinter::Orientation = QPrinter::Portrait,int widthmm = 0,int heightmm = 0);
qreal GetPxOnMil();
qreal GetPxOnUnitX(QTypeUnit type,bool phis = false);
qreal GetPxOnUnitY(QTypeUnit type,bool phis = false);
QString GetDesControlName(QWidget *parent,QString);


#endif // QDESUTILS_H
