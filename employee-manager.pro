CONFIG += qt debug c++17 link_pkgconfig
QT += core network sql websockets

*-g++* {
    QMAKE_CXXFLAGS += "-Wall -Wextra -pedantic"
}


SOURCES += src/main.cpp \
    src/Client.cpp \
    src/Daemon.cpp \
    src/Database.cpp \
    src/Employee.cpp \
    src/MailClient.cpp \
    src/Request.cpp \
    src/RequestFactory.cpp \
    src/Smtp.cpp

HEADERS += \
    src/Client.h \
    src/Const.h \
    src/Daemon.h \
    src/Database.h \
    src/Employee.h \
    src/MailClient.h \
    src/Request.h \
    src/RequestFactory.h \
    src/Singleton.h \
    src/Smtp.h
