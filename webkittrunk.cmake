#
# export QT_WEBKIT=webkit_trunk
# export WEBKITDIR=$HOME/dev/webkit
# export WEBKITBRANCH=some_cool_git_branch_of_webkit
#
## PORTME
# # CONFIG += $ENV{QT_WEBKIT}
# # webkit_trunk {
# # set(    WEBKITDIR $ENV{WEBKITDIR)}
# # set(    WEBKITBRANCH $ENV{WEBKITBRANCH)}
# #     isEmpty(WEBKITBRANCH) {
# # set(        CONFIG(release):WEBKITBUILD $$WEBKITDIR/WebKitBuild/Release/lib)
# # set(        CONFIG(debug):WEBKITBUILD $$WEBKITDIR/WebKitBuild/Debug/lib)
# #     } else {
# # set(        CONFIG(release):WEBKITBUILD $$WEBKITDIR/WebKitBuild/$$WEBKITBRANCH/Release/lib)
# # set(        CONFIG(debug):WEBKITBUILD $$WEBKITDIR/WebKitBuild/$$WEBKITBRANCH/Debug/lib)
# #     }
# #     message(Using WebKit Trunk at $$WEBKITDIR)
# #     message(Using WebKit Build at $$WEBKITBUILD)
# #     QT -= webkit
# #     DEFINES += WEBKIT_TRUNK
# # set(    QMAKE_LIBDIR_FLAGS -L$$WEBKITBUILD)
# # set(    LIBS -L$$WEBKITBUILD -lQtWebKit)
# # set(    INCLUDEPATH $$WEBKITDIR/WebKit/qt/Api $$INCLUDEPATH)
# # set(    QMAKE_RPATHDIR $$WEBKITBUILD $$QMAKE_RPATHDIR)
# # }

