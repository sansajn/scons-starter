// glew3, OpenGL ES 2.0, triangle sample
#include <iostream>
#include <cassert>
#include <GLFW/glfw3.h>

using std::cout, std::endl;

constexpr GLuint WIDTH = 800,
	HEIGHT = 600;

GLchar const * vertex_shader_source = R"(
#version 100
attribute vec3 position;
void main() {
	gl_Position = vec4(position, 1.0);
})";

GLchar const * fragment_shader_source = R"(
#version 100
void main() {
	gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
})";

GLfloat const vertices[] = {
	0.0f, 0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f,
};

GLint get_shader_program(char const * vertex_shader_source, char const * fragment_shader_source);


int main(int argc, char * argv[]) 
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	
	GLFWwindow * window = glfwCreateWindow(WIDTH, HEIGHT, __FILE__, NULL, NULL);
	assert(window);
	glfwMakeContextCurrent(window);

	cout << "GL_VENDOR: " << glGetString(GL_VENDOR) << "\n" 
		<< "GL_VERSION: " << glGetString(GL_VERSION) << "\n"
		<< "GL_RENDERER: " << glGetString(GL_RENDERER) << "\n"
		<< "GLSL_VERSION: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
	
	GLuint shader_program = get_shader_program(vertex_shader_source, fragment_shader_source);
	GLint position_attr = glGetAttribLocation(shader_program, "position");

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glViewport(0, 0, WIDTH, HEIGHT);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(position_attr, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(position_attr);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	while (!glfwWindowShouldClose(window)) 
	{
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shader_program);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window);
	}
	
	glDeleteBuffers(1, &vbo);
	glfwTerminate();
	glDeleteProgram(shader_program);
	
	return 0;
}

GLint get_shader_program(char const * vertex_shader_source, char const * fragment_shader_source)
{
	enum Consts {INFOLOG_LEN = 512};
	GLchar infoLog[INFOLOG_LEN];
	GLint fragment_shader;
	GLint shader_program;
	GLint success;
	GLint vertex_shader;

	/* Vertex shader */
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
	glCompileShader(vertex_shader);
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex_shader, INFOLOG_LEN, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" 
			<< infoLog << endl;
	}

	/* Fragment shader */
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
	glCompileShader(fragment_shader);
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment_shader, INFOLOG_LEN, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" 
			<< infoLog << endl;
	}

	/* Link shaders */
	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader_program, INFOLOG_LEN, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" 
			<< infoLog << endl;
	}

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	return shader_program;
}
