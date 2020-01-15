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

#ifndef ASREPORT_H
#define ASREPORT_H

#include <QObject>
#include <QtSql>
#include <QDomDocument>
#include <QPrinter>
#include "asreportlib_global.h"

class QASReportPrivate;

class  ASREPORTLIBSHARED_EXPORT QASReport: public QObject
{
    Q_OBJECT

public:
    explicit QASReport(QObject *parent = 0);

    virtual ~QASReport();

    void reportView();
    void reportPrint(bool show = true);
    void exportTo(int type);
    QPrinter * printer();
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

    QString  reportName();
    void setReportName(QString value);

signals:
    void signalGetValue(QString paramName, QVariant &paramValue);
    void signalPrepearReport();
    void signalBeginReport();
    void signalEndReport();
    void signalSaveToStream(QDomDocument *);
    void signalModelOnNextRec(QObject *);
    void signalModelOnFirstRec(QObject *);
    void signalNewPage(int);

protected:
    QASReportPrivate * const d_ptr;

    QASReport(QASReportPrivate & prep,QObject *parent = 0);

private:
    Q_DECLARE_PRIVATE(QASReport)
};










#endif // ASREPORT_H
