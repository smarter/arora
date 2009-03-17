set(TARGET DEPENDPATH += .)
INCLUDEPATH += .

include(../autotests.cmake OPTIONAL)

# Input
SOURCES += tst_webactionmapper.cpp
HEADERS +=
add_executable( _sources)
