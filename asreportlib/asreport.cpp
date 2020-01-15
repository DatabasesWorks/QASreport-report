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

#include "asreport_p.h"
#include "asreport.h"

QASReportPrivate::QASReportPrivate(QObject *parent):QObject(parent)
{
    m_xmlDoc = new QDomDocument("Report");

    m_ReportView = Q_NULLPTR;

    m_Printer = 0;

    //m_ConnectionName = "";

    m_Designer = 0;

    m_parentWidget = parent->inherits("QWidget")?(QWidget*)parent:Q_NULLPTR;

    m_reportName = "";
}

void QASReportPrivate::clearModels()
{
    QDesDataSource * dataSource;
    QList <QDesDataSource*> list = m_MapModels.values();

    while(list.count()){
        dataSource = list.at(0);

        delete dataSource;
        list.removeAt(0);
    }

    m_MapModels.clear();
}

QASReportPrivate::~QASReportPrivate()
{
    delete m_xmlDoc;

    clearModels();

}

void QASReportPrivate::slotPreviewClose()
{
  m_ReportView = 0;
}

void QASReportPrivate::reportView()
{
#ifdef SHOWWAITDIALOG
    QASWaitDialog waitdialog(Q_NULLPTR);

    waitdialog.setProcessMessage(tr("Processing page: %1"));
    waitdialog.setMessage(tr("Processing pages..."));//Processing page: 1
    waitdialog.setWindowTitle(tr("Preparing the report..."));
    waitdialog.setEnableCancel(false);

    QObject::connect(this,SIGNAL(signalNewPage(int)),&waitdialog,SLOT(slotUpdateText(int)));

    waitdialog.show();
#endif

    qApp->processEvents(QEventLoop::ExcludeUserInputEvents);

    if(!m_ReportView)
    {
        m_ReportView = new QASReportView(m_parentWidget,this);

        connect(m_ReportView,SIGNAL(destroyed()),this,SLOT(slotPreviewClose()));
    }

    QPrintPreviewWidget *preview = m_ReportView->PreviewWidget();

    connect(preview, SIGNAL(paintRequested(QPrinter*)), this, SLOT(PaintPreview(QPrinter*)));

    preview->setZoomFactor(1);

    preview->updatePreview();
    preview->setCurrentPage(1);

#ifdef SHOWWAITDIALOG
    waitdialog.close();
#endif

    m_ReportView->exec();
}

void QASReportPrivate::slotCancelPrintProcess()
{



}

void QASReportPrivate::reportPrint(bool show)
{   
    QPrinter printer;
    QPrintDialog dlg(&printer);

    dlg.setMinMax(1, m_curPage + 1);

    if (show){
        if(dlg.exec() == QDialog::Accepted)
            PaintPreview(&printer);
    }
    else{
        dlg.setVisible(false);

        PaintPreview(&printer);
    }

}

void QASReportPrivate::exportTo(TypeExport type)
{
    QPrinter printer;
    QString format;

    switch (type){
    case PDF:
        printer.setOutputFormat(QPrinter::PdfFormat);
        format = "*.pdf";
     break;
    }

    QString filename = QFileDialog::getSaveFileName(m_parentWidget, tr("Save Dialog"), "","File ("+ format +")");
    if(filename.isEmpty()) return;

     printer.setOutputFileName(filename);

    PaintPreview(&printer);

}

bool QASReportPrivate::load(QString xmltxt)
{
    if (!m_xmlDoc->setContent(xmltxt))
        return false;

    return true;
}

bool QASReportPrivate::load(QIODevice * dev)
{
    if (!m_xmlDoc->setContent(dev))
        return false;

    return true;
}

bool QASReportPrivate::load(QByteArray & buffer)
{
    if (!m_xmlDoc->setContent(buffer))
        return false;

    return true;
}

void QASReportPrivate::newPage()
{
if(!m_Printer) return;

m_curPage++;

m_ispaint = (!m_Printer->fromPage() && !m_Printer->toPage()) ||
              (m_curPage + 1 >= m_Printer->fromPage() && m_curPage + 1 <= m_Printer->toPage());

if(m_ispaint){
    if(m_firstprint) //
        m_Printer->newPage();

    m_firstprint = true;
}

if(m_ReportView)
     m_ReportView->PreviewWidget()->setCurrentPage(m_curPage + 1);
}

void QASReportPrivate::prepareReport()
{
QDomElement elem = m_xmlDoc->documentElement().childNodes().at(m_curReport).toElement();
qreal pxmx = GetPxOnUnitX(rmMill),pxmy = GetPxOnUnitY(rmMill);//pxmm = GetPxOnMil();

m_margLeft = elem.attribute("marginLeft").toInt() * pxmx;
m_margTop = elem.attribute("marginTop").toInt() * pxmy;
m_margBottom = elem.attribute("marginBottom").toInt() * pxmy;
m_margRight = elem.attribute("marginRight").toInt() * pxmx;
m_curOrient   = (QPrinter::Orientation)elem.attribute("orientation").toInt();
m_papeSize = (QPrinter::PaperSize)elem.attribute("papesize").toInt();
m_pageSize = GetPageSize(m_papeSize,m_curOrient,elem.attribute("widthMM").toInt(),elem.attribute("heightMM").toInt());

m_Printer->setPaperSize(m_papeSize);
m_Printer->setOrientation(m_curOrient);
m_Printer->setFullPage(true);//
//m_Printer->setPaperSize(m_pageSize,QPrinter::DevicePixel);

if(m_papeSize == QPrinter::Custom && elem.attribute("widthMM").toInt() && elem.attribute("heightMM").toInt())
 m_Printer->setPageSizeMM(QSizeF(elem.attribute("widthMM").toInt(),elem.attribute("heightMM").toInt()));

m_Printer->setPageMargins(m_margLeft,m_margTop,m_margRight,m_margBottom,QPrinter::DevicePixel);
}



int QASReportPrivate::getBackBr(QString value,int beg,QChar ch0,QChar ch1)
{
    int lb = 0;
    QChar c;

    for (int i = beg + 1; i < value.length(); ++i){
        c = value.at(i);

        if(c == ch0) lb++;
        if(c == ch1)
            if(lb){
                lb--;
                continue;
            } else
                return i;
    }

    return -1;
}

QVariant QASReportPrivate::getDBValue(QString param)
{
    QString table, field;
    QVariant varvalue;
    QDesDataSource * dataSource;

    table = param.section(".",1,1);
    field = param.section(".",2,2);

    dataSource = getDataSource(table, true);

    if(dataSource)
        varvalue = dataSource->value(field);

    return varvalue;
}

