set(TARGET DEPENDPATH += .)
INCLUDEPATH += .

include(../autotests.cmake OPTIONAL)

# Input
SOURCES += tst_history.cpp
HEADERS +=
add_executable( _sources)
