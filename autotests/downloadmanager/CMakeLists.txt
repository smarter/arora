set(TARGET DEPENDPATH += .)
INCLUDEPATH += .

include(../autotests.cmake OPTIONAL)

# Input
SOURCES += tst_downloadmanager.cpp
HEADERS +=
add_executable( _sources)
