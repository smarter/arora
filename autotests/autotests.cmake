win32: CONFIG += console
mac:CONFIG -= app_bundle

CONFIG += qtestlib

include($$PWD/../src/src.cmake OPTIONAL)
include($$PWD/modeltest/modeltest.cmake OPTIONAL)

HEADERS += qtest_arora.h

DEFINES += AUTOTESTS

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

set(RCC_DIR     $$PWD/.rcc)
set(UI_DIR      $$PWD/.ui)
set(MOC_DIR     $$PWD/.moc)
set(OBJECTS_DIR $$PWD/.obj)


