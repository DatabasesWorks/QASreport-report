QT       +=  sql script xml

QT       -= gui

greaterThan(QT_MAJOR_VERSION, 4): QT += printsupport

SOURCES += asreportlib.cpp\
    asdatasource.cpp \
    asrepdesigner.cpp \
    asreport.cpp \
    asreportview.cpp \
    aswaitdialog.cpp \
    utils/calcutils.cpp \
    descontrols.cpp \
    desdialogs.cpp \
    deseditors.cpp \
    designpage.cpp \
    despropeditor.cpp \
    despropinspector.cpp \
    dessettings.cpp \
    destextedit.cpp \
    desutils.cpp \
    desvariables.cpp \
    barcode/barcode.cpp
	
HEADERS += asreportlib.h\
    asreportlib_global.h\
    asdatasource.h \
    asrepdesigner.h \
    asreport.h \
    asreport_p.h \
    asreportview.h \
    aswaitdialog.h \
    utils/calcutils.h \
    descontrols.h \
    desdefdata.h \
    desdialogs.h \
    deseditors.h \
    designpage.h \
    desinterface.h \
    despropeditor.h \
    despropinspector.h \
    dessettings.h \
    destextedit.h \
    desutils.h \
    desvariables.h \
    barcode/barcode.h \
    barcode/barcodedata.h

FORMS += \
    asrepdesigner.ui \
    asreportview.ui \
    aswaitdialog.ui \
    dessettings.ui \
    destextedit.ui \
    desvariables.ui

RESOURCES += \
    images.qrc
	
	
