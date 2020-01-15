/********************************************
*           Calc utils                      *
*                                           *
*  Copyright (c) 2015-2016  Sergey Zgukov   *
*   E-mail:  serg_@ukr.net                  *
********************************************/

#include "calcutils.h"



inline double simpleRoundTo(double n, unsigned d)
{
    double pn = pow(10., d);

    return floor(n * pn + .5) / pn;
}

/*
types:
0: 1234,5
1: 1234,50
2: 1 234,5
3: 1 234,50
4: 1,234500e+03
*/
QString FormatFloat(double num, int type,int prec)
{
    char tf;
    int pr;
    QLocale locale;
    QLocale::NumberOptions opt;
    double d = num;
    QString strnum, strdec;

    opt = type == 0 || type == 1 || type == 4?QLocale::OmitGroupSeparator:QLocale::RejectGroupSeparator;

    locale.setNumberOptions(opt);

    if(type == 0 | type == 2){ tf = 'g'; pr = 16; d = simpleRoundTo(num, prec);} //pr = 16;
    if(type == 1 | type == 3) {tf = 'f'; pr = prec;}
    if(type == 4){ tf = 'e'; pr = prec;}

    strnum = locale.toString(d,tf,pr);

    strdec = strnum.section(locale.decimalPoint(),1,1);

    if(strdec.length() > prec){
        strdec = strdec.left(prec);
        strnum = strnum.section(locale.decimalPoint(),0,0) + locale.decimalPoint() + strdec;
    }

    return strnum;
}













