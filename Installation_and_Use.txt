Installation and Use
Buid project file: asreport.pro.
After compiling the directory / build / debug-release will contain the shared library: asreportlib, file executable file demo program: asreportdemo, form files, and other files.
To use the linking library file: asreportlib. To the  project folder copy a dynamic link library file: asreportlib.dll (win).
The project also need to connect the modules: sql xml printsupport widgets.

The base class for working with the library - QASReport.

#include ".. /asreportlib/asreport.h "

//create an instance of the class QASReport.
m_Report  =  new QASReport(this);

//Adding to the report data sources - for output model
m_Report ->addModel("sqldatamodel",m_sqldatamodel);
m_Report ->addModel("stringsmodel", m_stringsmodel);
m_Report ->addModel("itemsmodel", m_itemsmodel);

//Load file report
QString filereport  =  QDir::currentPath()  +  "/forms/list.xml";

if(!QFile::exists(filereport)) return;

    QFile file(filereport);

    if (!file.open(QIODevice::ReadOnly))
        return;
	
bool res  = m_Report->load(&file);

//Output report on preview 
m_Report->reportView();

//Print
Report->reportPrint();

//Designing
m_Report->edit(filereport);

For more info see /Docs/help  and   demo program
