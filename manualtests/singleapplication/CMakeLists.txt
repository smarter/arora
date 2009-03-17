set(TARGET DEPENDPATH += .)
INCLUDEPATH += .

include(../manualtests.cmake OPTIONAL)

# Input
set(SOURCES main.cpp singleapplication.cpp)
set(HEADERS singleapplication.h)add_executable( _sources)
