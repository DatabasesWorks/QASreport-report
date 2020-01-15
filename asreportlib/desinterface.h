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

#ifndef DESINTERFACE_H
#define DESINTERFACE_H

#include <QtPlugin>
#include <QIODevice>


class DesInterface
{
public:
    ~DesInterface(){}

   virtual void runDesigner(QObject *report) = 0;
   virtual void editReport(QObject *repor,QString) = 0;
   virtual void editReport(QObject *repor,QIODevice *) = 0;
   virtual void editReport(QObject *repor,QByteArray&) = 0;
};


QT_BEGIN_NAMESPACE
//! [3] //! [4]
#define DesInterface_iid "org.ajoursoft.DesInterface"

Q_DECLARE_INTERFACE(DesInterface, DesInterface_iid)

QT_END_NAMESPACE






#endif // DESINTERFACE_H
