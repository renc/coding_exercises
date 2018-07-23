cmake_minimum_required(VERSION 2.8.11)

# this project name can be used as a standstore for ${QtExamples_SOURCE_DIR}
project(QtExamples)

#set (SQLite3Header_Dir ${CMAKE_SOURCE_DIR}/ExternalLibraries/sqlite-amalgamation-3160200)

Include_directories(${CMAKE_SOURCE_DIR}  
	${SQLite3Header_Dir}
	)

# want to do these setting here rather in cmake-gui
set (Qt5Core_DIR 	D:/Qt/Qt5.7.0/5.7/msvc2015_64/lib/cmake/Qt5Core 	CACHE STRING "")
set (Qt5Widgets_DIR D:/Qt/Qt5.7.0/5.7/msvc2015_64/lib/cmake/Qt5Widgets 	CACHE STRING "")
set (Qt5Gui_DIR 	D:/Qt/Qt5.7.0/5.7/msvc2015_64/lib/cmake/Qt5Gui 		CACHE STRING "") 	
set (Qt5Charts_DIR 	D:/Qt/Qt5.7.0/5.7/msvc2015_64/lib/cmake/Qt5Charts 	CACHE STRING "") 
set (Qt5Network_DIR 	D:/Qt/Qt5.7.0/5.7/msvc2015_64/lib/cmake/Qt5Network 	CACHE STRING "") 
set (Qt5Sql_DIR 	D:/Qt/Qt5.7.0/5.7/msvc2015_64/lib/cmake/Qt5Sql 	CACHE STRING "") 

	 
# ---------------------
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
 
# ---------------------

#add_subdirectory(ExternalLibraries/sqlite-amalgamation-3160200)
add_subdirectory(testQBarSeries) 
add_subdirectory(testFortuneserver)
add_subdirectory(testFortuneclient)
add_subdirectory(sqlTableModel)
add_subdirectory(thread_detach_crash)
