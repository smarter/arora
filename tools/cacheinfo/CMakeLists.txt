project(arora-cacheinfo)
DEPENDPATH += .
INCLUDEPATH += .

win32: CONFIG += console
mac:CONFIG -= app_bundle

QT += network

# Input
SOURCES += main.cpp

set(RCC_DIR     $$PWD/.rcc)
set(UI_DIR      $$PWD/.ui)
set(MOC_DIR     $$PWD/.moc)
set(OBJECTS_DIR $$PWD/.obj)
add_executable(arora-cacheinfo arora-cacheinfo_sources)
