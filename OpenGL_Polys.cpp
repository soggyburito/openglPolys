#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <chrono>

const char* vertexSource = R"glsl(#version 150 core
	in vec2 position;
	in float color;

	out float Color;
	void main(){
		Color=color;
		gl_Position = vec4(-position, 0.0, 1.0);
	})glsl";

const char* fragmentSource = R"glsl(
		#version 150 core
		in float  Color;
		out vec4 outColor;
		void main(){
			outColor = vec4(Color, Color,Color,1.0);
		})glsl";

const char* colorUniform = R"glsl(
		#version 150 core
		uniform vec3 triangleColor;
		out vec4 outColor;
		void main(){
			outColor = vec4(Color,1.0);
		})glsl";

int main(){
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Polys", nullptr, nullptr);

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();

	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);

	printf("%u\n", vertexBuffer);

	/*float vertices[] = {
		0.0f, 0.5f, //vertex1(0.0,0.5)(x,y) \
		0.5f, -0.5f,//vertex2(0.5,-0.5)(x,y)  vertices without colors
		-0.5f, -0.5f//vertex3(-0.5,-0.5)(x,y)/
	};*/

	/*float vertices[] = {
		-0.5f, 0.5f, 1.0f, 0.0f, 0.0f, // Top-left
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, // Top-right
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-right
		//-0.5f, -0.5f, 1.0f, 1.0f, 1.0f  // Bottom-left
	};*/

	float vertices[] = {
		0.0f, 0.5f, 0.0f,  // Vertex 1: Red
		0.5f, -0.5f, 0.5f, // Vertex 2: Green
		-0.5f, -0.5f, 1.0f// Vertex 3: Blue
	};

	GLuint elements[] = {
		0, 1, 2,
		2, 3, 0
	};

	GLuint vao;
	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);//generates 1 buffer

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

	//compile vertex shader

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);

	glCompileShader(vertexShader);

	GLint status;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	if (!status){
		printf("error compiling vertex shader");
		return 4;
	}
	//compile fragment shader

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);   //glShaderSource(fragmentShader, 1, &fragmentSource, NULL);replace &fragmentShader with &colorUnifrom 
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
	
	if (!status){
		printf("error compiling fragment shader");
		return 5;
	}

	
	//combine shaders into a program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor"); // not necessary with only one output

	glLinkProgram(shaderProgram);

	glUseProgram(shaderProgram);

	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), 0);
	glEnableVertexAttribArray(posAttrib);

	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 1, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

	//GLint uniColor = glGetUniformLocation(shaderProgram, "triangleColor");
	//glUniform3f(uniColor, 1.0f, 0.0f, 0.0f);

	while (!glfwWindowShouldClose(window)){
		//auto t_start = std::chrono::high_resolution_clock::now();

		glfwSwapBuffers(window);
		glfwPollEvents();
		
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//auto t_now = std::chrono::high_resolution_clock::now();
		//float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();

		//glUniform3f(uniColor, (sin(time*4.0f) + 1.0f) / 2.0f, 0.0f, 0.0f);

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

	}

	glfwTerminate();
	return 0;
}