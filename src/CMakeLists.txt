# mac {
# set(    TARGET Arora)
# set(    QMAKE_MACOSX_DEPLOYMENT_TARGET 10.4)
# }

find_package(Qt4 REQUIRED)

## TODO: optional
find_package(KDE4 REQUIRED)
find_package(KdeWebKit OPTIONAL)

include_directories(${QT_INCLUDES})

add_definitions(${QT_DEFINITIONS})
add_definitions( -DQT_NO_CAST_FROM_ASCII -DQT_STRICT_ITERATORS)

## TODO: optional
include_directories(${KDE4_INCLUDES})
add_definitions(${KDE4_DEFINITIONS})

if(${KDEWEBKIT_FOUND})
    include_directories(${KDEWEBKIT_INCLUDE_DIR})
    add_definitions(${KDEWEBKIT_DEFINITIONS} -DHAVE_KDEWEBKIT)
endif(${KDEWEBKIT_FOUND})

include_directories(${CMAKE_SOURCE_DIR}/src
                    ${CMAKE_SOURCE_DIR}/src/cookiejar
                    ${CMAKE_SOURCE_DIR}/src/cookiejar/networkcookiejar
                    ${CMAKE_SOURCE_DIR}/src/networkmonitor
                    ${CMAKE_SOURCE_DIR}/src/utils
                    ${CMAKE_CURRENT_BINARY_DIR})

set(arora_SRCS
    main.cpp
    aboutdialog.cpp
    acceptlanguagedialog.cpp
    bookmarks.cpp
    browserapplication.cpp
    browsermainwindow.cpp
    clearprivatedata.cpp
    downloadmanager.cpp
    edittableview.cpp
    edittreeview.cpp
    history.cpp
    locationbar.cpp
    languagemanager.cpp
    modelmenu.cpp
    networkaccessmanager.cpp
    plaintexteditsearch.cpp
    searchbar.cpp
    searchlineedit.cpp
    settings.cpp
    sourcehighlighter.cpp
    sourceviewer.cpp
    tabbar.cpp
    tabwidget.cpp
    toolbarsearch.cpp
    webactionmapper.cpp
    webview.cpp
    webviewsearch.cpp
    xbel.cpp)

set(arora_FORMS
    aboutdialog.ui
    addbookmarkdialog.ui
    acceptlanguagedialog.ui
    bookmarks.ui
    downloaditem.ui
    downloads.ui
    history.ui
    passworddialog.ui
    proxy.ui
    searchbanner.ui
    settings.ui)

## src.cmake
# CONFIG += qt warn_on
# contains(QT_BUILD_PARTS, tools): CONFIG += uitools
# else : DEFINES += QT_NO_UITOOLS

## TODO: necessary?
#include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# set(RCC_DIR .rcc)
# set(UI_DIR .ui)
# set(MOC_DIR .moc)
# set(OBJECTS_DIR .obj)

