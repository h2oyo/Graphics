#define GLEW_STATIC
#include "GLEW\glew.h"
#include "GLFW\glfw3.h"
#include <cstdio>
#include "crendertiles.h"
#include "Vertex.h"

void test()
{
	glfwInit();
	glfwTerminate();
}

Geomtry makeGemotry(const Vertex * verts, size_t vsize, const unsigned int * tris, size_t tsize)
{
	Geomtry retval;

	// define the variables
	glCreateBuffers(1,&retval.vbo); //store vertices
	glCreateBuffers(1, &retval.ibo); // store indices
	glCreateVertexArrays(1, &retval.vao); // store attribute infromotaion



	// scope the varaibales
		glBindVertexArray(retval.vao);
		glBindBuffer(GL_ARRAY_BUFFER, retval.vbo);//scope our vertices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, retval.ibo);//triangle is scoped

	// imitalize the varibbles
		glBufferData(GL_ARRAY_BUFFER, vsize * sizeof(Vertex), verts, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, tsize * sizeof(unsigned), tris, GL_STATIC_DRAW);

		//Attributes lets us tell openGL how the memory is laid out
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		// index of the attribute, number of elements, type, normalized?, size of vertex, offset
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)16);


	// unscope the variables
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


		retval.size = tsize;
	return retval;
}

void freeGeometry(Geomtry &geo)
{
	glDeleteBuffers(1, &geo.vbo);
	glDeleteBuffers(1, &geo.ibo);
	glDeleteVertexArrays(1, &geo.vao);
	geo = { 0,0,0 };
}

Shader makeShader(const char * vsource, const char * fsource)
{
	Shader retval;
	//creat our variables
	retval.handle = glCreateProgram();
	unsigned vs = glCreateShader(GL_VERTEX_SHADER);
	unsigned fs = glCreateShader(GL_FRAGMENT_SHADER);
	// initialize variables
	glShaderSource(vs, 1, &vsource, NULL);
	glShaderSource(fs, 1, &fsource, NULL);
	// complie shaders
	glCompileShader(vs);
	glCompileShader(fs);
	//link the shaders into a single program
	glAttachShader(retval.handle, vs);
	glAttachShader(retval.handle, fs);
	glLinkProgram(retval.handle);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return Shader();
}

void freeShader(Shader &shader)
{
	glDeleteProgram(shader.handle);
	shader.handle = 0;
}

void draw(const Shader &shader, const Geomtry &geomtry)
{
	glUseProgram(shader.handle);

	//binding the VAO aslo binds the IBO (tri) and VOB (verts)
	glBindVertexArray(geomtry.vao);
	//draw elements will draw the vertices that are currently bound using an array of indices
	//IF an IBO IS BOUND, we dont need to provide any indices.
	glDrawElements(GL_TRIANGLES, geomtry.size, GL_UNSIGNED_INT, 0);
}
