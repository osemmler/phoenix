#-------------------------------------------------
#
# Project created by QtCreator 2017-10-16T22:20:29
#
#-------------------------------------------------

# generate systemconfig.h
system($PWD/../script/generate_systemconfig.sh)

QT       += core gui svg
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = phoenix
DESTDIR = ../bin
OBJECTS_DIR = ../build
MOC_DIR = ../build
RCC_DIR = ../build
UI_DIR = ../build

TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += Monster

SOURCES += *.cpp
HEADERS += *.h
FORMS   += *.ui

SOURCES += Monster/*.cpp
HEADERS += Monster/*.h

#SOURCES += main.cpp\
#        widget.cpp \
#    config.cpp \
#    update.cpp \
#    globals.cpp \
#    comm.cpp \
#    backlight.cpp \
#    phoenix.cpp

#HEADERS  += widget.h \
#    config.h \
#    update.h \
#    globals.h \
#    systemconfig.h \
#    comm.h \
#    backlight.h \
#    phoenix.h

DEPENDPATH += "."

RESOURCES += \
    resources.qrc

contains(QMAKE_HOST.arch, arm.*):{
    #raspberry
    LIBS += -lrf24
}else{
    #desktop
}
