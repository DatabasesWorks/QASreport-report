						QASreport   ver.1.02
						C++ QT library from ver.5.
						
Content
Description
Features
Installation and Use
License


1. Description
QASreport - is a multi-platform C ++ Qt library that contains a set of classes for building reports. It is a mix of designer and report generator output means. It is intended to add to the application of automation to create, save, report output. Reports templates are stored in XML format. And can be stored and loaded from a file on disk, memory, or table blob fields. The library contains built-in designer, available in run-time, with the ability to work like a normal graphic editor. Possible conclusion of the tables of any SQL database, which is connected to the driver. And the output of any source data models, inherited from QAbstractItemModel.

   The author does not guarantee the correct operation of the component and is not responsible for any damages resulting from the use of this software. All comments and suggestions to the address below.
   
2. Features
• Report generator based on the band;
• Built-in designer, available in run-time, with the ability to work like a normal graphic editor;
• The conclusion of the report results in the Preview;
• Any number of pages in the report;
• Multiple reports, the possibility of drawing up a number of reports and forms in a single file;
• Save the report file format - XML;
• Master-detail reports;
• Full control over the printing process, support of any type of paper and printing devices;
• Easy to design a report using simple components: a text box, line, image, shape, and bar-code;
• The output data of any database, which the driver is installed;
• The output data from any SQL model, QSqlQueryModel inheritor;
• Data output from any of the standard model, the inheritable QAbstractItemModel;
• Export reports to PDF files;
• Support for Qt Script (in the future);
• others.						

3. Installation and Use
To compile the library and demo program, run on a compilation of the group project file: asreport.pro.
After compiling the directory / build / debug-release will contain the shared library: asreportlib, file executable file demo program: asreportdemo, form files, and other files.
To use the linking library file: asreportlib. To the  project folder copy a dynamic link library file: asreportlib.dll (win).
By the project also need to connect the modules: sql xml printsupport widgets.
The base class for working with the library - QASReport.

Connecting the class file QASReport:
#include ".. /asreportlib/asreport.h "

//create an instance of the class QASReport.
m_Report  =  new QASReport(this);

m_Report  =  new QASReport(this);

//Adding to the report data sources - for output model
m_Report ->addModel("sqldatamodel ",m_sqldatamodel);
m_Report ->addModel("stringsmodel ", m_stringsmodel);
m_Report ->addModel("itemsmodel ", m_itemsmodel);

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

//If necessary, signal processing is initialized:
// Begin print
connect(m_Report,SIGNAL(signalBeginReport()),this,SLOT(slotBeginPrint())); 

//processing variables of custom reports
connect(m_Report,SIGNAL(signalGetValue(QString,QVariant&)),this,
SLOT(slotGetValue(QString,QVariant&))); 

// processing the output end of the print output
connect(m_Report,SIGNAL(signalEndReport ()),this,SLOT(slotEndReport())); 

// Process output in the transition to the next record output tabular data
//sending a pointer to the output model
connect(m_Report,SIGNAL(signalModelOnNextRec(QObject *),this,SLOT(slotModelOnNextRec (QObject *) )); 

// Process output when set to the first record of the table
// sending a pointer to the output model
connect(m_Report,SIGNAL(signalModelOnFirstRec (QObject *),this,SLOT(slotModelOnFirstRec (QObject *) ));

// Process of forming a new page
// sending to the current page number
connect(m_Report,SIGNAL(signalNewPage (QObject *),this,SLOT(slotNewPage (QObject *) ));

4. License
This version released by the author QASreport for free use under the terms of
LGPL.



autor: Zgukov Serg
Contact details: 
email: serg_@ukr.net
WWW:  









