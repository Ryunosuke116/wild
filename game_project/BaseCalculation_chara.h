#pragma once
class BaseCalculation_chara
{
public:
	VECTOR Gravity(const bool& isGround, const VECTOR& moveVec);

protected:
	static constexpr float gravity = -0.01f;

	float velocity_gravity;
	float deltaTime;
};

