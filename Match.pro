TEMPLATE = app

QT += qml quick widgets

CONFIG += c++11

SOURCES += src/main.cpp \
        src/accountmanager.cpp \
        src/jsoncpp.cpp \
        src/account.cpp
        ../src/sqlite/sqlite3.c \
        ../src/sqlite/sqlite.cpp

RESOURCES += src/ui/qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
        src/accountmanager.h \
        src/json/json.h \
        src/json/json-forwards.h \
        src/account.h
        ../src/sqlite/sqlite3.h \
        ../src/sqlite/sqlite.cpp
