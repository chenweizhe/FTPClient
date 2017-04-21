#-------------------------------------------------
#
# Project created by QtCreator 2017-04-10T21:24:57
# 相当于安卓的清单文件AndroidManifest.xml
#-------------------------------------------------
#如果使用其他的模块的话可以添加其他模块。常用的有QtNetwork,QtOpenGl, QtSql, QtXml. QtWebkit等等，如果应用程序使用到该模块，需要添加相应的模块。
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FTPClient
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


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
