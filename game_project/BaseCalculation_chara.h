#pragma once
class BaseCalculation_chara
{
public:
	float Gravity(const bool& isGround, const float& jumpPower);

private:
	static constexpr float gravity = -0.06f;
};