bool QASReportPrivate::isExpression(QString param)
{
bool ret = false;

    ret = param.indexOf('[') != -1 || param.indexOf(']') != -1 ||
            param.toUpper().indexOf("SUM(") != -1 || param.toUpper().indexOf("COUNT(") != -1 ||
            param.toUpper().indexOf("AVG(") != -1 || param.toUpper().indexOf("MAX(") != -1 ||
            param.toUpper().indexOf("MIN(") != -1 ||
            param.indexOf('+') != -1 || param.indexOf('-') != -1 || param.indexOf('/') != -1 ||
            param.indexOf('*') != -1 || param.indexOf('=') != -1;

 return ret;
}

QVariant QASReportPrivate::getResultFunc(QString function,QString param)
{
QVariant retval;
RepFunc repfunc;

for(int i = 0; i < m_ListFunc.count(); ++i) {
    repfunc = m_ListFunc.at(i);

    if(repfunc.bandindex == m_DrawIndex && repfunc.func == function && repfunc.param == param)
       {
        if(function.toUpper() == "AVG")
        retval = repfunc.value / repfunc.count;
        else
        retval = repfunc.value;
        break;
       }
}

return retval;
}

double QASReportPrivate::calcValue(QChar action,double basevalue, double calcvalue)
{
double value = basevalue;

    if(action == '+')  value += calcvalue;
    if(action == '-')  value -= calcvalue;
    if(action == '*')  value *= calcvalue;
    if(action == '/')  value /= calcvalue;
    if(action == '=')  value =  calcvalue;

return  value;
}

QVariant QASReportPrivate::getExpressValue(QString expression)
{
double basevalue = 0,calcvalue = 0;
QString strvar = "", strnum = "",  substr = "",substr1 = "", informule, funcname = "";
int pos, beg, end;
QChar ch, curAction = '=';
bool qsc = false, func = false, ok = false;
QVariant varvalue;
QSet<QChar> setArifm;
QSet<QChar> setNumber;

if(expression.isEmpty()) return 0.;

setArifm << '+' << '-' << '/' << '*' << '=';
setNumber <<'0'<<'1'<<'2'<<'3'<<'4'<<'5'<<'6'<<'7'<<'8'<<'9' << QLocale().decimalPoint();

informule = expression;

informule += "$";

for (int i = 0; i < informule.length(); ++i)
{
    ch = informule.at(i);

    if(!qsc)
        if(setNumber.contains(ch))
        {
            if(ch == QLocale().decimalPoint())
                                        ch = '.';

            strnum += ch;

            continue;
        }else
        {
            if(!strnum.isEmpty())
            {
                calcvalue = strnum.toDouble(&ok);

                if(!ok)calcvalue = 0;

                strnum = "";

                basevalue = calcValue(curAction,basevalue,calcvalue);
                calcvalue = 0;
            }
        }

    if(!func && (ch.toUpper() == 'S' || ch.toUpper() == 'A' || ch.toUpper() == 'C' || ch.toUpper() == 'M'))
        func = true;

    if(func)
    {
        funcname += ch.toUpper();

        if(funcname == "SUM" || funcname == "COUNT" || funcname == "AVG" || funcname == "MAX" || funcname == "MIN")
        {
            if((beg = informule.indexOf('(',i)) == -1) {func =  false; continue;}

            substr = informule.mid(i, beg - i + 1);
            substr = substr.trimmed().toUpper();
            substr1 = funcname.at(funcname.length() -1) + (QString)"(";

            if(substr != substr1) {func =  false; continue;}

            end = getBackBr(informule,beg,'(',')');

            if(end  == -1) {func =  false; continue;}

            substr = informule.mid(beg + 1, end - beg - 1);

            if(!substr.isEmpty())
            {
                varvalue = getResultFunc(funcname, substr);

                if(varvalue.isNull()) calcvalue = 0;
                else
                {
                    calcvalue = varvalue.toDouble(&ok);
                    if(!ok)calcvalue = 0;
                }

                basevalue = calcValue(curAction,basevalue,calcvalue);
            }

            substr = "";

            i += end - i;
            func =  false;

            funcname = "";

            if(qsc)
                strvar = "";

            continue;
        }

     //   continue;
    }


    if(setArifm.contains(ch))
    {
        curAction = ch;
        continue;
    }

    if(ch == '[')
    {
        qsc = true;
        continue;
    }

    if(qsc && ch != '[' && ch != ']')
    {
        strvar += ch;

        continue;
    }

    if(ch == ']')
    {
        if(!strvar.isEmpty())
        {
            varvalue = getParamValue(strvar);

            if(varvalue.isNull())  calcvalue = 0;
            else
            {
                calcvalue = varvalue.toDouble(&ok);

                if(!ok) calcvalue = 0;
            }

            basevalue = calcValue(curAction,basevalue,calcvalue);

            calcvalue = 0.;
            qsc = false;
            strvar = "";
            func = false;
            funcname = "";

        }

        continue;
    }

    if(ch == '(')
    {
        pos = getBackBr(informule,i,'(',')');

        if(pos == -1) continue;

        substr  =  informule.mid(i + 1, pos - i - 1);

        varvalue = getExpressValue(substr);

        calcvalue = varvalue.toDouble(&ok);

        if(!ok) calcvalue = 0.;

        basevalue = calcValue(curAction,basevalue,calcvalue);

        i += pos - i;

        substr = "";

        continue;
    }
}

varvalue = basevalue;

return varvalue;
}

QVariant QASReportPrivate::getParamValue(QString param)
{
QVariant varvalue;// = param;

if(param.indexOf("DB.",0) != -1)
    varvalue = getDBValue(param);

if(param.toUpper() == "PAGE#")
    varvalue = m_curPage + 1;

if(param.toUpper() == "DATE#")
     varvalue = QDate::currentDate();

if(param.toUpper() == "TIME#")
    varvalue = QTime::currentTime();

if(param.toUpper() == "LINETHROUGH#")
    varvalue = m_globCountRec;

if(param.toUpper() == "LINE#")
    varvalue = m_curLine;

if(param.toUpper() == "CURRENT#")
    varvalue = m_curLine;

if(param.toUpper() == "TOTALPAGES#")
    varvalue = m_Printer->toPage();

emit signalGetValue(param, varvalue);

return varvalue;
}

