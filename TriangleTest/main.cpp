#include "window.h"
#include "crendertiles.h"
#include "Vertex.h"
#include "Gallery.h"
#include "Timer.h"
#include "Input.h"

#include "Camera.h"


#include "procgen.h"

int main()
{
	Window window;
	Gallery gallery;
	Time	time;
	Input   input;

	window.init(1280, 720);
	gallery.init();
	input.init(window);
	time.init();

	unsigned char pixels[] = { 255,255,0 };

	Texture tex = loadTexture("../res/textures/pants.jpg");


	gallery.loadShader("CAMERA", "../res/shaders/cameraVert.txt",
		"../res/shaders/cameraFrag.txt");


	gallery.loadShader("SIMPLE", "../res/shaders/simpleVert.txt",
		"../res/shaders/simpleFrag.txt");


	gallery.loadObjectOBJ("SPHERE", "../res/models/sphere.obj");
	gallery.loadObjectOBJ("CUBE", "../res/models/cube.obj");
//	float time = 0;

	while (window.step())
	{
	/*	time += 0.016667f;
		draw(gallery.getShader("SIMPLE"), gallery.getObject("SPHERE"),time);
		draw(gallery.getShader("SIMPLE"), gallery.getObject("CUBE"), time*2);
		draw(gallery.getShader("SIMPLE"), gallery.getObject("SPHERE"), time*3);
		draw(gallery.getShader("SIMPLE"), gallery.getObject("CUBE"), time/2);*/
	}

	gallery.term();
	window.term();
	return 0;
}