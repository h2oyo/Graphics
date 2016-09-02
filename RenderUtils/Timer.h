#pragma once


class Time 
{
	float currentTime;
	float previousTime;
	float deltaTime;
public:
	bool init();
	bool step();
	bool term();


	float getDeltaTime() const;
	float GetTotalTime() const;

	float getRealTime() const;
};