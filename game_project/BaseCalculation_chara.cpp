#include "common.h"
#include "BaseCalculation_chara.h"

VECTOR BaseCalculation_chara::Gravity(const bool& isGround,const VECTOR& moveVec)
{
	VECTOR returnVec = moveVec;

	if (!isGround)
	{
		velocity_gravity += gravity;

		returnVec.y += velocity_gravity;
	}
	else
	{
		velocity_gravity = 0.0f;
		returnVec.y = 0.0f;
	}
	
	//�ڒn���͏d�͂�t�^���Ȃ�
	return returnVec;
}
