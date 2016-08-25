#pragma once
void test();


struct Geomtry
{
	unsigned vbo, ibo, vao, size;
	//vertex buffer object : an array of vertices
	// 

};

Geomtry makeGemotry(struct Vertex *verts, size_t vsize,
	unsigned int *tris, size_t tsize);
void freeGemotry(Geomtry &);