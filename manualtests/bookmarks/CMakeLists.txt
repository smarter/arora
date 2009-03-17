set(TARGET DEPENDPATH += .)
INCLUDEPATH += .

include(../manualtests.cmake OPTIONAL)
include(../../autotests/modeltest/modeltest.cmake OPTIONAL)

# Input
SOURCES += main.cpp
HEADERS +=
add_executable( _sources)
