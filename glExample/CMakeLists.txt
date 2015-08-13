
CMAKE_MINIMUM_REQUIRED( VERSION 2.4.6 )

## Setup platform specific helper defines build variants

IF(WIN32)
  
  ADD_DEFINITIONS (-D_USE_MATH_DEFINES)

ENDIF(WIN32)

set ( OPENGL_INCLUDE_DIR )
set ( GLUT_INCLUDE_DIR 
	"${PROJECT_SOURCE_DIR}/external/glut_fromMudboxCg"
	)
set (GLUT_LIBRARY_DIR 
	"${PROJECT_SOURCE_DIR}/external/glut_fromMudboxCg/ntx64/lib.x64"
	)
set (GLUT_LIBRARIES 
	"${GLUT_LIBRARY_DIR}/glut32.lib"
	)

set(GLEW_INCLUDE_DIR 
	"${PROJECT_SOURCE_DIR}/external/glew_fromLibigl/include"
	)
set(GLEW_SRC 
	"${PROJECT_SOURCE_DIR}/external/glew_fromLibigl/src/glew.c"
	)
set(GLEW_LIBRARY_DIR
	"${PROJECT_SOURCE_DIR}/external/glew_fromLibigl"
	)

include_directories( 
	${OPENGL_INCLUDE_DIR}
	${GLUT_INCLUDE_DIR}
	${GLEW_INCLUDE_DIR} 
)
link_directories(
	${GLUT_LIBRARY_DIR}
	${GLEW_LIBRARY_DIR}
)

add_executable(texExample mainTexExample.cpp ${GLEW_SRC}) 
target_link_libraries(texExample 
${GLUT_LIBRARIES} ${OPENGL_LIBRARIES}
)   

