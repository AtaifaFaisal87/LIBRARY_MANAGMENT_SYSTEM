QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET   = LibraryGUI
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    loginwindow.cpp \
    registerdialog.cpp \
    userdashboard.cpp \
    librariandashboard.cpp \
    library_core.cpp

HEADERS += \
    mainwindow.h \
    loginwindow.h \
    registerdialog.h \
    userdashboard.h \
    librariandashboard.h \
    library_core.h \
    styles.h
