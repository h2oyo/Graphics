#pragma once
void test();


struct Geomtry
{
	unsigned vbo, ibo, vao, size;
	//vertex buffer object : an array of vertices
	// index buffewr object: an array of indces (Triagnles)
	//vertext Array object: groups the two with some formatting
	//size: number of triagnles

};


Geomtry makeGemotry(const struct Vertex *verts, size_t vsize,
	const unsigned int *tris, size_t tsize);
void freeGemotry(Geomtry &);
struct  Shader
{
	unsigned handle;
};

Shader makeShader(const char *vscript, const char *fsource);

void freeShader(Shader &);

void draw(const Shader &, const Geomtry &);