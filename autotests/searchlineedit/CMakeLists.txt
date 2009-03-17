set(TARGET DEPENDPATH += .)
INCLUDEPATH += .

include(../autotests.cmake OPTIONAL)

# Input
SOURCES += tst_searchlineedit.cpp
add_executable( _sources)
