cmake_minimum_required(VERSION 2.8.11)

project(testFortuneclient)

set (APPNAME testFortuneclient) 

SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/Debug" )
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/Release")


# Find includes in corresponding build directories
# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)

# Find the QtCore library 
find_package( Qt5Core)
find_package( Qt5Widgets) 
find_package( Qt5Network) 

# Tell CMake to create the executable
add_executable(${APPNAME} 
	main.cpp  
	client.h  client.cpp
	)

# Use the Widgets module from Qt 5.
target_link_libraries(${APPNAME} 
	Qt5::Core
	Qt5::Widgets  
	Qt5::Network  
	)

