project(htmlToXBel)
DEPENDPATH += .
INCLUDEPATH += .

win32: CONFIG += console
mac:CONFIG -= app_bundle

QT += network webkit

# Input
SOURCES += main.cpp

set(RCC_DIR     $$PWD/.rcc)
set(UI_DIR      $$PWD/.ui)
set(MOC_DIR     $$PWD/.moc)
set(OBJECTS_DIR $$PWD/.obj)

RESOURCES += source.qrc

include(../../webkittrunk.cmake OPTIONAL)
add_executable(htmlToXBel htmlToXBel_sources)
