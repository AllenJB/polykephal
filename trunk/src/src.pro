SOURCES += main.cpp \
 PolykephalApp.cpp
TEMPLATE = app
CONFIG += warn_on thread qt
TARGET = polykephal
DESTDIR = ../bin
QT -= gui
QT += network
HEADERS += PolykephalApp.h \
 icecapmisc.h
