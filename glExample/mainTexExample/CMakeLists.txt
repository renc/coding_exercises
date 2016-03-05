
## Setup platform specific helper defines build variants

IF(WIN32)
  
  ADD_DEFINITIONS (-D_USE_MATH_DEFINES)

ENDIF(WIN32)

set ( OPENGL_INCLUDE_DIR )
set( OPENGL_LIBRARIES OpenGL32.lib)

set ( GLUT_INCLUDE_DIR 
	"${PROJECT_SOURCE_DIR}/external/glut_fromMudboxCg"
	)
set (GLUT_LIBRARY_DIR 
	"${PROJECT_SOURCE_DIR}/external/glut_fromMudboxCg/ntx64/lib.x64"
	)
set (GLUT_LIBRARIES 
	"${GLUT_LIBRARY_DIR}/glut32.lib"
	)

#set(GLEW_INCLUDE_DIR 
#	"${PROJECT_SOURCE_DIR}/external/glew_fromLibigl/include"
#	)
#set(GLEW_SRC 
#	"${PROJECT_SOURCE_DIR}/external/glew_fromLibigl/src/glew.c"
#	)
#set(GLEW_LIBRARY_DIR
#	"${PROJECT_SOURCE_DIR}/external/glew_fromLibigl"
#	)
set( GLEW_INCLUDE_DIR_1_12_0
	"D:/extern/glew-1.12.0/include")
set( GLEW_SRC_1_12_0
	"D:/extern/glew-1.12.0/src/glew.c")
set( GLEW_LIBRARY_DIR_1_12_0
	"D:/extern/glew-1.12.0/lib/Debug/x64")
set( GLEW_LIBRARIES glew32d.lib)

include_directories( 
	${OPENGL_INCLUDE_DIR}
	${GLUT_INCLUDE_DIR}
	${GLEW_INCLUDE_DIR_1_12_0} 
)
link_directories(
	${GLUT_LIBRARY_DIR}
	${GLEW_LIBRARY_DIR_1_12_0}
)

add_executable(texExample mainTexExample.cpp ) 
target_link_libraries(texExample 
${GLUT_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES}
)   

