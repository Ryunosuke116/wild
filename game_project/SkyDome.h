#pragma once
#include "BaseObject.h"
class SkyDome : public BaseObject
{
private:

public:
	SkyDome();
	SkyDome(const char* path);
	~SkyDome();
	void Initialize()override;
	void Update()override;
	void Update(const VECTOR& playerPos);
	bool Draw()override;

};

