CONFIG += qt debug c++17 link_pkgconfig
QT += sql websockets

*-g++* {
    QMAKE_CXXFLAGS += "-Wall -Wextra -pedantic"
}


SOURCES += src/main.cpp \
    src/Daemon.cpp \
    src/Client.cpp

HEADERS += \
    src/Daemon.h \
    src/Client.h
