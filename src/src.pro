SOURCES += main.cpp \
 PolykephalApp.cpp \
 server.cpp \
 client_thread.cpp
TEMPLATE = app
CONFIG += warn_on thread qt
TARGET = polykephal
DESTDIR = ../bin
QT -= gui
QT += network
HEADERS += PolykephalApp.h \
 server.h \
 client_thread.h \
 icecapmisc.h
