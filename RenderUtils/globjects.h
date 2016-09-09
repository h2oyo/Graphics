#pragma once
struct Geometry
{
	unsigned vbo, ibo, vao, size;
};

struct Shader {
	unsigned handle;
};
struct  Texture
{
	unsigned handle;
	int width, height, format;
};