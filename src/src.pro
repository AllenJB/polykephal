SOURCES += main.cpp \
 PolykephalApp.cpp \
 client_thread.cpp \
 server.cpp
TEMPLATE = app
CONFIG += warn_on thread qt
TARGET = polykephal
DESTDIR = ../bin
QT -= gui
QT += network
HEADERS += PolykephalApp.h \
 client_thread.h \
 server.h
