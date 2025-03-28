QT       += core gui opengl openglwidgets widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
QMAKE_CXX = g++
QMAKE_CC = gcc
QMAKE_CXXFLAGS += -Wno-sign-compare -Wno-nonnull

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    myopenglwidget.cpp \
    ../logic/model.cpp \
    ../logic/transformation.cpp \

HEADERS += \
    mainwindow.h \
    ../logic/model.hpp \
    ../controller/controller.hpp \
    ../controller/strategies.hpp \
    myopenglwidget.h

FORMS += \
    mainwindow.ui

INCLUDEPATH += \
    $$PWD/../logic/

include(QGifImage/gifimage/qtgifimage.pri)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target