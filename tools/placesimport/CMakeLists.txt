project(arora-placesimport)
DEPENDPATH += .
INCLUDEPATH += .

win32: CONFIG += console
mac:CONFIG -= app_bundle

QT += sql

# Input
SOURCES += main.cpp

# This can be improved to just include the history manager and utility classes
# once the history classes are separated out into individual files.
include(../../src/src.cmake OPTIONAL)

set(RCC_DIR     $$PWD/.rcc)
set(UI_DIR      $$PWD/.ui)
set(MOC_DIR     $$PWD/.moc)
set(OBJECTS_DIR $$PWD/.obj)
add_executable(arora-placesimport arora-placesimport_sources)
