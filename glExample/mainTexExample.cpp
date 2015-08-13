
/*

OpenGL texture example:    
Load image data from disk file to cpu memory; 
Create texture object, setting parameters;     
Load image data from cpu memory to gl memory; 
Sampling during rendering. 

*/ 


#include <cstdio>
#include <cmath>
#include <string.h> 
 
#include <gl/glew.h>
#include <GL/glut.h>

// shader source
const GLchar* vertexSource = 
	"#version 150\n"
	"in vec3 position;"
	"in vec3 color;"
	"in vec2 texcoord;"
	"out vec3 Color;"
	"out vec2 Texcoord;"
	"void main() {"
	"	Color = color;"
	"	Texcoord = texcoord;"
	"	gl_Position = vec4(position, 1.0);"
	"}";
const GLchar* fragmentSource = 
	"#version 150\n"
	"in vec3 Color;"
	"in vec2 Texcoord;"
	"out vec4 outColor;"
	"uniform sampler2D texSampler;"
	"void main() {"
	"	outColor = texture(texSampler, Texcoord);"
	"}";

GLuint g_iVBO; 
GLuint g_iEBO;
GLint posAttrib;
GLint colAttrib;
GLint texAttrib;
GLuint g_iShaderProgram; 

void DisplayCB(void)
{
	// Clear the screen with current background color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	
    glBindBuffer(GL_ARRAY_BUFFER, g_iVBO);
	
	// Specify the layout of the vertex data
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);

    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

    glEnableVertexAttribArray(texAttrib);
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	
	// draw a rectangle from the 2 triangles using 6 indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_iEBO); 
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(posAttrib);
	glDisableVertexAttribArray(colAttrib);
	glDisableVertexAttribArray(texAttrib); 
	
	// Present frame buffer
	glutSwapBuffers();
}

static void AddShader(GLuint iShaderProgram, const char *pShaderText, GLenum iShaderType)
{
	GLuint iShaderObj = glCreateShader(iShaderType);

	if (iShaderObj == 0) {
		fprintf( stderr, "Error creating shader type %d.\n", iShaderType);
		exit(0);
	}

	const GLchar *p[1];
	p[0] = pShaderText;
	GLint aLengths[1];
	aLengths[0] = strlen(pShaderText);
	glShaderSource(iShaderObj, 1, p, aLengths);//glShaderSource(iShaderObj, 1, &pShaderText, NULL);  
	glCompileShader(iShaderObj); 
	GLint iSuccess; 
	glGetShaderiv(iShaderObj, GL_COMPILE_STATUS, &iSuccess); 
	if (!iSuccess) {
		GLchar cInfoLog[1024];
		glGetShaderInfoLog(iShaderObj, 1024, NULL, cInfoLog); 
		fprintf( stderr, "Error compiling shader type %d: %s.\n", iShaderType, cInfoLog ); 
		exit(1); 
	}

	glAttachShader(iShaderProgram, iShaderObj); 
}
static void CompileShaders()
{
	// create and link the vertex and fragment shader into a shader program
    GLuint shaderProgram = glCreateProgram();
	if (shaderProgram == 0) {
		fprintf( stderr, "Error creating shader program.\n");
		exit(1);
	} 
	
	AddShader(shaderProgram, vertexSource, GL_VERTEX_SHADER); 
	AddShader(shaderProgram, fragmentSource, GL_FRAGMENT_SHADER); 

    glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    // Specify the layout of the vertex data
    posAttrib = glGetAttribLocation(shaderProgram, "position");
    colAttrib = glGetAttribLocation(shaderProgram, "color");
    texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
    glUniform1i(glGetUniformLocation(shaderProgram, "texSample"), 0);

	g_iShaderProgram = shaderProgram;
}

int main(int argc,char * argv[])
{
	// Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(500, 400);
	// Center window
	glutInitWindowPosition(
		glutGet(GLUT_SCREEN_WIDTH)/2-glutGet(GLUT_INIT_WINDOW_WIDTH)/2,
		glutGet(GLUT_SCREEN_HEIGHT)/2-glutGet(GLUT_INIT_WINDOW_HEIGHT)/2);
	glutCreateWindow("Texture Example");
	glutCreateMenu(NULL);
		
    // Set GLUT callbacks
	glutDisplayFunc(DisplayCB);
	//glutCloseFunc(closeEvent);
    //glutReshapeFunc(resizeEvent);
    //glutMotionFunc(mouseEvent);
    //glutMouseFunc(mouseButtonEvent);
    //glutPassiveMotionFunc(mousePassiveEvent);
    //glutKeyboardFunc(keyboardEvent);
    //glutSpecialFunc(keySpecEvent);
	
	// Must be done after glut is initialized!
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf( stderr, "Error: ''.\n", glewGetErrorString(res));
		return 1;
	} 

	printf("GL version: %s.\n", glGetString(GL_VERSION) );

	glClearColor(0.0f, .3f, 0.0f, 1.0f); 

	// create vertex array object
	GLuint iVAO;
	glGenVertexArrays(1, &iVAO);
	glBindVertexArray(iVAO);

	// create a vertex buffer object and copy the vertex data to it
	GLfloat vertices[] = {
		// pos, color, texcoord
		-0.5, -0.5, 0,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f, // top-left
		 0.5, -0.5, 0,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f, // top-left
		 0.5,  0.5, 0,	1.0f, 0.0f, 1.0f,	1.0f, 1.0f, // top-left
		-0.5,  0.5, 0,	1.0f, 1.0f, 1.0f,	0.0f, 1.0f, // top-left	
	}; 
	glGenBuffers(1, &g_iVBO);
	glBindBuffer(GL_ARRAY_BUFFER, g_iVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int indices[] = { 0, 1, 2, 
		0, 2, 3 };
	glGenBuffers(1, &g_iEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_iEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 

	CompileShaders(); 

	// Load textures
    GLuint texId;
    glGenTextures(1, &texId);

    glActiveTexture(GL_TEXTURE0); // for one tex this can be ignore, it is designed for multiply texture.
    glBindTexture(GL_TEXTURE_2D, texId);
    
	// Black/white checkerboard
	float pixels[] = {
		0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f
	};
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_FLOAT, pixels);
	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);GL_LINEAR;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);GL_LINEAR;
	
	// Call the GLUT main loop
	glutMainLoop();

	glDeleteTextures(1, &texId);
    glDeleteProgram(g_iShaderProgram);
    glDeleteBuffers(1, &g_iVBO);
    glDeleteVertexArrays(1, &iVAO);
	return 0;
}
