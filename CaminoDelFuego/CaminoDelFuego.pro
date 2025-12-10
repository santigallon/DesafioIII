QT += widgets gui core multimedia


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    angel.cpp \
    camara2d.cpp \
    cargadortiled.cpp \
    cristiano.cpp \
    endemoniado.cpp \
    entidad.cpp \
    game.cpp \
    gamewidget.cpp \
    gestorbarrasestado.cpp \
    gestoreventos.cpp \
    gestorfisicas.cpp \
    gestorinteracciones.cpp \
    gestorsonido.cpp \
    habilidadwidget.cpp \
    housewindow.cpp \
    hudmanager.cpp \
    inputview.cpp \
    judio.cpp \
    level.cpp \
    levelmanager.cpp \
    main.cpp \
    mainwindow.cpp \
    mapatiles.cpp \
    nombremanager.cpp \
    npc.cpp \
    patternwindow.cpp \
    player.cpp \
    poseido.cpp \
    registro.cpp \
    romano.cpp \
    tiledloader.cpp \
    ventanacartaspulinas.cpp \
    ventanaexorcismo.cpp \
    ventanaoratoria.cpp \
    ventanasanacion.cpp

HEADERS += \
    angel.h \
    camara2d.h \
    cargadortiled.h \
    cristiano.h \
    endemoniado.h \
    entidad.h \
    game.h \
    gamewidget.h \
    gestorbarrasestado.h \
    gestoreventos.h \
    gestorfisicas.h \
    gestorinteracciones.h \
    gestorsonido.h \
    habilidadwidget.h \
    housewindow.h \
    hudmanager.h \
    inputview.h \
    judio.h \
    level.h \
    levelmanager.h \
    mainwindow.h \
    mapatiles.h \
    nombremanager.h \
    npc.h \
    patternwindow.h \
    player.h \
    poseido.h \
    registro.h \
    romano.h \
    tiledloader.h \
    ventanacartaspulinas.h \
    ventanaexorcismo.h \
    ventanaoratoria.h \
    ventanasanacion.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
