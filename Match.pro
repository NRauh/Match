TEMPLATE = app

QT += qml quick widgets

CONFIG += c++11

SOURCES += src/main.cpp \
        src/accountmanager.cpp \
        src/jsoncpp.cpp \
        src/account.cpp
        ../src/sqlitecpp/Backup.cpp \
        ../src/sqlitecpp/Column.cpp \
        ../src/sqlitecpp/Database.cpp \
        ../src/sqlitecpp/Statement.cpp \
        ../src/sqlitecpp/Transaction.cpp \
        ../src/sqlitecpp/sqlite3.c

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
        ../src/sqlitecpp/Assertion.h \
        ../src/sqlitecpp/Backup.h \
        ../src/sqlitecpp/Column.h \
        ../src/sqlitecpp/Database.h \
        ../src/sqlitecpp/Exception.h \
        ../src/sqlitecpp/SQLiteCpp.h \
        ../src/sqlitecpp/Statement.h \
        ../src/sqlitecpp/Transaction.h \
        ../src/sqlitecpp/sqlite3.h
