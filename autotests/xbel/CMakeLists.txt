set(TARGET DEPENDPATH += .)
INCLUDEPATH += .

include(../autotests.cmake OPTIONAL)

# Input
set(SOURCES tst_xbel.cpp xbel.cpp)
set(HEADERS xbel.h)
add_executable( _sources)
