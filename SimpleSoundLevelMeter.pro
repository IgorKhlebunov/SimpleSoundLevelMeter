TEMPLATE = app

QT += qml quick widgets \
    multimedia

CONFIG += c++11

SOURCES += main.cpp \
    mainquickview.cpp \
    soundwrapper.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    mainquickview.h \
    soundwrapper.h
