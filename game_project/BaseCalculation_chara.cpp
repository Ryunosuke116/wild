#include "common.h"
#include "BaseCalculation_chara.h"

float BaseCalculation_chara::Gravity(const bool& isGround,const float& jumpPower)
{
	float returnPower = jumpPower;

	if (!isGround)
	{
		return returnPower += gravity;
	}
	
	//�ڒn���͏d�͂�t�^���Ȃ�
	return returnPower;
}
