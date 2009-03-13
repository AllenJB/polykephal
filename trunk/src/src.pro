SOURCES += main.cpp \
 PolykephalApp.cpp \
 server.cpp \
 client_thread.cpp \
 IcecapEvent.cpp
TEMPLATE = app
CONFIG += warn_on thread qt
TARGET = polykephal
DESTDIR = ../bin
QT -= gui
QT += network
HEADERS += PolykephalApp.h \
 icecapmisc.h \
 server.h \
 client_thread.h \
 IcecapEvent.h