QString QASReportPrivate::formatValue(QVariant varvalue, QString format)
{
QString value,formatparam;
QStringList strlist;
int prec, typevalue, typeformat;
bool ok, boolval;
double numval;
QDateTime datetime;
QTime time;

if(format.isEmpty()) return varvalue.toString();

strlist = format.split("%");

if(strlist.count() > 0)
{
    typevalue = strlist.at(0).toInt(&ok);
    if(!ok) typevalue = tvText;
}

if(typevalue == tvText) return varvalue.toString();

if(strlist.count() > 1)
{
  typeformat = strlist.at(1).toInt(&ok);
  if(!ok) typeformat = 0;
}

if(strlist.count() > 2)
    formatparam =  strlist.at(2);

    value = "";

    switch (typevalue){
    case tvNumber:
        numval = varvalue.toDouble(&ok);

        if(ok){
               prec = formatparam.toInt(&ok);
               if(!ok) prec = 2;

            value = FormatFloat(numval,typeformat,prec);
        }
        break;

    case tvDate:
        datetime = varvalue.toDateTime();

        if(datetime.isValid() && !formatparam.isEmpty())
            value = datetime.toString(formatparam);
        else
            value = varvalue.toString();
         break;

    case tvTime:
        time = varvalue.toTime();

        if(time.isValid() && !formatparam.isEmpty())
            value = time.toString(formatparam);
        else
            value = varvalue.toString();
        break;

    case tvBoolean:
        if(varvalue.canConvert(QMetaType::Bool))
        {
            boolval = varvalue.toBool();

            strlist = formatparam.split(";");

            if(strlist.count() >= 2)
                value =  !boolval?strlist.at(0):strlist.at(1);
        }

        break;
    }

return   value;
}

QVariant QASReportPrivate::getVarValue(QString invalue)
{
  QString param = "", str = invalue;
  QVariant varvalue;
  int beg, end;

  beg = str.indexOf('[');

  if(beg == -1) return varvalue;

  end = getBackBr(str, beg,'[',']');

      if(end != -1)
      {
          param = str.mid(beg + 1,end - beg - 1);

          varvalue = getParamValue(param);
      }

  return varvalue;
}

QString QASReportPrivate::getValue(QString invalue,QString format)
{
    QString tmpvalue = "", value = "", param = "", str = invalue;
    QVariant varvalue;
    int beg, end;

    while ((beg = str.indexOf('[')) != -1)
    {
        end = getBackBr(str, beg,'[',']');

        value += str.mid(0,beg);

        if(end != -1)
        {
            param = str.mid(beg + 1,end - beg - 1);

            if(isExpression(param))
                varvalue = getExpressValue(param);
            else
                varvalue = getParamValue(param);

            if(!format.isEmpty()){
                tmpvalue = formatValue(varvalue,format);

                value += tmpvalue;

            }else
                value +=  varvalue.toString();
        }

        str = str.right(str.length() - end - 1);
    }

    value += str;

    while(value.indexOf("\n\n") != -1)
            value.replace("\n\n","\n");

    return value;
}

