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

#include "barcode.h"

QString Convert(QString s)
{
int v;
QString t = "";

for(int i = 0;i < s.length();++i){
    v = int(s.toLatin1().at(i)) - 1;

    if(!(i % 2))
            v += 5;

    t += QString(QChar(char(v)));
}

return t;
}

int quersumme(int x)
{
int sum;

sum = 0;

while(x > 0){
sum += (x % 10);
x /= 10;
}

return sum;
}

//---------------------- EAN -------------------------------------
QString getEAN(QString nr)
{
int fak, sum;
QString tmp;

    sum = 0;
    tmp = nr;

    fak = tmp.length();

    for(int i = 0; i < tmp.length();++i) {
        if (!(fak % 2))
            sum += (QString(tmp.at(i)).toInt() * 1);
        else
            sum +=  (QString(tmp.at(i)).toInt() * 3);
        fak--;
    }

    if (!(sum % 10))
        return  tmp + '0';
    else
        return tmp + QString::number(10 - (sum % 10));
}


//----QBarcode--------------------------------------------------------------------
QBarcode::QBarcode(QObject *parent):QObject(parent)
{
m_type = bcCodeEAN13;
m_ratio = 2;
m_module = 1;
m_checkSum = false;
m_showText = false;

m_height = 40;
m_txtheight = 15;
}

QString QBarcode::GetTypeText()
{
QString bcNames[] =
{
    "2_5_interleaved",
    "2_5_industrial",
    "2_5_matrix",
    "Code39",
    "Code39 Extended",
    "Code128A",
    "Code128B",
    "Code128C",
    "Code93",
    "Code93 Extended",
    "MSI",
    "PostNet",
    "Codebar",
    "EAN8",
    "EAN13"
};

return   bcNames[m_type];
}

/*
calculate the width and the linetype of a sigle bar

  Code   Line-Color      Width               Height
------------------------------------------------------------------
        '0'   white           100%                full
        '1'   white           100%*Ratio          full
        '2'   white           150%*Ratio          full
        '3'   white           200%*Ratio          full
        '5'   black           100%                full
        '6'   black           100%*Ratio          full
        '7'   black           150%*Ratio          full
        '8'   black           200%*Ratio          full
        'A'   black           100%                2/5  (used for PostNet)
        'B'   black           100%*Ratio          2/5  (used for PostNet)
        'C'   black           150%*Ratio          2/5  (used for PostNet)
        'D'   black           200%*Ratio          2/5  (used for PostNet)
*/

void  QBarcode::OneBarProps(char code, int &w, BarLineType &lt)
{
    switch(code) {
    case  '0': w = m_modules[0]; lt = white; break;
    case  '1': w = m_modules[1]; lt = white; break;
    case  '2': w = m_modules[2]; lt = white; break;
    case  '3': w = m_modules[3]; lt = white; break;

    case  '5': w = m_modules[0]; lt = black; break;
    case  '6': w = m_modules[1]; lt = black; break;
    case  '7': w = m_modules[2]; lt = black; break;
    case  '8': w = m_modules[3]; lt = black; break;

    case  'A': w = m_modules[0]; lt = black_half; break;
    case  'B': w = m_modules[1]; lt = black_half; break;
    case  'C': w = m_modules[2]; lt = black_half; break;
    case  'D': w = m_modules[3]; lt = black_half; break;
    default:
        // something went wrong  :-(
        // mistyped pattern table
        QMessageBox::warning(QApplication::activeWindow(),"Error",QString("%1: internal Error").arg(this->metaObject()->className()));
        break;
    }
}

QString QBarcode::MakeData()
{
QString res = "";

   // calculate the with of the different lines (modules)
    MakeModules();

    // get the pattern of the barcode
    switch(m_type){
    case  bcCode_2_5_interleaved: res = Code_2_5_interleaved(); break;
    case  bcCode_2_5_industrial:  res = Code_2_5_industrial(); break;
    case  bcCode_2_5_matrix:      res = Code_2_5_matrix();break;
    case  bcCode39:               res = Code_39();break;
    case  bcCode39Extended:       res = Code_39Extended();break;
    case  bcCode128A:
    case  bcCode128B:
    case  bcCode128C:             res = Code_128();break;
    case  bcCode93:               res = Code_93();break;
    case  bcCode93Extended:       res = Code_93Extended();break;
    case  bcCodeMSI:              res = Code_MSI();break;
    case  bcCodePostNet:          res = Code_PostNet();break;
    case  bcCodeCodabar:          res = Code_Codabar();break;
    case  bcCodeEAN8:             res = Code_EAN8();break;
    case  bcCodeEAN13:            res = Code_EAN13();break;
    default:
        QMessageBox::warning(QApplication::activeWindow(),"Errot",QString("%1: wrong BarcodeType").arg(this->metaObject()->className()));
        break;
    }

    return res;
}

