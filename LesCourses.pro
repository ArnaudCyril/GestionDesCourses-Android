#-------------------------------------------------
#
# Project created by QtCreator 2013-10-18T13:47:31
#
#-------------------------------------------------

QT       += core gui
QT      +=sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LesCourses
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dialogmodifliste.cpp \
    dialogqtenewprod.cpp

HEADERS  += mainwindow.h \
    dialogmodifliste.h \
    dialogqtenewprod.h

FORMS    += mainwindow.ui \
    dialogmodifliste.ui \
    dialogqtenewprod.ui

CONFIG += mobility
MOBILITY = 

OTHER_FILES += \
    android/AndroidManifest.xml \
    android/res/layout/splash.xml \
    android/res/values-ru/strings.xml \
    android/res/values-fr/strings.xml \
    android/res/values-it/strings.xml \
    android/res/values-zh-rCN/strings.xml \
    android/res/values/strings.xml \
    android/res/values/libs.xml \
    android/res/values-pl/strings.xml \
    android/res/values-id/strings.xml \
    android/res/values-de/strings.xml \
    android/res/values-et/strings.xml \
    android/res/values-fa/strings.xml \
    android/res/values-ms/strings.xml \
    android/res/values-zh-rTW/strings.xml \
    android/res/values-pt-rBR/strings.xml \
    android/res/values-nb/strings.xml \
    android/res/values-el/strings.xml \
    android/res/values-rs/strings.xml \
    android/res/values-es/strings.xml \
    android/res/values-ro/strings.xml \
    android/res/values-nl/strings.xml \
    android/res/values-ja/strings.xml \
    android/version.xml \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/src/org/qtproject/qt5/android/bindings/QtActivity.java \
    android/src/org/qtproject/qt5/android/bindings/QtApplication.java

RESOURCES += \
    image.qrc