void QASReportPrivate::drawField(QDomNode node,int yband, int bandtop,bool listdata)
{
if(node.isNull()) return;
if(!m_ispaint) return;

bool ok;
QDomElement elem = node.toElement();
QFieldType  type = (QFieldType)elem.attribute("type").toInt();
int     top  = elem.attribute("Top").toInt() - bandtop + yband,
        left  = elem.attribute("Left").toInt() + m_margLeft,
        height = elem.attribute("Height").toInt(),
        width = elem.attribute("Width").toInt(),
        border = elem.attribute("Border","0").toInt(),
        borderstyle = elem.attribute("BorderStyle","1").toInt(),
        borderwidth =  elem.attribute("BorderWidth","1").toInt(),
        texttypeangle = elem.attribute("AngleType","0").toInt();
QColor backolor, fontcolor, bordercolor;
Qt::Alignment al = (Qt::Alignment)elem.attribute("TextAlignment",QString::number(Qt::AlignLeft)).toInt(&ok);
QFont font(elem.attribute("FontFamily","Arial"),elem.attribute("FontSize","10").toInt());
QPen pen;
QBrush brush;
QString value = elem.attribute("Text"), format = elem.attribute("Format");
qreal b, w, h;
QPointF p;
bool stretchable, wordwrap;


if(value.indexOf("[") != -1 && value.indexOf("]") != -1)
    value = getValue(value,format);
else
    if(!format.isEmpty())
        value = formatValue(value, format);

b = borderwidth, w = width, h = height;
p = QPointF(left,top);

backolor.setNamedColor(elem.attribute("BackColor","#ffffff"));
fontcolor.setNamedColor(elem.attribute("FontColor","#000000"));
bordercolor.setNamedColor(elem.attribute("BorderColor","#000000"));

font.setBold(elem.attribute("FontBold","false") == "true"?true:false);
font.setUnderline(elem.attribute("FontUnderline","false") == "true"?true:false);
font.setItalic(elem.attribute("FontItalic","false") == "true"?true:false);

if(elem.attribute("BackColor").isEmpty() || (backolor.red() == 0xff && backolor.green() == 0xff && backolor.blue() == 0xff))
  brush.setStyle(Qt::NoBrush);
else
  brush.setStyle(Qt::SolidPattern);

m_Painter.save();

brush.setColor(backolor);
pen.setColor(bordercolor);
pen.setWidth(borderwidth);
pen.setStyle((Qt::PenStyle)borderstyle);

m_Painter.setPen(pen);
m_Painter.setFont(font);
m_Painter.setBrush(brush);

if(type == ftText || type == ftPicture || type == ftBarCode)
{
    if(type == ftText)
    {
        wordwrap = elem.attribute("WordWrap","false") == "true"?true:false;
        stretchable = elem.attribute("Stretchable","false") == "true"?true:false;

        //correct height strechable fields
        if(stretchable)
        {
            QRect rect = m_Painter.boundingRect(left, top, width, 0, al | (wordwrap?Qt::TextWordWrap:0), value);

            if(width < rect.width()){
                width = rect.width();
                w = width;
            }

            if(height < rect.height()){
                height = rect.height();
                h = height;

            }else if(listdata){
                height += m_correctBandHeight;
                h = height;
            }
        }
    }

//if(fillrect)
m_Painter.fillRect(left,top,width,height,brush);

if(type == ftBarCode && !value.isEmpty())
{
    QBarcode barcode;
    QPixmap pixmap;
    double angle = 0;
    QPoint center;

    barcode.setType((QBarcode::BarcodeType)elem.attribute("BarcodeType","14").toInt());
    barcode.setText(value);
    barcode.setChecksum(elem.attribute("Checksum","false") == "true"?true:false);
    barcode.setShowText(elem.attribute("ShowText","true") == "true"?true:false);
    barcode.setHeight(angle == 0 || angle == 180?height:width);
    barcode.setRatio(elem.attribute("Ratio","2").toDouble());
    angle = elem.attribute("BarcodeAngle","0").toDouble();

    pixmap = barcode.GetImage();

           if(angle == 0 || angle == 180)
                       center = QPoint(left + pixmap.width()/2, top + pixmap.height()/2);

           if(angle == 90)
                       center = QPoint(left + pixmap.height()/2, top + pixmap.height()/2);

           if(angle == 270)
                       center = QPoint(left + pixmap.width()/2,top + pixmap.width()/2);

           m_Painter.save();

           m_Painter.translate(center);
           m_Painter.rotate(angle);
           m_Painter.translate(-center);

           m_Painter.drawPixmap(left,top, pixmap);

           m_Painter.restore();
}

if(border & 0x01)
    m_Painter.drawLine(p + QPointF(b/2,b/2),p + QPointF(b/2,h - b/2));

if(border >> 1 & 0x01)
    m_Painter.drawLine(p + QPointF(w - b/2,b/2),p + QPointF(w - b/2,h - b/2));

if(border >> 2 & 0x01)
    m_Painter.drawLine(p + QPointF(b/2,b/2),p + QPointF(w - b/2,b/2));

if(border >> 3 & 0x01)
    m_Painter.drawLine(p + QPointF(b/2, h - b/2), p + QPointF(w - b/2, h - b/2));

}

if(type == ftText && !value.isNull())
{
    if(texttypeangle == 0){ //QDesText::Horizontal
        pen.setColor(fontcolor);
        m_Painter.setPen(pen);

        m_Painter.setClipRect(left, top, width, height);
        m_Painter.setClipping(true);
        m_Painter.drawText(left + b + 2, top, width - b - 4, height, al | (wordwrap?Qt::TextWordWrap:0),value);
        m_Painter.setClipping(false);
    }else
    {
        m_Painter.save();
        m_Painter.translate(left,top + height);
        m_Painter.rotate(-90);

        m_Painter.setClipRect(0, 0, height, width);
        m_Painter.setClipping(true);
        m_Painter.drawText(b + 2, 0,height - b - 4, width, al | (wordwrap?Qt::TextWordWrap:0),value);
        m_Painter.setClipping(false);

        m_Painter.restore();
    }
}

if(type == ftPicture)
{
    m_Painter.setPen(pen);

    QPixmap pixmap;

     if(!elem.attribute("Source").isEmpty()){
         QVariant value = getVarValue(elem.attribute("Source"));

         if(value.isValid()){
             if(value.type() == QVariant::Pixmap && value.canConvert<QPixmap>())
                 pixmap = value.value<QPixmap>();

             if(value.type() == QVariant::ByteArray)
                 pixmap = QPixmap::fromImage(QImage::fromData(value.toByteArray()));
         }

     }else{
         QByteArray byteArray =  QByteArray::fromBase64(elem.attribute("Picture").toLatin1());
         pixmap = QPixmap::fromImage(QImage::fromData(byteArray, "PNG"));
     }

    m_Painter.drawPixmap(left,top,width,height,pixmap);

    if(border & 0x01)
        m_Painter.drawLine(p + QPointF(b/2,b/2),p + QPointF(b/2,h - b/2));

    if(border >> 1 & 0x01)
        m_Painter.drawLine(p + QPointF(w - b/2,b/2),p + QPointF(w - b/2,h - b/2));

    if(border >> 2 & 0x01)
        m_Painter.drawLine(p + QPointF(b/2,b/2),p + QPointF(w - b/2,b/2));

    if(border >> 3 & 0x01)
        m_Painter.drawLine(p + QPointF(b/2, h - b/2), p + QPointF(w - b/2, h - b/2));
}

if(type == ftLine)
{
    brush.setStyle(Qt::NoBrush);
    m_Painter.setBrush(brush);

    if(width > height)
        m_Painter.drawLine(left,top,left + width,top);
    else
        m_Painter.drawLine(left,top,left,top + height);
}

if(type == ftFigure)
{
    QPainterPath path;
    int typefigure = elem.attribute("FigureType","0").toInt();

    switch (typefigure) {
    case 0:
        m_Painter.drawRect(left,top,width,height);
        break;

    case 1:
        path.moveTo(left,top + height);
        path.lineTo(left + width/2,top);
        path.lineTo(left + width,top + height);
        path.closeSubpath();

        m_Painter.drawPath(path);
        break;

    case 2:
        //m_Painter.setBackgroundMode(Qt::OpaqueMode);
        path.addEllipse(left,top,width,height);
        path.closeSubpath();

         m_Painter.drawPath(path);
        break;

    case 3:
        path.addRoundedRect(left, top, width, height, 5, 5);
        path.closeSubpath();

         m_Painter.drawPath(path);
        break;
  }

}

m_Painter.restore();
}

QList<QString> QASReportPrivate::models()
{
return m_MapModels.keys();
}


QDesDataSource * QASReportPrivate::getDataSource(QString name, bool addonabsent)
{
    QMap<QString, QDesDataSource*>::const_iterator it = m_MapModels.find(name.toLower());
    QDesDataSource * dataSource;

    if(it != m_MapModels.end()){
        dataSource = it.value();

        return dataSource;
    }else{
        if(!addonabsent) return 0;

        bool find = false;
        QStringList list = QSqlDatabase::connectionNames();
        QSqlDatabase db;
        QSqlTableModel *model;

        foreach (QString conname, list){
            db = QSqlDatabase::database(conname, false);

            if(!db.isOpen()) continue;

            find = db.tables().indexOf(name) != -1;

            if(!find)
                find = db.tables().indexOf(name.toUpper()) != -1;

            if(find){
                model = new QSqlTableModel(this,db);
                model->setTable(name);

                if(model->select())
                    model->query().first();

                addModel(name.toLower(), model);

                it = m_MapModels.find(name.toLower());

                dataSource = it.value();

                return dataSource;
            }
        }
    }

    return 0;
}

