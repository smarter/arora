set(TARGET DEPENDPATH += .)
INCLUDEPATH += . ../

include(../autotests.cmake OPTIONAL)

# Input
SOURCES += tst_tabbar.cpp
HEADERS +=
add_executable( _sources)
