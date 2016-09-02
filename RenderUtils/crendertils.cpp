#define GLEW_STATIC
#include "GLEW\glew.h"
#include "GLFW\glfw3.h"
#include <cstdio>
#include "crendertiles.h"
#include "Vertex.h"


Geometry makeGeometry(const Vertex * verts, size_t vsize, const unsigned int * tris, size_t tsize)
{
	Geometry retval;

	// define the variables
	glGenBuffers(1,&retval.vbo); //store vertices
	glGenBuffers(1, &retval.ibo); // store indices
	glGenVertexArrays(1, &retval.vao); // store attribute infromotaion



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
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)Vertex::POSITION);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)Vertex::COLOR);


	// unscope the variables
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


		retval.size = tsize;
	return retval;
}

void freeGeometry(Geometry &geo)
{
	glDeleteBuffers(1, &geo.vbo);
	glDeleteBuffers(1, &geo.ibo);
	glDeleteVertexArrays(1, &geo.vao);
	geo = { 0,0,0,0 };
}

Shader makeShader(const char * vsource, const char * fsource)
{
	Shader retval;
	//creat our variables
	retval.handle = glCreateProgram();
	unsigned vs = glCreateShader(GL_VERTEX_SHADER);
	unsigned fs = glCreateShader(GL_FRAGMENT_SHADER);
	// initialize variables
	glShaderSource(vs, 1, &vsource, 0);
	glShaderSource(fs, 1, &fsource, 0);
	// complie shaders
	glCompileShader(vs);
	glCompileShader(fs);
	//link the shaders into a single program
	glAttachShader(retval.handle, vs);
	glAttachShader(retval.handle, fs);
	glLinkProgram(retval.handle);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return retval;
}

#include <fstream>
#include <istream>
#include <string>
char getTextFromFile(char *dest, size_t dlen, const char *path)
{
	return false;
}
std::string cppStyleFileToString(const char *path)
{
	std::ifstream infile{ path };
	std::string file_contents{ std::istreambuf_iterator<char>(infile),
		std::istreambuf_iterator<char>() };
	return file_contents;
}

Shader loadShader(const char *vpath, const char *fpath)
{
	char vsource[5012]; //max of 5012 characters in source
	char fsource[5012];	//will probably change that later

	getTextFromFile(vsource, 5012, vpath);
	getTextFromFile(fsource, 5012, fpath);

	std::string vs = cppStyleFileToString(vpath);
	std::string fs = cppStyleFileToString(fpath);
	return makeShader(vs.c_str(), fs.c_str());
}
void freeShader(Shader &shader)
{
	glDeleteProgram(shader.handle);
	shader.handle = 0;
}

void draw(const Shader &shader, const Geometry &geometry)
{
	glUseProgram(shader.handle);

	//binding the VAO aslo binds the IBO (tri) and VOB (verts)
	glBindVertexArray(geometry.vao);
	//draw elements will draw the vertices that are currently bound using an array of indices
	//IF an IBO IS BOUND, we dont need to provide any indices.
	glDrawElements(GL_TRIANGLES, geometry.size , GL_UNSIGNED_INT, 0);
}
void draw(const Shader &shader, const Geometry &geometry, float time)
{
	glUseProgram(shader.handle);
	glBindVertexArray(geometry.vao);

	int loc = glGetUniformLocation(shader.handle, "time");

	glUniform1f(loc, time);

	glDrawElements(GL_TRIANGLES, geometry.size, GL_UNSIGNED_INT, 0);


}
#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
#include "OBJ\tiny_obj_loader.h"
#include <random>

Geometry loadOBJ(const char *path)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;

	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path);

	Vertex   *verts = new Vertex[attrib.vertices.size() / 3];
	unsigned * tris = new unsigned[shapes[0].mesh.indices.size()];

	for (int i = 0; i < attrib.vertices.size() / 3; ++i)
	{
		verts[i] = { attrib.vertices[i * 3],
			attrib.vertices[i * 3 + 1],
			attrib.vertices[i * 3 + 2], 1 };

		verts[i].color[0] = rand() * 1.0f / RAND_MAX;
		verts[i].color[1] = rand() * 1.0f / RAND_MAX;
		verts[i].color[2] = rand() * 1.0f / RAND_MAX;
		verts[i].color[3] = 1;
	}

	for (int i = 0; i < shapes[0].mesh.indices.size(); ++i)
		tris[i] = shapes[0].mesh.indices[i].vertex_index;

	Geometry retval = makeGeometry(verts, attrib.vertices.size() / 3,
		tris, shapes[0].mesh.indices.size());

	delete[] verts;
	delete[] tris;
	// then we can call makeGeometry as per normal.
	return retval;
}

void draw(const Shader &s, const Geometry &g,
	const float M[16], const float V[16], const float P[16], float time)
{
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glUseProgram(s.handle);
	glBindVertexArray(g.vao);


	glUniformMatrix4fv(0, 1, GL_FALSE, P);
	glUniformMatrix4fv(1, 1, GL_FALSE, V);
	glUniformMatrix4fv(2, 1, GL_FALSE, M);

	glUniform1f(3, time);

	glDrawElements(GL_TRIANGLES, g.size, GL_UNSIGNED_INT, 0);
}