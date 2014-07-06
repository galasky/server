#QT       += sql # On rajoute SQL
QT       -= gui # On enlève la GUI
CONFIG   += console # On rajoute la console
TEMPLATE = app # On veut une application, oui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = server
OBJECTS_DIR  = ./obj

QMAKE_CXXFLAGS +=

INCLUDEPATH += include

SOURCES +=      src/main.cpp            \
                src/Server.cpp          \
                src/Client.cpp          \
                src/DataBase.cpp        \
                src/User.cpp            \
                src/Parser.cpp          \
                src/Game.cpp            \
                src/md5.cpp
           
HEADERS +=      include/Server.hh       \
                include/Client.hh       \
                include/DataBase.hh     \
                include/User.hh         \
                include/md5.h          \
                include/Game.hh         \
                include/Parser.hh


FORMS +=