int QBarcode::GetWidth()
{
QString data;
int  w, res, l;
BarLineType lt;

res = 0;

// get barcode pattern
data = MakeData();

l = data.length();

for(int i = 0;i < data.length();++i) //examine the pattern string
{
    OneBarProps(data.toLatin1().at(i), w, lt);

    res += w;
}

return res;
}

QString QBarcode::Code_EAN8()
{
QString res = "", tmp;

if (m_checkSum){
    tmp = '00000000' + m_text;
    tmp = getEAN(tmp.mid(tmp.length() - 6, 7) + '0');
}
else
    tmp = m_text;

res = "505";   // Startcode

for(int i = 0;i <  4;++i)
    for (int j = 0;j < 4;++j){
        res += tabelle_EAN_A[QString(m_text.at(i)).toInt()][j];
    }

res += "05050";   // Trennzeichen

for (int i = 4;i < 8;++i)
    for (int j = 0;j < 4;++j)
        res +=  tabelle_EAN_C[QString(m_text.at(i)).toInt()][j];

return   res += "505";   // Stopcode
}


//---------------------------- EAN13 --------------------------------
QString QBarcode::Code_EAN13()
{
int LK;
QString res = "", tmp;

if (m_checkSum) {
    tmp = "0000000000000" + m_text;
    tmp = getEAN(tmp.mid(tmp.length() - 11,12) + '0');
}else
    tmp = m_text;

LK = QString(tmp.toLatin1().at(0)).toInt();
tmp = tmp.mid(1,12);

res = "505";   // Startcode

for(int i = 0;i < 6;++i)
{
    switch(tabelle_ParityEAN13[LK][i]){
    case 'A': for(int j = 0;j < 4;++j)
            res += tabelle_EAN_A[QString(tmp.at(i)).toInt()][j] ; break;

    case 'B': for(int j = 0;j < 4;++j)
            res += tabelle_EAN_B[QString(tmp.at(i)).toInt()][j] ; break;

    case 'C': for(int j = 0;j < 4;++j)
            res += tabelle_EAN_C[QString(tmp.at(i)).toInt()][j] ; break;
    }
}

res += "05050";   // Trennzeichen

for(int i = 6;i <  12;++i)
    for(int j = 0;j < 4;++j)
        res += tabelle_EAN_C[QString(tmp.at(i)).toInt()][j] ;

return res + "505";   // Stopcode
}


QString QBarcode::Code_2_5_interleaved()
{
QString res = "";
char c;

res = "5050"; // Startcode

for(int i = 0;i < m_text.length() - 1;i += 2)
{
    for(int j = 0;j < 5;++j)
    {
        if(tabelle_2_5[QString(m_text.at(i)).toInt()][j] == '1')
            c = '6';
        else
            c = '5';

        res +=  QString(c);

        if(tabelle_2_5[QString(m_text.at(i + 1)).toInt()][j] == '1')
            c = '1';
        else
            c = '0';

        res +=  QString(c);
    }
}

res += "605";    // Stopcode

return res;
}

QString QBarcode::Code_2_5_industrial()
{
QString res = "";

res = "606050";   // Startcode

for(int i = 0;i <  m_text.length();++i)
{
    for(int j = 0;j <  5;++j)
    {
                if(tabelle_2_5[QString(m_text.at(i)).toInt()][j] == '1')
              res += "60";
              else
              res += "50";
}
}

res += "605060";   // Stopcode

return res;
}

QString QBarcode::Code_2_5_matrix()
{
QString res = "";
char c;

res = "705050";   // Startcode

for (int i = 0;i < m_text.length();++i)
{
    for(int j = 0;j <  5;++j)
    {
        if (tabelle_2_5[QString(m_text.at(i)).toInt()][j] == '1')
            c = '1';
        else
            c = '0';

        if(!(j % 2))
            c =  int(c) + 5;

        res += QString(c);

    }

    res +=  "0";
}

res +=  "70505";   // Stopcode

return res;
}


int FindIdx(char z)
{
int size =  sizeof(tabelle_39)/sizeof(Code39);

for(int i = 0;i < size;++i){

    if (z == tabelle_39[i].c)
        return i;
}

return -1;
}


