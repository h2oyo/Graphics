#pragma once


class Window
{
private:
	friend class Input;

	int width, height;
	char title[64];
	bool isInitialized = false;
	// forward declare
	struct GLFWwindow *winHandle = nullptr;

public:
	bool init(int a_width = 800, int a_height = 600, char *a_title = "I am a Potato");  // Setup, Start, Initialize... etc.
	bool step();									      // Update, Refresh,
	bool term();										  // Kill, Terminate, DIEDIEDIE, POTG

};