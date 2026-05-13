QT += widgets multimedia

CONFIG += c++17


QMAKE_CXXFLAGS += -Wno-error=implicit-function-declaration
QMAKE_CFLAGS += -Wno-error=implicit-function-declaration


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Level1.cpp \
    Level2.cpp \
    Level2enemy.cpp \
    artifact.cpp \
    character.cpp \
    enemy.cpp \
    game.cpp \
    gameObject.cpp \
    gameWindow.cpp \
    hint.cpp \
    inventory.cpp \
    item.cpp \
    level.cpp \
    level1enemy.cpp \
    main.cpp \
    obstacle.cpp \
    player.cpp \
    tools.cpp

HEADERS += \
    Level1.h \
    Level2.h \
    Level2enemy.h \
    artifact.h \
    character.h \
    enemy.h \
    game.h \
    gameObject.h \
    gameWindow.h \
    hint.h \
    inventory.h \
    item.h \
    level.h \
    level1enemy.h \
    obstacle.h \
    player.h \
    tools.h

FORMS += \
    gameWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
<<<<<<< Updated upstream
=======
CONFIG += resources_big

DISTFILES += \
    images/Level2.jpeg \
    images/hieroglyph_chart-2.png \
    images/rock_A-3.png \
    images/rock_M.png \
    images/rock_N.png
>>>>>>> Stashed changes

