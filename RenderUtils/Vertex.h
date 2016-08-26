#pragma once

struct  Vertex
{
	float positon[4];
	float color[4];

	enum {
		POSITION = 0,
		COLOR = 16
	};

};