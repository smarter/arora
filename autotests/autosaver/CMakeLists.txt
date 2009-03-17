set(TARGET DEPENDPATH += .)
INCLUDEPATH += .

include(../autotests.cmake OPTIONAL)

# Input
set(SOURCES tst_autosaver.cpp autosaver.cpp)
set(HEADERS autosaver.h)
add_executable( _sources)
