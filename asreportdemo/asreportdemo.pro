#-------------------------------------------------
#
# Project created by QtCreator 2016-11-06T10:22:21
#
#-------------------------------------------------

QT       += core gui sql xml printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET  = asreportdemo
TEMPLATE = app

include(../asreportlib/asreportconfig.pri)

INCLUDEPATH += $$PWD/../asreportlib/
   LIBS += -L$${DEST_DIRECTORY}/lib -lasreportlib

DESTDIR = $${DEST_DIRECTORY}

SOURCES += main.cpp\
        mainwindow.cpp \
    formlist.cpp \
    formdetaillist1.cpp \
    formdetaillist2.cpp \
    formgroups.cpp \
    formanymodel.cpp \
    formimages.cpp

HEADERS  += mainwindow.h \
    formlist.h \
    formdetaillist1.h \
    formdetaillist2.h \
    formgroups.h \
    formanymodel.h \
    formimages.h

FORMS    += mainwindow.ui \
    formlist.ui \
    formdetaillist1.ui \
    formdetaillist2.ui \
    formgroups.ui \
    formanymodel.ui \
    formimages.ui


win32 {
    MOC_DIR = tmp_win32
    UI_DIR = tmp_win32
    UI_HEADERS_DIR = tmp_win32
    UI_SOURCES_DIR = tmp_win32
    OBJECTS_DIR = tmp_win32
    RCC_DIR = tmp_win32
}

linux {
    MOC_DIR = tmp_lin64
    UI_DIR = tmp_lin64
    UI_HEADERS_DIR = tmp_lin64
    UI_SOURCES_DIR = tmp_lin64
    OBJECTS_DIR = tmp_lin64
    RCC_DIR = tmp_lin64
}


# Copy files
win32 {
    QMAKE_LIB_DESTDIR       = $$quote($$DEST_DIRECTORY/)
    QMAKE_LIB_TARGET        = $${replace(QMAKE_LIB_DESTDIR, /, \\)}

    QMAKE_POST_LINK += \
        xcopy /sy \"$$PWD\\files\\*\" \"$${QMAKE_LIB_TARGET}\"
}
else {
    QMAKE_LIB_DESTDIR       = $$quote($$DEST_DIRECTORY/)
    QMAKE_LIB_TARGET        = $$QMAKE_LIB_DESTDIR
    QMAKE_POST_LINK += \
        cp -rpv \"$$PWD/files/\"* \"$${QMAKE_LIB_TARGET}\" $$escape_expand(\n\t)
}

RESOURCES += \
    files.qrc