QObject * QASReportPrivate::getModel(QString name, bool addonabsent)
{
    QMap<QString, QDesDataSource*>::const_iterator it = m_MapModels.find(name.toLower());
    QObject * model;

    if(it != m_MapModels.end()){
        model = it.value()->model();

        return model;
    }else
    {
        if(!addonabsent) return 0;

        bool find = false;
        QStringList list = QSqlDatabase::connectionNames();
        QSqlDatabase db;
        QSqlTableModel *model;

        foreach (QString conname, list){
            db = QSqlDatabase::database(conname, false);

            if(!db.isOpen()) continue;

            find = db.tables().indexOf(name) != -1;

            if(!find)
                find = db.tables().indexOf(name.toUpper()) != -1;

            if(find){
                model = new QSqlTableModel(this,db);
                model->setTable(name);

                if(model->select())
                    model->query().first();

                addModel(name.toLower(), model);

                return model;
            }
        }
    }

    return 0;
}

void QASReportPrivate::addModel(QString name,QObject* object)
{
    QMap<QString, QDesDataSource*>::const_iterator it = m_MapModels.find(name.toLower());
    QDesDataSource * dataSource;

    if(it == m_MapModels.end()){
        dataSource = new QDesDataSource((QAbstractItemModel*)object);

        m_MapModels[name.toLower()] = dataSource;
    }

}

QDomNode QASReportPrivate::getBand(QDomNode parent,QBandType type)
{
QDomNodeList nlist;
QDomNode retnode, node;

nlist = parent.toElement().elementsByTagName("Band");

for(int i = 0; i < nlist.count(); ++i)
{
  node = nlist.item(i);

  if(node.toElement().attribute("BandType").toInt() == type)
  {
       retnode = node;
       break;
  }
}

return retnode;
}

QDomNode QASReportPrivate::getBand(QBandType type,int beg, int end)
{
QDomNode retnode, node;

for (int i = beg; i < end; ++i){
      node = m_ListBands.at(i);

      if(node.toElement().attribute("BandType").toInt() == type)
      {
         retnode =  node;
         break;
      }
 }

return retnode;

}

QList<QDomNode> QASReportPrivate::getBandList(QBandType type,int beg, int end)
{
QDomNode node;
QList<QDomNode> list;

list.clear();

for (int i = beg; i < end; ++i){
      node = m_ListBands.at(i);

      if(node.toElement().attribute("BandType").toInt() == (int)type)
        list.append(node);
 }

return list;
}

void QASReportPrivate::resetAgregate(int bandindex)
{
RepFunc repfunc;

for (int a = 0; a < m_ListFunc.count(); ++a){
    repfunc = m_ListFunc.at(a);

    if(repfunc.bandindex == bandindex)
    {
        repfunc.value = 0;
        repfunc.count = 0;

        m_ListFunc.replace(a,repfunc);
    }
}

}

void QASReportPrivate::calcAgregate(QDomNode band)
{
QVariant value;
double d = 0;
RepFunc repfunc;
bool ok;
int type, rec, index;
QDomNode node;

for (int a = 0; a < m_ListFunc.count(); ++a){
    repfunc = m_ListFunc.at(a);

    node = m_ListBands.at(repfunc.bandindex);

    type = node.toElement().attribute("BandType").toInt();

    index = m_ListBands.indexOf(band);

    if(!(repfunc.dataindex == index || type == (int)btReportSummary))
        continue;

    value = getValue(repfunc.param);

    d = value.toDouble(&ok);

    if(!ok) d = 0.;

    repfunc.count++;

    rec = repfunc.count;

    if(repfunc.func == "SUM")
        repfunc.value += d;

    if(repfunc.func == "COUNT")
        repfunc.value = rec;

    if(repfunc.func == "AVG")
        repfunc.value += d;

    if(repfunc.func == "MAX")
    {
        if(d > repfunc.value)  repfunc.value = d;
    }

    if(repfunc.func == "MIN")
    {
        if(repfunc.count == 1)
            repfunc.value = d;
        else
            if(d < repfunc.value) repfunc.value = d;
    }

    m_ListFunc.replace(a,repfunc);
}
}

