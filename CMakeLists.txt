project(arora)

cmake_minimum_required(VERSION 2.6.0 FATAL_ERROR)

set(QT_MIN_VERSION "4.4.0")

find_package(Qt4 REQUIRED)

add_subdirectory(src)
#add_subdirectory(tools)

# unix {
#     # this is an ugly work around to do .PHONY: doc
# set(    doxygen.target doc dox)
# set(    doxygen.commands doxygen Doxyfile)
# set(    doxygen.depends Doxyfile)
#     QMAKE_EXTRA_UNIX_TARGETS += doxygen
# }
