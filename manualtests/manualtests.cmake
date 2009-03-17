win32: CONFIG += console
mac:CONFIG -= app_bundle

include($$PWD/../src/src.cmake OPTIONAL)

set(RCC_DIR     $$PWD/.rcc)
set(UI_DIR      $$PWD/.ui)
set(MOC_DIR     $$PWD/.moc)
# can't share main.cpp objects of course
set(OBJECTS_DIR .obj)

