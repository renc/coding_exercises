

#include <GL/glew.h> // always include it before gl.h and glfw
#include <GLFW/glfw3.h>
#include <stdio.h>

unsigned int g_iWindowWidth = 648;
unsigned int g_iWindowHeight = 480;

void glfw_error_callback(int error, const char* description)
{

}

void glfw_window_size_callback(GLFWwindow *window, int w, int h)
{
	// after window size is change,
	// update the global value.
	g_iWindowWidth = w;
	g_iWindowHeight = h; 
}

void update_fps_counter(GLFWwindow *window)
{
	static double previous_seconds = glfwGetTime();
	static unsigned int frame_count = 0;

	++frame_count;

	double current_seconds = glfwGetTime();
	if (current_seconds - previous_seconds > 0.25)
	{
		double fps = (double)frame_count / (current_seconds - previous_seconds);

		char tmp[128];
		sprintf(tmp, " fps: %.2f ", fps);
		
		glfwSetWindowTitle(window, tmp);
		previous_seconds = current_seconds;
		frame_count = 0;
	}
}

float *build_triangle()
{
	// 3 points, every point is vec3f
	  
	float *points = new float[3*3];
	points[0] = -0.5f;
	points[1] = -0.5f;
	points[2] = 0;
	points[3] = 0.5f;
	points[4] = -0.5f;
	points[5] = 0;
	points[6] = 0;
	points[7] = 0.5f;
	points[8] = 0; 

	return points;
}

int main()
{

	glfwSetErrorCallback(glfw_error_callback);
	// start gl context and os windows
	if (!glfwInit())
	{
		fprintf(stderr, "Error: could not init glfw.\n");
		return 0;
	}
	
	// renc: if these lines are enable, it change from 4.4 to 3.2
	// renc: so i just disable these lines.
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(
		g_iWindowWidth, g_iWindowHeight, "Hello Triangle", NULL, NULL);
	if (!window)
	{
		fprintf(stderr, "Error: could not open window with glfw.\n");
		return 0;
	}
	glfwMakeContextCurrent(window);
	glfwSetWindowSizeCallback(window, glfw_window_size_callback);

	//
	glewExperimental = GL_TRUE;
	glewInit();

	const GLubyte *renderer = glGetString(GL_RENDERER);
	const GLubyte *version = glGetString(GL_VERSION);
	printf("Renderer: %s.\n", renderer);
	printf("OpenGL version supproted %s.\n", version);

	// tell gl to only draw onto a pixel
	// if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as closer

	// geometry data
	unsigned int iPointCount = 3; 
	unsigned int iVtxDim = 3; // vec3f
	float *points = NULL;
	points = build_triangle();

	//unsigned int iPointDataSize = sizeof(points); iPointDataSize;
	//if (iPointDataSize != sizeof(float) * 9) 
	//{ 
	//	fprintf(stderr, "Error: point data size.\n");  
	//	return 1; 
	//};

	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*iPointCount*iVtxDim, points,
		GL_STATIC_DRAW);

	// Most meshes will use a collection of one or more vertex buffer
	// objects to hold vertex points, tex-coordinates, vertex normals,
	// ectc. In older GL implementions we would have to bind each one,
	// and define their memory layout, EVERY TIME THAT WE DRAW THE MESH.
	// To simplify that, we have new thing called the vertex attribute
	// object (VAO), which remembers all of the vertex buffers that you
	// want to use, and memory layer of each one. We set up the vertex 
	// array object once per mesh. When we want to draw, all we do then
	// is bind the VAO and draw.
	// 
	// 
	unsigned int iPosAttrLoc = 0;
	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(iPosAttrLoc);// attr location 0
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // vbo is bind to attr loc 0.
	glVertexAttribPointer(iPosAttrLoc, iVtxDim, GL_FLOAT, GL_FALSE, 0, NULL);
	// bindBuffer must before vtxAttrPointer which describe the layout 
	// of each buffer(vbo) to the vao.
	// renc: when we define vbo, we use bufferData to transfer data,
	// renc: here we need to tell data layout to vao.


	///
	const char* vertex_shader =
		"#version 400\n"
		"in vec3 vp;"
		"void main() {"
		" gl_Position = vec4(vp, 1.0);"
		"}";
	const char* fragment_shader =
		"#version 400\n"
		"out vec4 frag_colour;"
		"void main() {"
		" frag_colour = vec4(0.5, 0.0, 0.5, 1.0);"
		"}";
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);

	GLuint shader_programme = glCreateProgram();
	glAttachShader(shader_programme, fs);
	glAttachShader(shader_programme, vs);
	glBindAttribLocation(shader_programme, iPosAttrLoc, "vp");
	glLinkProgram(shader_programme);

	// to draw everything with lines (wire-frame)
	//glPolygonMode(GL_FRONT, GL_LINE);

	// mainloop
	while (!glfwWindowShouldClose(window))
	{
		update_fps_counter(window);

		// wipe the drawing surface clear
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// if window is resized, the gl part does not scale to fit,
		// to fix that we need this glViewport.
		// renc: this seems map the whole window to [-1,1].
		glViewport(0, 0, g_iWindowWidth, g_iWindowHeight);

		// try to disale this use program, check the scene.
		glUseProgram(shader_programme);
		
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// put the stuff we've been drawing onto the display
		glfwSwapBuffers(window);

		// update other events like input handling,
		// update non-graphical events like key-presses.
		glfwPollEvents();
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, 1);		
	}

	glfwTerminate();
	return 1;
}