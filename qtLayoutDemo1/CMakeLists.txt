
cmake_minimum_required(VERSION 2.8.11)

project(qtLayoutDemo1)

# Without this setting, find qt will fails, check the Qt 5.6/CMake Manual for details. 
# This other way, without using this, may be setting Qt path at the PATH environment variable.
set (CMAKE_PREFIX_PATH "D:\\Qt\\Qt5.6.0\\5.6\\msvc2015_64\\")

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(QtCore) 
find_package(Qt5Widgets)

# Tell CMake to create the helloworld executable
add_executable(qtLayoutDemo1 WIN32 main.cpp widget.cpp)

# Use the Widgets module from Qt 5.
target_link_libraries(qtLayoutDemo1 Qt5::Core Qt5::Widgets)


# 2016.4.16 when trying to build the project with visual studio 2012 (x64), i get this error:
# 
#2>  Generating Code...
#2>qtmaind.lib(qtmain_win.obj) : error LNK2038: mismatch detected for '_MSC_VER': value '1900' doesn't match value '1700' in main.obj
#
# The reason for that is i'm using qt 5.6.0 for windows 64-bit (VS 2015). 
# to fix that, i need to use visual studio 2015 to build the project.

# References:
# Qt CMake Manual 
#