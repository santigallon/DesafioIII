QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    angel.cpp \
    cristiano.cpp \
    endemoniado.cpp \
    entidad.cpp \
    judio.cpp \
    main.cpp \
    mainwindow.cpp \
    npc.cpp \
    player.cpp \
    poseido.cpp \
    romano.cpp

HEADERS += \
    angel.h \
    cristiano.h \
    endemoniado.h \
    entidad.h \
    judio.h \
    mainwindow.h \
    npc.h \
    player.h \
    poseido.h \
    romano.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
