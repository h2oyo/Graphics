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

Geomtry makeGemotry(Vertex * verts, size_t vsize, unsigned int * tris, size_t tsize)
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