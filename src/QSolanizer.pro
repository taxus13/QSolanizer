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
    utils/qcustomplot.cpp \
    datastructure/solarpart.cpp \
    datastructure/day.cpp \
    datastructure/year.cpp \
    datastructure/month.cpp \
    utils/csvreader.cpp \
    ui/qsolanizer.cpp

HEADERS  += \
    utils/qcustomplot.h \
    datastructure/solarpart.h \
    datastructure/day.h \
    datastructure/year.h \
    datastructure/month.h \
    utils/csvreader.h \
    ui/qsolanizer.h

FORMS    += \
    ui/qsolanizer.ui

RC_FILE = qsolanizer.rc




