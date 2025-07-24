#pragma once
#include "common.h"

class GameTimer
{
public:

	GameTimer(){}
	~GameTimer(){}

	void Update()
	{
		LONGLONG now = GetNowHiPerformanceCount();
		deltaTime = (now - oldTime) / 1000.0f;
		oldTime = now;
	}

	float GetDeltaTime() const { return deltaTime; }

private:
	float deltaTime = 0.0f;
	LONGLONG oldTime = GetNowHiPerformanceCount();
};

