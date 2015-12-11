#-------------------------------------------------
#
# Project created by QtCreator 2015-09-27T18:52:11
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = epollClient
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    TcpSocket.cpp \
    SocketPacket.cpp \
    LockAuto.cpp \
    Lock.cpp \
    Timeval.cpp \
    SocketBuffer.cpp

HEADERS += \
    global.h \
    TcpSocket.h \
    SocketPacket.h \
    LockAuto.h \
    Lock.h \
    Timeval.h \
    SocketBuffer.h
