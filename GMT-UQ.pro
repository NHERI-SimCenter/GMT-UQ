#-------------------------------------------------
#
# Project created by QtCreator 2017-06-06T06:31:52
#
#-------------------------------------------------

QT       += core gui charts concurrent network sql qml webenginewidgets uitools webengine webchannel

TARGET = GMT
TEMPLATE = app

macos:LIBS += /usr/lib/libcurl.dylib -llapack -lblas
win32:INCLUDEPATH += "c:\Users\SimCenter\libCurl-7.59.0\include"
win32:LIBS += C:\Users\SimCenter\libCurl-7.59.0/lib/libcurl.lib
linux:LIBS += /usr/lib/x86_64-linux-gnu/libcurl.so

win32 {
    RC_ICONS = icons/NHERI-GMT-Icon.ico
} else {
    mac {
    ICON = icons/NHERI-GMT-Icon.icns
    }
}

include(../EE-UQ/EE-UQ.pri)
include(../SimCenterCommon/Workflow/Workflow.pri)
include(../SimCenterCommon/Common/Common.pri)
include(../SimCenterCommon/RandomVariables/RandomVariables.pri)
include(../SimCenterCommon/InputSheetBM/InputSheetBM.pri)
include(../GroundMotionUtilities/UI/GroundMotionWidgets.pri)
include(../SiteResponseTool/SiteResponseTool.pri)
include(./MiniZip/MiniZip.pri)

SOURCES += main.cpp \
    WorkflowAppGMT.cpp \
    ResultsGMT.cpp \
    LocationInformation.cpp \
    RunWidget.cpp \
    timeIntegrators.cpp \
    calcResponseSpectrum.cpp \
    qcustomplot.cpp \
    ResponseWidget.cpp

HEADERS  += \
    WorkflowAppGMT.h \
    ResultsGMT.h \
    LocationINformation.h \
    RunWidget.h \ 
    timeIntegrators.h \
    qcustomplot.h \
    ResponseWidget.h

RESOURCES += \
    #resources.qrc
    #../EE-UQ/images.qrc \


#FORMS    += mainwindow.ui

#RESOURCES += \
#    schema.qrc



