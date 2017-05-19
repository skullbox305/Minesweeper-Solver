# -------------------------------------------------
# Project created by QtCreator 2010-10-28T13:39:17
# -------------------------------------------------
QT += sql
QT += widgets
TARGET = Minesweeper
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    minesweeper.cpp \
    minesweeperbutton.cpp \
    solver.cpp \
    performanceanalysis.cpp \
    helpwindow.cpp \
    aboutwindow.cpp
HEADERS += mainwindow.h \
    minesweeper.h \
    minesweeperbutton.h \
    solver.h \
    performanceanalysis.h \
    helpwindow.h \
    aboutwindow.h
FORMS += mainwindow.ui \
    performanceanalysis.ui \
    helpwindow.ui \
    aboutwindow.ui
RESOURCES += \
    images.qrc
