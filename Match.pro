TEMPLATE = app

QT += qml quick widgets

SOURCES += src/main.cpp \
    src/accountmanager.cpp

RESOURCES += src/ui/qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    src/accountmanager.h
