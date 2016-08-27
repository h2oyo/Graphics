#include "Gallery.h"

bool Gallery::makeShader(const char * name, const char * vsource, const char * fsource)
{
	if (!shaders.count(std::string(name)) )
	{
		shaders[name] = ::makeShader(vsource, fsource);
		return true;
	}
	else
	{
		printf("Shaders %s arleady exists!", name);
		return false;
	}

}

bool Gallery::loadShader(const char * name, const char * vpath, const char * fpath)
{
	if (!shaders.count(name))
	{
		shaders[name] = ::loadShader(vpath, fpath);
		return true;

	}
	else
	{
		fprintf(stderr, "Shader %s alreadyt exists!", name);

		return false;
	}

}

bool Gallery::makeObject(const char * name, const Vertex * verts, size_t vsize, const unsigned * tris, size_t tsize)
{
	

	objects[name] = ::makeGeometry(verts, vsize, tris, tsize);

	return true;
}

bool Gallery::loadObjectOBJ(const char * name, const char * path)
{

	objects[name] = loadOBJ(path);
	return false;
}

const Geometry & Gallery::getObject(const char * name)
{

	return objects[name];
	

}

const Shader & Gallery::getShader(const char * name)
{
	return shaders[name];
}

bool Gallery::init()
{


	return true;
}

bool Gallery::term()
{
	for each (auto shader in shaders)
	{
		freeShader(shader.second);
	}

	for each (auto object in objects)
		freeGeometry(object.second);
	return false;
}