if(WIN32)
    add_definitions( -DGITVERSION=\"0\" -DGITCHANGENUMBER=\"0\")
else(WIN32)
#     exists($$PWD/../.git/HEAD) {
#         GITVERSION=$$system(git log -n1 --pretty=format:%h)
#         DEFINES += GITVERSION=\"\\\"$$GITVERSION\\\"\"
#         GITCHANGENUMBER=$$system(git log --pretty=format:%h | wc -l)
#         DEFINES += GITCHANGENUMBER=\"\\\"$$GITCHANGENUMBER\\\"\"
#     } else {
#         DEFINES += GITVERSION=\"\\\"0\\\"\"
#         DEFINES += GITCHANGENUMBER=\"\\\"0\\\"\"
#     }

    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../.git/HEAD)
        execute_process(
            COMMAND git log -n1 --pretty=format:%h
            OUTPUT_VARIABLE git_version
            ERROR_VARIABLE git_version
        )

        execute_process(
            COMMAND git log --pretty=format:%h
            COMMAND wc -l 
            OUTPUT_VARIABLE git_change_number
            ERROR_VARIABLE git_change_number
        )

        ## FIXME: wc -l returns a \n so we can't just add '"' to pass it as a QString
        ## maybe this should be done in browserapplication instead of here?
        ## FIXME: definitions set after find_package(KDE4) seems to behave weirdly and
        ## don't keep the '"'
        add_definitions( -DGITVERSION=\"0\" -DGITCHANGENUMBER=\"0\")
    else(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../.git/HEAD)
        add_definitions( -DGITVERSION=\"0\" -DGITCHANGENUMBER=\"0\")
    endif(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../.git/HEAD)
endif(WIN32)

include(networkmonitor/networkmonitor.cmake REQUIRED)
include(cookiejar/cookiejar.cmake REQUIRED)
include(utils/utils.cmake REQUIRED)

set(arora_RCS
    data/data.qrc
    htmls/htmls.qrc)

set(DISTFILES
    ${DISTFILES}
    ../AUTHORS
    ../ChangeLog
    ../LICENSE.GPL2
    ../LICENSE.GPL3
    ../README)

if(WIN32)
    set(RC_FILE browser.rc)
endif(WIN32)

if(APPLE)
    set(ICON browser.icns)
    set(QMAKE_INFO_PLIST Info_mac.plist)
endif(APPLE)

if(UNIX)
##     isEmpty(PREFIX) {
#        set(        PREFIX /usr/local)
##    }
#    set(    BINDIR ${PREFIX}/bin)
#    set(    DATADIR ${PREFIX}/share)
#    set(    PKGDATADIR ${DATADIR}/arora)

#    add_definitions( -DDATADIR ${DATADIR} -DPKGDATADIR ${PKGDATADIR})

     ## See git_version fixme
     #set(PKGDATADIR ${CMAKE_INSTALL_PREFIX}/share/arora)
     set(PKGDATADIR 0)
     add_definitions(-DPKGDATADIR="${PKGDATADIR}")
endif(UNIX)

include(../webkittrunk.cmake OPTIONAL)

if(WIN32)
    set(arora_SRCS ${arora_SRCS} explorerstyle.cpp)
    add_library(arora gdi32 advapi32)
#     LIBS += -lgdi32
#     LIBS += -ladvapi32
endif(WIN32)

## /src.cmake

#set(DESTDIR ../)

include(locale/locale.cmake REQUIRED)

# unix {
#     INSTALLS += target translations desktop iconxpm iconsvg icon16 icon32 icon128 man man-compress
# 
# set(    target.path $$BINDIR)
# 
# set(    translations.path $$PKGDATADIR)
#     translations.files += .qm/locale
# 
# set(    desktop.path $$DATADIR/applications)
#     desktop.files += arora.desktop
# 
# set(    iconxpm.path $$DATADIR/pixmaps)
#     iconxpm.files += data/arora.xpm
# 
# set(    iconsvg.path $$DATADIR/icons/hicolor/scalable/apps)
#     iconsvg.files += data/arora.svg
# 
# set(    icon16.path $$DATADIR/icons/hicolor/16x16/apps)
#     icon16.files += data/16x16/arora.png
# 
# set(    icon32.path $$DATADIR/icons/hicolor/32x32/apps)
#     icon32.files += data/32x32/arora.png
# 
# set(    icon128.path $$DATADIR/icons/hicolor/128x128/apps)
#     icon128.files += data/128x128/arora.png
# 
# set(    man.path $$DATADIR/man/man1)
#     man.files += data/arora.1
# 
# set(    man-compress.path $$DATADIR/man/man1)
# set(    man-compress.extra "" "gzip -9 -f \$(INSTALL_ROOT)/$$DATADIR/man/man1/arora.1" "")
# }

## TODO: split that, or find an automatic way to do it
set( arora_MOCS
../src/aboutdialog.h
../src/acceptlanguagedialog.h
../src/bookmarks.h
../src/browserapplication.h
../src/browsermainwindow.h
../src/clearprivatedata.h
../src/cookiejar/cookiedialog.h
../src/cookiejar/cookieexceptionsdialog.h
../src/cookiejar/cookieexceptionsmodel.h
../src/cookiejar/cookiejar.h
../src/cookiejar/cookiemodel.h
../src/cookiejar/networkcookiejar/networkcookiejar.h
../src/downloadmanager.h
../src/edittableview.h
../src/edittreeview.h
../src/history.h
../src/languagemanager.h
../src/locationbar.h
../src/locationbar_p.h
../src/modelmenu.h
../src/networkaccessmanager.h
../src/networkmonitor/networkmonitor.h
../src/plaintexteditsearch.h
../src/searchbar.h
../src/searchlineedit.h
../src/settings.h
../src/sourcehighlighter.h
../src/sourceviewer.h
../src/tabbar.h
../src/tabwidget.h
../src/toolbarsearch.h
../src/utils/autosaver.h
../src/utils/lineedit.h
../src/utils/lineedit_p.h
../src/utils/proxystyle.h
../src/utils/singleapplication.h
../src/utils/squeezelabel.h
../src/webactionmapper.h
../src/webview.h
../src/webviewsearch.h)

qt4_wrap_ui(UI ${arora_FORMS})
qt4_wrap_cpp(MOC ${arora_MOCS})
qt4_add_resources(RCS ${arora_RCS})

add_executable(arora ${arora_SRCS} ${UI} ${MOC} ${RCS})

target_link_libraries(arora ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTNETWORK_LIBRARY} ${QT_QTWEBKIT_LIBRARY} ${QT_QTUITOOLS_LIBRARY})

## TODO: optional
target_link_libraries(arora ${KDE4_KDECORE_LIBS} ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS})

if(KDEWEBKIT_FOUND)
    target_link_libraries(arora ${KDEWEBKIT_LIBRARIES})
endif(KDEWEBKIT_FOUND)

## FIXME: shouldn't be hardcoded
install(TARGETS arora RUNTIME DESTINATION /usr/bin) 
