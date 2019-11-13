CONFIG += qt debug c++17 link_pkgconfig
QT += sql websockets

*-g++* {
    QMAKE_CXXFLAGS += "-Wall -Wextra -pedantic"
}


SOURCES += src/main.cpp \
    src/Client.cpp \
    src/Daemon.cpp \
    src/Database.cpp \
    src/Employee.cpp \
    src/Request.cpp \
    src/RequestFactory.cpp

HEADERS += \
    src/Client.h \
    src/Const.h \
    src/Daemon.h \
    src/Database.h \
    src/Employee.h \
    src/Request.h \
    src/RequestFactory.h \
    src/Singleton.h