void QASReportPrivate::drawDataBand(QBandType type,int beg, int end)
{
    QString table;
    bool next, first = true, prev;
    QDomNode databand, band, childband, node;
    int index, nextindex = -1,childindex = -1, endseek, tmpcurline;
    QBandType btype;
    QVariant value;
    QList <GroupHeader>listgroupheader;
    GroupHeader groupheader;
    QDesDataSource * dataSource;

    databand = getBand(type,beg,end);

    if(databand.isNull()) return;

    index = m_ListBands.indexOf(databand);

    if(type == btMasterData)
    {
        if(beg == 0)
            drawBandByType(m_pageNode,btColumnHeader);

        btype = btMasterHeader;

        band = getBand(btMasterData,index + 1, m_ListBands.count());

        if(!band.isNull())
            nextindex = m_ListBands.indexOf(band);

        //collect group bands
        QList <QDomNode> listheader = getBandList(btGroupHeader,0, m_ListBands.count()),
                listfooter = getBandList(btGroupFooter,0, m_ListBands.count());

        listgroupheader.clear();
        for (int i = 0; i < listheader.count(); ++i){
            node = listheader.at(i);

            groupheader.BandHeader  = node;

            if(listfooter.count() - 1 >= i)
                groupheader.BandFooter =  listfooter.at(i);

            listgroupheader.append(groupheader);
        }
    }

    if(type == btDetailData)
        btype = btDetailHeader;

    band = getBand(btype, beg, index);

    if(!band.isNull())
    {
        m_DrawIndex = m_ListBands.indexOf(band);

        drawBand(band);

        m_YBand += band.toElement().attribute("Height").toInt();
    }

    table = databand.toElement().attribute("Source");

    dataSource = getDataSource(table, true);

    if(!dataSource) return;

    next = dataSource->first();

    emit signalModelOnFirstRec(dataSource->model());

    while (next){        
        //if(expectNextPage())
        if(expectNextPage(databand.toElement().attribute("Height").toInt()))
            if(m_curPage)
                drawBandByType(m_pageNode,btColumnHeader);

        //draw group band
        if(type == btMasterData)
        {
            //draw group footer
            if(!first)
            {
                for (int h = listgroupheader.count() - 1; h >= 0; --h){
                    groupheader = listgroupheader.at(h);

                    value = getValue(groupheader.BandHeader.toElement().attribute("Source"));

                    if(groupheader.value == value)continue;
                    if(groupheader.BandFooter.isNull()) continue;

                    prev =  dataSource->previous();

                    if(prev){
                        if(expectNextPage(groupheader.BandFooter.toElement().attribute("Height").toInt()))
                            if(m_curPage)
                                drawBandByType(m_pageNode,btColumnHeader);

                        m_DrawIndex = m_ListBands.indexOf(groupheader.BandFooter);

                        drawBand(groupheader.BandFooter);
                        m_YBand += groupheader.BandFooter.toElement().attribute("Height").toInt();

                        dataSource->next();

                        emit signalModelOnNextRec(dataSource->model());

                        resetAgregate(m_ListBands.indexOf(groupheader.BandFooter));
                    }
                }
            }


            for (int h = 0; h < listgroupheader.count(); ++h){
                groupheader = listgroupheader.at(h);

                value = getValue(groupheader.BandHeader.toElement().attribute("Source"));

                if(groupheader.value == value)continue;

                //draw group header
                if(expectNextPage(groupheader.BandHeader.toElement().attribute("Height").toInt()))
                    if(m_curPage)
                        drawBandByType(m_pageNode,btColumnHeader);

                m_DrawIndex = m_ListBands.indexOf(groupheader.BandHeader);

                drawBand(groupheader.BandHeader);
                m_YBand += groupheader.BandHeader.toElement().attribute("Height").toInt();

                m_curLine = 0;

                groupheader.value = value;

                listgroupheader.replace(h,groupheader);
            }
        }

        m_globCountRec++;
        m_curLine++;

        //calc agregates field
        calcAgregate(databand);

        m_DrawIndex = index;

        drawBand(databand);
        m_YBand += databand.toElement().attribute("Height").toInt();


        //draw child bands
        if(type == btMasterData)
        {
            endseek =  nextindex != -1?nextindex:m_ListBands.count();
            childindex = index;

            while(!(childband = getBand(btDetailData,childindex + 1, endseek)).isNull())
            {
                if(!childband.isNull())
                {
                    tmpcurline = m_curLine;
                    drawDataBand(btDetailData,childindex + 1, endseek);
                    m_curLine = tmpcurline;

                }

                childindex = m_ListBands.indexOf(childband);
            }
        }

        next = dataSource->next();

        emit signalModelOnNextRec(dataSource->model());

        first = false;
    }

    endseek = end;

    if(type == btMasterData)
    {
        btype = btMasterFooter;

        endseek = nextindex != -1?nextindex:m_ListBands.count();


        //draw group footer
        prev =  dataSource->previous();
        if(prev){
            for (int h = listgroupheader.count() - 1; h >= 0; --h){
                groupheader = listgroupheader.at(h);

                if(expectNextPage(groupheader.BandFooter.toElement().attribute("Height").toInt()))
                    if(m_curPage)
                        drawBandByType(m_pageNode,btColumnHeader);

                m_DrawIndex = m_ListBands.indexOf(groupheader.BandFooter);

                drawBand(groupheader.BandFooter);
                m_YBand += groupheader.BandFooter.toElement().attribute("Height").toInt();
            }

            dataSource->next();

            emit signalModelOnNextRec(dataSource->model());
        }
    }

    if(type == btDetailData)
        btype = btDetailFooter;

    band = getBand(btype,index + 1, endseek);

    if(!band.isNull())
    {
        if(expectNextPage(band.toElement().attribute("Height").toInt()))
            if(m_curPage)
                drawBandByType(m_pageNode,btColumnHeader);

        m_DrawIndex = m_ListBands.indexOf(band);

        drawBand(band);

        resetAgregate(m_ListBands.indexOf(band));


        m_YBand += band.toElement().attribute("Height").toInt();
    }

    if(nextindex != -1)
    {
        tmpcurline = m_curLine;
        drawDataBand(btMasterData,index + 1, end);
        m_curLine = tmpcurline;
    }
    else
        drawBandByType(m_pageNode,btColumnFooter);
}

int QASReportPrivate::corrStretchBandHeight(QDomNode band)
{
QString value;
QDomNode node =  band.firstChild();
QDomElement elem;
int top, left,height, width, bandtop, bandheight,tb = 0, correct = 0;
Qt::Alignment al;
QFont font;
QRect rect;
bool ok, wordwrap;

while(!node.isNull()){
    elem = node.toElement();

    if(elem.tagName() != "Field" ||
            elem.attribute("type") != "0" ||
            elem.attribute("Stretchable","false") != "true"){node = node.nextSibling(); continue;}

    value = elem.attribute("Text");

    if(value.indexOf("[") != -1 && value.indexOf("]") != -1)
        value = getValue(value,elem.attribute("Format"));

    bandtop = band.toElement().attribute("Top").toInt();
    bandheight = band.toElement().attribute("Height").toInt();

    top  = elem.attribute("Top").toInt() - bandtop + m_YBand;
    left  = elem.attribute("Left").toInt() + m_margLeft;
    height = elem.attribute("Height").toInt();
    width = elem.attribute("Width").toInt();
    wordwrap = elem.attribute("WordWrap","false") == "true"?true:false;

    al = (Qt::Alignment)elem.attribute("TextAlignment",QString::number(Qt::AlignLeft)).toInt(&ok);
    font = QFont(elem.attribute("FontFamily","Arial"),elem.attribute("FontSize","10").toInt());

    m_Painter.setFont(font);

    rect = m_Painter.boundingRect(left, top, width, 0, al | (wordwrap?Qt::TextWordWrap:0), value);

   if(height < rect.height())
       if((top + rect.height()) > (m_YBand + bandheight)){
           tb = (top + rect.height()) - (m_YBand + bandheight);

           if(correct < tb) correct = tb;
       }

    node = node.nextSibling();
}

return correct;
}

void QASReportPrivate::drawBand(QDomNode band)
{
QDomNode node = band.firstChild();
int bandtop, bandtype;
bool stretchable, listdata = false;

if(node.isNull()) return;

m_correctBandHeight = 0;

bandtop = band.toElement().attribute("Top").toInt();
stretchable = band.toElement().attribute("Stretchable","false") == "true"?true:false;

if(stretchable){
    m_correctBandHeight = corrStretchBandHeight(band);

    bandtype = band.toElement().attribute("BandType").toInt();

    listdata = bandtype == btMasterHeader || bandtype == btMasterData || bandtype == btMasterFooter ||
               bandtype == btDetailHeader ||  bandtype == btDetailData || bandtype == btDetailFooter ||
               bandtype == btColumnHeader || bandtype == btGroupHeader || bandtype == btGroupFooter;
}

while(!node.isNull()){    
    if(node.toElement().tagName() == "Field")
               drawField(node, m_YBand, bandtop, listdata);

    node = node.nextSibling();
}

m_YBand += m_correctBandHeight;
}

