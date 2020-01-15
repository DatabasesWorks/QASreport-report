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

#include "asdatasource.h"


QDesDataSource::QDesDataSource(QAbstractItemModel * model)
{
    m_model = model;

    m_curRow = 0;
}

QAbstractItemModel *QDesDataSource::model()
{
 return m_model;
}

QVariant QDesDataSource::value(QString field)
{
    QVariant value;
    bool ok;
    int fieldnum = 0;

    if(field.isEmpty()) return value;

    if(m_model->inherits("QSqlQueryModel")){
        return ((QSqlQueryModel*)m_model)->query().value(field);
    }else{
        if(m_model->inherits("QAbstractListModel"))
            fieldnum = 0;
        else
            fieldnum = field.toInt(&ok);
        if(!ok) fieldnum = 0;

        return m_model->data(m_model->index(m_curRow,fieldnum));
    }

    return value;
}

bool QDesDataSource::first()
{
    if(m_model->inherits("QSqlQueryModel")){
        QSqlQuery query;

        query = ((QSqlQueryModel*)m_model)->query();

        return query.first();
    }else{
        m_curRow = 0;

        return true;
    }

    return false;
}

bool QDesDataSource::next()
{
 bool res = false;

    if(m_model->inherits("QSqlQueryModel")){
        QSqlQuery query;

        query = ((QSqlQueryModel*)m_model)->query();

        res = query.next();
    }else{
        m_curRow++;

        res = m_curRow <= m_model->rowCount() - 1;

        if(m_curRow > m_model->rowCount() - 1)
                 m_curRow = m_model->rowCount() - 1;
    }

    return res;
}

bool QDesDataSource::previous()
{
 bool res = false;

    if(m_model->inherits("QSqlQueryModel")){
        QSqlQuery query;

        query = ((QSqlQueryModel*)m_model)->query();

        res = query.previous();
    }else{
        m_curRow--;

        res = m_curRow >= 0;

        if(m_curRow < 0)
                m_curRow = 0;
    }

    return res;
}

bool QDesDataSource::eof()
{
 bool res = false;

    if(m_model->inherits("QSqlQueryModel")){
        QSqlQuery query;

        query = ((QSqlQueryModel*)m_model)->query();

        res = query.next();

        if(res)
            query.previous();

        res = !res;
    }else{
          res = m_curRow >= m_model->rowCount() - 1;
    }

  return res;
}
