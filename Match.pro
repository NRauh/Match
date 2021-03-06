TEMPLATE = app

QT += qml quick widgets

CONFIG += c++11

LIBS += -ldl

SOURCES += src/main.cpp \
        src/accountmanager.cpp \
        src/account.cpp \
        src/sqlite/sqlite3.c \
        src/sqlite/sqlite.cpp \
        src/budget.cpp \
        src/helpers.cpp

RESOURCES += src/ui/qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
        src/accountmanager.h \
        src/account.h \
        src/sqlite/sqlite3.h \
        src/sqlite/sqlite.cpp \
        src/budget.h \
        src/helpers.h