bool QASReportPrivate::expectNextPage(int calcheight)
{
    if(m_YBand + m_heightFooter + calcheight > m_pageSize.height() - m_margBottom - PAGEBOTTMARG){

        m_YBand = m_pageSize.height() - m_margBottom - m_heightFooter;

        drawBandByType(m_pageNode,btPageFooter);

        newPage();

        m_YBand = m_margTop;

        drawBandByType(m_pageNode,btPageHeader);

        emit signalNewPage(m_curPage);

        //qApp->processEvents(QEventLoop::ExcludeUserInputEvents);//

        return true;
    }

    return false;
}

int QASReportPrivate::getDataIndex(QDomNode node)
{
QList<QDomNode> list;
int prentindex = -1, index, type, bandtype = -1, addbandtype = -1, addparentindex = -1;

if(node.isNull()) return -1;

type = node.toElement().attribute("BandType").toInt();

if(type == btMasterFooter)
       bandtype = btMasterData;

if(type == btDetailFooter)
       bandtype = btDetailData;

if(type == btGroupFooter){
    bandtype = btMasterData;
    addbandtype = btDetailData;
}

if(bandtype == -1 && addbandtype == -1) return -1;

index = m_ListBands.indexOf(node);

list = getBandList((QBandType)bandtype, 0, index);

if(list.count())
        prentindex = m_ListBands.indexOf(list.at(list.count() - 1));

if(addbandtype != -1){
    list = getBandList((QBandType)addbandtype, 0, index);

    if(list.count())
            addparentindex = m_ListBands.indexOf(list.at(list.count() - 1));

    if(prentindex < addparentindex) prentindex = addparentindex;
}

return prentindex;
}


void QASReportPrivate::drawBandByType(QDomNode parent,QBandType type)
{
QDomNode node = getBand(parent,type);

if(node.isNull()) return;

m_DrawIndex = m_ListBands.indexOf(node);

drawBand(node);

m_YBand += node.toElement().attribute("Height").toInt();
}

void QASReportPrivate::updateAggregates()
{
QString value = "", func,substr, substr1;
int beg, end, fb, type, typeband, parentindex = -1;
QDomNode parent, node;
QDomNodeList nodelist;
QStringList strlist = QString(AGREGATEFUNCS).split(',');
RepFunc repfunc;

m_ListFunc.clear();

nodelist = m_pageNode.toElement().elementsByTagName("Field");

for (int n = 0; n < nodelist.count(); ++n) {
    node = nodelist.at(n);
    parent = node.parentNode();
    type = node.toElement().attribute("type").toInt();

    if(type != (int)ftText) continue;

    if(parent.toElement().tagName() != "Band")continue;

    typeband  = parent.toElement().attribute("BandType").toInt();

    if(!(typeband == (int)btMasterFooter || typeband == (int)btDetailFooter || typeband == (int)btReportSummary ||
       typeband == (int)btGroupFooter))
       continue;

    value = node.toElement().attribute("Text");

    if(value.isEmpty()) continue;

    parentindex = getDataIndex(parent);

    for (int i = 0; i < strlist.count(); ++i){
       func = strlist.at(i);

       if((fb = value.toUpper().indexOf(func)) == -1)  continue;

       if((beg = value.indexOf('(',fb)) == -1)  continue;

       substr = value.mid(fb, beg - fb + 1);
       substr = substr.trimmed().toUpper();
       substr1 = func + (QString)"(";

       if(substr != substr1) continue;

       end = getBackBr(value,beg,'(',')');

       if(end  == -1) continue;

       substr = value.mid(beg + 1, end - beg - 1);

       repfunc.func = func;
       repfunc.param = substr;
       repfunc.value = 0.;
       repfunc.count = 0;
       repfunc.dataindex = parentindex;
       repfunc.bandindex = m_ListBands.indexOf(parent);

       m_ListFunc.append(repfunc);
     }

}

foreach (repfunc, m_ListFunc) {
   func =  repfunc.func;
}
}

void QASReportPrivate::sortNodeList(QList<QDomNode> &nodelist,QString prop)
{
  QDomNode node1,node2;
  int vmin,v;

  for (int i = 0; i < nodelist.count(); ++i){
          node1 = nodelist.at(i);
           vmin = node1.toElement().attribute(prop).toInt();

          for (int ii = i + 1; ii < nodelist.count(); ++ii){
                node2 = nodelist.at(ii);
                v = node2.toElement().attribute(prop).toInt();

                if(v < vmin){
                    nodelist.swap(i,ii);
                     vmin = v;
                }
          }
    }
}

void QASReportPrivate::updateBandList()
{
    QDomNodeList nlist;
    QDomNode node;

    m_ListBands.clear();

    nlist = m_pageNode.toElement().elementsByTagName("Band");

    for (int i = 0; i < nlist.count(); ++i) {
        node = nlist.at(i);

        m_ListBands.append(node);
    }

    sortNodeList(m_ListBands,"Top");
}

//ProcessReport//
void QASReportPrivate::processReport()
{
QDomNode node;

m_YBand = m_margTop;
m_heightHeader = 0;

m_pageNode = m_xmlDoc->documentElement().childNodes().at(m_curReport);

m_globCountRec = 0;
m_curLine = 0;

updateBandList();

updateAggregates();

drawBandByType(m_pageNode,btReportTitle);

drawBandByType(m_pageNode,btPageHeader);

node = getBand(m_pageNode,btPageFooter);

if(!node.isNull())
    m_heightFooter = node.toElement().attribute("Height").toInt();
else
    m_heightFooter = 0;


//draw free fields
node = m_pageNode.firstChild();
while(!node.isNull()){

  if(node.toElement().tagName() != "Field")  { node = node.nextSibling(); continue;}

    drawField(node,m_heightHeader + m_margTop);

    node = node.nextSibling();
}

//draw db data
drawDataBand(btMasterData,0,m_ListBands.count());

drawBandByType(m_pageNode,btReportSummary);

if(m_heightFooter)
{
m_YBand = m_pageSize.height() - m_heightFooter;

drawBandByType(m_pageNode,btPageFooter);
}

}

int QASReportPrivate::pageCount()
{
 return  m_curPage + 1;
}

