set(TARGET DEPENDPATH += .)
INCLUDEPATH += .

include(../autotests.cmake OPTIONAL)

# Input
set(SOURCES tst_lineedit.cpp lineedit.cpp)
set(HEADERS lineedit.h lineedit_p.h proxystyle.h)
add_executable( _sources)
