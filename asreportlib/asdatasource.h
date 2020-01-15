/********************************************
*         QASReport ver.1.01                *
*           Report class                    *
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

#ifndef ASDATASOURCE_H
#define ASDATASOURCE_H

#include <QObject>
#include <QSqlQueryModel>
#include <QSqlQuery>

class QDesDataSource
{

private:
QAbstractItemModel * m_model;

int m_curRow;

public:
    QDesDataSource(QAbstractItemModel *);

QAbstractItemModel *model();
QVariant value(QString);

bool first();
bool previous();
bool next();
bool eof();


};






#endif // ASDATASOURCE_H
