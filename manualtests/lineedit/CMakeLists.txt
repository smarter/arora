set(TARGET DEPENDPATH += .)
INCLUDEPATH += .

include(../manualtests.cmake OPTIONAL)

# Input
set(FORMS   dialog.ui)
set(SOURCES main.cpp lineedit.cpp)
set(HEADERS lineedit.h lineedit_p.h proxystyle.h)
add_executable( _sources)
