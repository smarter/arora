set(TARGET DEPENDPATH += .)
INCLUDEPATH += .

include(../manualtests.cmake OPTIONAL)

SOURCES += main.cpp
HEADERS +=
add_executable( _sources)
