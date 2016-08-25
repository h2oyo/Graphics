#pragma once

class Window
{
	int width, height;
	char title[64];
	bool isInitialized = false;

	// forward declare (this type exites but not in this file)
	struct GLFWwindow *winHandle = nullptr;

public:
	bool init(int a_width = 800, int a_height = 600, char *a_title = "I am a Potato");// start
	bool step();// update
	bool term();//exit, kill


};