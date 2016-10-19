#pragma once

struct Geometry
{
	unsigned vbo, ibo, vao, size;
};

struct Shader
{
	unsigned handle;

	bool depthTest;
	bool additiveBlend;
	bool faceCulling;
};


struct Texture
{
	unsigned handle;
	unsigned width, height, format;
};

struct CubeTexture
{
	unsigned handle;
	unsigned width, height, format;
};

struct Framebuffer
{
	unsigned handle;
	unsigned width, height, nColors;

	Texture depth;
	Texture colors[8];
};