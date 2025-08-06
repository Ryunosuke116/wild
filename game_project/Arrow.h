#pragma once
#include "BaseObject.h"

class Arrow : public BaseObject
{
public:
	Arrow();
	~Arrow();

	void Initialize(const int& modelHandle_player,
		const std::string& frameName, const float& angle_player);
	void Initialize()override;
	void Update()override;
	bool Draw()override;

private:
	float angle;
	VECTOR direction;
};

