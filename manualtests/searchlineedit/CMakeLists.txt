set(TARGET DEPENDPATH += .)
INCLUDEPATH += .

include(../manualtests.cmake OPTIONAL)

# Input
SOURCES += main.cpp
HEADERS +=
add_executable( _sources)
