#-------------------------------------------------
#
# Project created by QtCreator 2017-01-03T13:44:26
#
#-------------------------------------------------

TARGET = asreportlib
TEMPLATE = lib
CONFIG += dll
CONFIG += create_prl
CONFIG += link_prl

macx{
    CONFIG  -= dll
    CONFIG  += lib_bundle
    CONFIG  += plugin
}

linux{
    CONFIG += plugin
}


DEFINES += ASREPORTLIB_LIBRARY

include(asreportlib.pri)
include(asreportconfig.pri)

DLLDESTDIR = $${DEST_DIRECTORY}
DESTDIR    = $${DEST_DIRECTORY}/lib


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




