#include "window.h"
#include "crendertiles.h"
#include "Vertex.h"
#include "Gallery.h"

int main()
{
	Window window;
	Gallery gallery;

	window.init(600, 600);
	gallery.init();

	gallery.loadShader("SIMPLE", "../res/shaders/simpleVert.txt",
		"../res/shaders/simpleFrag.txt");


	gallery.loadObjectOBJ("SPHERE", "../res/models/sphere.obj");
	gallery.loadObjectOBJ("CUBE", "../res/models/cube.obj");
	float time = 0;

	while (window.step())
	{
		time += 0.016667f;
		draw(gallery.getShader("SIMPLE"), gallery.getObject("SPHERE"),time);
		draw(gallery.getShader("SIMPLE"), gallery.getObject("CUBE"), time*2);
		draw(gallery.getShader("SIMPLE"), gallery.getObject("SPHERE"), time*3);
		draw(gallery.getShader("SIMPLE"), gallery.getObject("CUBE"), time/2);
	}

	gallery.term();
	window.term();
	return 0;
}