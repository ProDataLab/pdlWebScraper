#-------------------------------------------------
#
# Project created by QtCreator 2013-12-22T12:45:25
#
#-------------------------------------------------

<<<<<<< HEAD
QT       += core
=======
QT       += core network
>>>>>>> download-cache

QT       -= gui

TARGET = pdlWebScraper
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

<<<<<<< HEAD

SOURCES += main.cpp
=======
SOURCES += main.cpp \
    downloadcache.cpp

HEADERS += \
    downloadcache.h
>>>>>>> download-cache