QString QBarcode::Code_39()
{
QString res = "";
int idx, checksum, size =  sizeof(tabelle_39)/sizeof(Code39);
checksum = 0;

// Startcode
res = QString(tabelle_39[FindIdx('*')].data) + '0';

for(int i = 0;i < m_text.length();++i)
{
idx = FindIdx(m_text.toLatin1().at(i));

    if(idx < 0)
        continue;

    res += QString(tabelle_39[idx].data) + '0';
    checksum += tabelle_39[idx].chk;

}

// Calculate Checksum Data
if (m_checkSum)
{
    checksum %= 43;

    for (int i = 0;i <  size;++i)
        if (checksum == tabelle_39[i].chk)
            return    res += QString(tabelle_39[i].data) + '0';
}

// Stopcode
res += QString(tabelle_39[FindIdx('*')].data);

return res;
}

QString QBarcode::Code_39Extended()
{
QString save, res = "";
int c;

save = m_text;
m_text = "";

for (int i = 0;i < save.length();++i)
{
    c = int(save.toLatin1().at(i));

    if(c <= 127)
        m_text += code39x[c];
}

res = Code_39();
m_text = save;

return res;
}

int Find_Code128AB(char c,int type)
{
char v;

for (int i = 0;i < sizeof(tabelle_128)/sizeof(Code128);++i)
{
    if (type == QBarcode::bcCode128A)
        v = tabelle_128[i].a;
    else
        v = tabelle_128[i].b;

    if(c == v)
        return i;
}

return -1;
}

QString QBarcode::Code_128()
{
QString res = "", startcode;
const QString StartA = "211412",StartB ="211214",StartC = "211232",Stop  =  "2331112";
int  idx, checksum;

checksum = 0; // Added by TZ

switch (m_type){
case bcCode128A:   checksum = 103; startcode = StartA; break;
case bcCode128B:   checksum = 104; startcode = StartB; break;
case bcCode128C:   checksum = 105; startcode = StartC; break;
}

res = Convert(startcode);  // Startcode

if (m_type == bcCode128C) {
    for(int i = 0;i < m_text.length() / 2;++i)
    {
        // not implemented !!!
    }

}else
    for (int i = 0;i < m_text.length(); ++i)
    {
        idx = Find_Code128AB(m_text.toLatin1().at(i),m_type);

        if (idx < 0)
            idx = Find_Code128AB(' ',m_type);

        res +=  Convert(QString(tabelle_128[idx].data));

        checksum += idx * (i + 1);
    }

checksum %= 103;
res +=  Convert(QString(tabelle_128[checksum].data));
res += Convert(Stop);      // Stopcode

return res;
}


int Find_Code93(char c)
{
   for (int i = 0;i <  sizeof(tabelle_93)/sizeof(Code93);++i)
      if (c == tabelle_93[i].c)
                return i;

return -1;
}

QString QBarcode::Code_93()
{
QString res = "";
int  idx,checkC, checkK, weightC, weightK;

res = Convert("111141");   // Startcode

for (int i = 0;i < m_text.length();++i)
{
    idx = Find_Code93(m_text.toLatin1().at(i));


    if (idx < 0){
        QMessageBox::warning(QApplication::activeWindow(),"Error",QString("%1: Code93 bad Data <%2>").arg(this->metaObject()->className()).arg(m_text));
        return "";
    }

    res += Convert(QString(tabelle_93[idx].data));
}

checkC = 0;
checkK = 0;

weightC = 1;
weightK = 2;

for (int i = m_text.length() - 1;i >= 0;--i)
{
    idx = Find_Code93(m_text.toLatin1().at(i));

    checkC += idx * weightC;
    checkK += idx * weightK;

    weightC++;

    if(weightC > 20) weightC = 1;

    weightK++;

    if (weightK > 15) weightC = 1;
}

checkK += checkC;

checkC = checkC % 47;
checkK = checkK % 47;

res +=  Convert(QString(tabelle_93[checkC].data)) + QString(Convert(tabelle_93[checkK].data));
res +=  Convert("1111411");

return res;
}


QString QBarcode::Code_93Extended()
{
QString res = "", save;

save = m_text;
m_text = "";
int ck;


for (int i = 0;i < save.length();++i)
    if (save.toLatin1().at(i) <= 127){

        ck = int(save.toLatin1().at(i));

        m_text += code93x[int(save.toLatin1().at(i))];
    }

res = Code_93();
m_text = save;

return res;
}


QString QBarcode::Code_MSI()
{
QString res = "";
int check_even, check_odd, checksum;

res = "60";    // Startcode
check_even = 0;
check_odd  = 0;

for(int i = 0l;i < m_text.length();++i)
{
    if(!((i - 1) % 2))
        check_odd = check_odd * 10 + int(m_text.toLatin1().at(i));
    else
        check_even +=  int(m_text.toLatin1().at(i));

    res += tabelle_MSI[QString(m_text.toLatin1().at(i)).toInt()];

}

checksum = quersumme(check_odd * 2) + check_even;

checksum %=  10;

if (checksum > 0)
    checksum = 10 - checksum;

res += tabelle_MSI[QString(char(int('0') + checksum)).toInt()];

res += "515"; // Stopcode

return res;
}


