QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    attackpath.cpp \
    main.cpp \
    mainwindow.cpp \
    monster.cpp \
    plistreader.cpp \
    tower.cpp \
    towerposition.cpp \
    weapon.cpp

HEADERS += \
    attackpath.h \
    mainwindow.h \
    monster.h \
    plistreader.h \
    tower.h \
    towerposition.h \
    utility.h \
    weapon.h

FORMS += \
    mainwindow.ui

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=
