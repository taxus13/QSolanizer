#-------------------------------------------------
#
# Project created by QtCreator 2015-04-18T14:10:11
#
#-------------------------------------------------

QT       += core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = QSolanizer
TEMPLATE = app


SOURCES += main.cpp\
        qsolanizer.cpp \
    csvreader.cpp \
    qcp/qcustomplot.cpp \
    datastructure/solarpart.cpp \
    datastructure/day.cpp \
    datastructure/year.cpp \
    datastructure/month.cpp

HEADERS  += qsolanizer.h \
    csvreader.h \
    qcp/qcustomplot.h \
    datastructure/solarpart.h \
    datastructure/day.h \
    datastructure/year.h \
    datastructure/month.h

FORMS    += qsolanizer.ui


win32::RC_ICONS += qsolanizer.ico
#CONFIG += qwt

#win32 {
#  INCLUDEPATH += \
#    C:\Qwt-6.1.2\include
#
#  LIBS += \
#   -LC:\Qwt-6.1.2\lib \
#   -lqwtd

#}