#pragma once


struct Geometry
{
	unsigned vbo, ibo, vao, size;
	//vertex buffer object : an array of vertices
	// index buffewr object: an array of indces (Triagnles)
	//vertext Array object: groups the two with some formatting
	//size: number of triagnles

};


Geometry makeGeometry(const struct Vertex *verts, size_t vsize,
	const unsigned int *tris, size_t tsize);
void freeGeometry(Geometry &);
struct  Shader
{
	unsigned handle;
};

Shader makeShader(const char *vsource, const char *fsource);

Shader loadShader(const char *vpath, const char *fpath);

void freeShader(Shader &);

void draw(const Shader &, const Geometry &);
void draw(const Shader &, const Geometry &, float time);
Geometry loadOBJ(const char *path);



void draw(const Shader &, const Geometry &, const float M[16], const float V[16], const float P[16], float time);