QString QBarcode::Code_PostNet()
{
QString res = "";

res = "51";

for (int i = 0;i < m_text.length();++i)
    res += tabelle_PostNet[QString(m_text.toLatin1().at(i)).toInt()];

res += "5";

return res;
}

int Find_Codabar(char c)
{
    for(int i= 0;i <  sizeof(tabelle_cb)/sizeof(Codabar);++i)
        if( c == tabelle_cb[i].c)
            return i;


    return -1;
}

QString QBarcode::Code_Codabar()
{
QString res = "";
int idx;

res = QString(tabelle_cb[Find_Codabar('A')].data) + "0";

for (int i = 0;i < m_text.length();++i)
{
    idx = Find_Codabar(m_text.toLatin1().at(i));

    res += QString(tabelle_cb[idx].data) + "0";
}

res += QString(tabelle_cb[Find_Codabar('B')].data);

return res;
}

void QBarcode::MakeModules()
{
    switch(m_type) {
    case bcCode_2_5_interleaved:
    case bcCode_2_5_industrial:
    case bcCode39:
    case bcCodeEAN8:
    case bcCodeEAN13:
    case bcCode39Extended:
    case bcCodeCodabar:
                if (m_ratio < 2.0)  m_ratio = 2.0;
        if (m_ratio > 3.0)  m_ratio = 3.0;
        break;

    case bcCode_2_5_matrix:
                if (m_ratio < 2.25)  m_ratio = 2.25;
        if (m_ratio > 3.0)  m_ratio = 3.0;
        break;
    case bcCode128A:
    case bcCode128B:
    case bcCode128C:
    case bcCode93:
    case bcCode93Extended:
    case bcCodeMSI:
    case bcCodePostNet:   break;
    }

    m_modules[0] = m_module;
    m_modules[1] = abs(m_module * m_ratio);
    m_modules[2] = m_modules[1] * 3 / 2;
    m_modules[3] = m_modules[1] * 2;
}


void QBarcode::DoLines(QString data)
{
BarLineType lt;
int xadd,  w = 0, h = 0;
QPoint p1, p2, orgin;
QPen pen;
QBrush brush;

xadd = 0;
orgin.setX(0);
orgin.setY(0);

pen.setStyle(Qt::NoPen);
brush.setStyle(Qt::SolidPattern);

for (int i = 0;i <  data.length();++i)
{
    OneBarProps(data.toLatin1().at(i), w, lt);

    if(lt == black || lt == black_half)
        pen.setColor(Qt::black);
    else
        pen.setColor(Qt::white);

    brush.setColor(pen.color());

    if (lt == black_half)
        h = (m_height - (m_showText?m_txtheight:0)) * 2 / 5;
    else
        h = (m_height - (m_showText?m_txtheight:0));

    p1.setX(xadd);
    p1.setY(0);
    p2.setX(xadd + w - 1);
    p2.setY(h);

    m_Painter.setPen(pen);
    m_Painter.setBrush(brush);

    m_Painter.drawRect(QRect(p1, p2));

    xadd += w;
}
}

void QBarcode::Draw()
{
QFont font = QFont("Arial",8);
QString data;
QPen pen;
QBrush brush;
QRect rtxt, rect;
Qt::Alignment al = Qt::AlignHCenter | Qt::AlignVCenter;
int height = m_height, widht = GetWidth();

rect = QRect(0,0,widht,height);

m_pixmap = QPixmap(widht, height);

m_Painter.begin(&m_pixmap);

brush.setStyle(Qt::SolidPattern);
brush.setColor(Qt::white);

m_Painter.setBrush(brush);

m_Painter.fillRect(rect, brush);

data = MakeData();

DoLines(data);

if(m_showText)
{
    pen.setStyle(Qt::SolidLine);
    pen.setColor(Qt::black);
    pen.setWidth(1);
    brush.setColor(Qt::white);

    m_Painter.setBrush(brush);
    m_Painter.setFont(font);
    m_Painter.setPen(pen);

    rtxt = QRect(0,height - m_txtheight,widht, m_txtheight);

    m_Painter.fillRect(rtxt,brush);

    m_Painter.drawText(rtxt, al,m_text);
}

m_Painter.end();
}

QPixmap QBarcode::GetImage()
{
Draw();

return m_pixmap;
}
