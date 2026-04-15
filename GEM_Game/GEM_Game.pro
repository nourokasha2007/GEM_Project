QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    artifact.cpp \
    character.cpp \
    game.cpp \
    gameObject.cpp \
    gameWindow.cpp \
    hint.cpp \
    item.cpp \
    level1Enemy.cpp \
    level2Enemy.cpp \
    level3Enemy.cpp \
    main.cpp \
    obstacle.cpp \
    player.cpp \
    room.cpp

HEADERS += \
    artifact.h \
    character.h \
    game.h \
    gameObject.h \
    gameWindow.h \
    hint.h \
    item.h \
    level1Enemy.h \
    level2Enemy.h \
    level3Enemy.h \
    obstacle.h \
    player.h \
    room.h

FORMS += \
    gameWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    media_resources.qrc
