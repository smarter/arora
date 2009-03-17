set(TARGET DEPENDPATH += .)
INCLUDEPATH += .

include(../manualtests.cmake OPTIONAL)

# Input
set(SOURCES main.cpp squeezelabel.cpp)
set(HEADERS squeezelabel.h)
add_executable( _sources)
