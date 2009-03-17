set(TARGET DEPENDPATH += .)
INCLUDEPATH += . ../

include(../autotests.cmake OPTIONAL)

# Input
SOURCES += tst_edittreeview.cpp
HEADERS +=
add_executable( _sources)
