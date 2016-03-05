## Setup platform specific helper defines build variants

IF(WIN32)
  
  ADD_DEFINITIONS (-D_USE_MATH_DEFINES)

ENDIF(WIN32)

set ( OPENGL_INCLUDE_DIR )
set( OPENGL_LIBRARIES OpenGL32.lib)

set( GLEW_INCLUDE_DIR_1_12_0
	"D:/extern/glew-1.12.0/include")
set( GLEW_SRC_1_12_0
	"D:/extern/glew-1.12.0/src/glew.c")
set( GLEW_LIBRARY_DIR_1_12_0
	"D:/extern/glew-1.12.0/lib/Debug/x64")
set( GLEW_LIBRARIES_1_12_0 glew32d.lib)

set( GLFW_INCLUDE_DIR "D:/extern/glfw-3.1.2/include")
set( GLFW_LIBRARY_DIR "D:/extern/glfw-3.1.2/lib")
set( GLFW_LIBRARIES	glfw3.lib )


include_directories( 
	${OPENGL_INCLUDE_DIR}
	${GLEW_INCLUDE_DIR_1_12_0}
	${GLFW_INCLUDE_DIR}
)
link_directories(
	${GLEW_LIBRARY_DIR_1_12_0}
	${GLFW_LIBRARY_DIR}
)

add_executable(gridCube mainGridCube.cpp ) 
target_link_libraries(gridCube 
${GLFW_LIBRARIES} ${GLEW_LIBRARIES_1_12_0} ${OPENGL_LIBRARIES}
)   

