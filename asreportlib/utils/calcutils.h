/********************************************
*           Calc utils                      *
*                                           *
*  Copyright (c) 2015-2016  Sergey Zgukov   *
*   E-mail:  serg_@ukr.net                  *
********************************************/

#ifndef CALCUTILS_H
#define CALCUTILS_H

#include <QObject>
#include <QSet>
#include <QLocale>
#include <QtSql>
#include <math.h>
#include <QMessageBox>


inline double simpleRoundTo(double n, unsigned d = 2);
QString FormatFloat(double num, int type, int prec);







#endif // CALCUTILS_H