void QASReportPrivate::PaintPreview(QPrinter *printer)
{
#ifdef QT_NO_PRINTER
    Q_UNUSED(printer);
#else

    emit signalBeginReport();

    QDomNodeList nodelist;

    m_Printer = printer;

    if(!m_Printer)
    {
         QMessageBox::warning(m_parentWidget,tr("Error"),tr("Not available printer is found!"),QMessageBox::Ok);
        return;
    }

    m_curReport = 0;
    m_curPage = 0;

    prepareReport();

    //define printing page sets in print dialog
    m_ispaint = (!m_Printer->fromPage() && !m_Printer->toPage()) ||
                  (m_curPage + 1 >= m_Printer->fromPage() && m_curPage + 1 <= m_Printer->toPage());

    m_firstprint = m_ispaint;

    nodelist = m_xmlDoc->elementsByTagName("Report");

    m_Painter.begin(printer);

    for (int i = 0; i < m_xmlDoc->documentElement().childNodes().count(); i++)
    {
        if(m_xmlDoc->documentElement().childNodes().at(i).toElement().tagName() != "Report")
            continue;

        m_curReport = i;

        processReport();

        if(m_curReport < nodelist.count() - 1)
            newPage();
    }

    emit signalEndReport();

    m_Painter.end();

    m_Printer = 0;

#endif

}

//--------------------------------------------
void QASReportPrivate::slotDesignerClose()
{
 m_Designer = 0;
}

void  QASReportPrivate::createDesigner()
{
    m_Designer = new QASRepDesigner(m_parentWidget,this);

    connect(m_Designer,SIGNAL(destroyed()),this,SLOT(slotDesignerClose()));

    m_Designer->setWindowModality(Qt::ApplicationModal);
}

void QASReportPrivate::runDesigner()
{
createDesigner();

m_Designer->showMaximized();
}

void QASReportPrivate::edit(QString filename)
{
createDesigner();

    m_Designer->loadFromFile(filename);
    m_Designer->showMaximized();
}

void QASReportPrivate::edit(QIODevice * dev)
{
createDesigner();

    m_Designer->loadFromStream(dev);
    m_Designer->showMaximized();
}

void QASReportPrivate::edit(QByteArray & buffer)
{
createDesigner();

    m_Designer->loadFromStream(buffer);
    m_Designer->showMaximized();
}



//-------------------------------------------------------------------------------
QASReport::QASReport(QObject *parent):QObject(parent),d_ptr(new QASReportPrivate(parent))
{
    Q_D(QASReport);
    d->q_ptr = this;

    connect(d,SIGNAL(signalBeginReport()),this,SIGNAL(signalBeginReport()));
    connect(d,SIGNAL(signalEndReport()),this,SIGNAL(signalEndReport()));
    connect(d,SIGNAL(signalGetValue(QString,QVariant&)),this,SIGNAL(signalGetValue(QString,QVariant&)));
    connect(d,SIGNAL(signalModelOnFirstRec(QObject*)),this,SIGNAL(signalModelOnFirstRec(QObject*)));
    connect(d,SIGNAL(signalModelOnNextRec(QObject*)),this,SIGNAL(signalModelOnNextRec(QObject*)));
    connect(d,SIGNAL(signalNewPage(int)),this,SIGNAL(signalNewPage(int)));
    connect(d,SIGNAL(signalPrepearReport()),this,SIGNAL(signalPrepearReport()));
    connect(d,SIGNAL(signalSaveToStream(QDomDocument*)),this,SIGNAL(signalSaveToStream(QDomDocument*)));

}

void QASReport::reportView()
{
   Q_D(QASReport);
   d->reportView();
}

QASReport::~QASReport()
{
    delete d_ptr;
}


void QASReport::reportPrint(bool show)
{
   Q_D(QASReport);
   d->reportPrint(show);
}

void QASReport::exportTo(int type)
{
    if(type < QASReportPrivate::PDF || type > QASReportPrivate::PDF) return;

    Q_D(QASReport);
    d->exportTo((QASReportPrivate::TypeExport)type);
}

QPrinter * QASReport::printer()
{
  Q_D(QASReport);
  return d->printer();
}

QList<QString> QASReport::models()
{
    Q_D(QASReport);
    return d->models();
}

QObject * QASReport::getModel(QString name,bool addonabsent)
{
    Q_D(QASReport);
    return d->getModel(name,addonabsent);
}

void QASReport::addModel(QString name,QObject* object)
{
    Q_D(QASReport);
    d->addModel(name,object);
}

void QASReport::clearModels()
{
   Q_D(QASReport);
   d->clearModels();
}

int QASReport::pageCount()
{
   Q_D(QASReport);
   return  d->pageCount();
}

bool QASReport::load(QString filename)
{
   Q_D(QASReport);
   return d->load(filename);

}

bool QASReport::load(QIODevice * dev)
{
    Q_D(QASReport);
    return d->load(dev);
}

bool QASReport::load(QByteArray & buffer)
{
    Q_D(QASReport);
    return d->load(buffer);

}

void QASReport::runDesigner()
{
    Q_D(QASReport);
    d->runDesigner();
}

void QASReport::edit(QString filename)
{
    Q_D(QASReport);
    d->edit(filename);

}

void QASReport::edit(QIODevice * dev)
{
    Q_D(QASReport);
    d->edit(dev);
}

void QASReport::edit(QByteArray & buffer)
{
    Q_D(QASReport);
    d->edit(buffer);
}

QString  QASReport::reportName()
{
    Q_D(QASReport);
    return d->reportName();
}

void QASReport::setReportName(QString value)
{
   Q_D(QASReport);
   d->setReportName(value);
}

QASReport::QASReport(QASReportPrivate & prep,QObject *parent):QObject(parent),d_ptr(&prep)
{
    Q_D(QASReport);
    d->q_ptr = this;

    connect(d,SIGNAL(signalBeginReport()),this,SIGNAL(signalBeginReport()));
    connect(d,SIGNAL(signalEndReport()),this,SIGNAL(signalEndReport()));
    connect(d,SIGNAL(signalGetValue(QString,QVariant&)),this,SIGNAL(signalGetValue(QString,QVariant&)));
    connect(d,SIGNAL(signalModelOnFirstRec(QObject*)),this,SIGNAL(signalModelOnFirstRec(QObject*)));
    connect(d,SIGNAL(signalModelOnNextRec(QObject*)),this,SIGNAL(signalModelOnNextRec(QObject*)));
    connect(d,SIGNAL(signalNewPage(int)),this,SIGNAL(signalNewPage(int)));
    connect(d,SIGNAL(signalPrepearReport()),this,SIGNAL(signalPrepearReport()));
    connect(d,SIGNAL(signalSaveToStream(QDomDocument*)),this,SIGNAL(signalSaveToStream(QDomDocument*)));


}



