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
	
	//Ú’n’†‚Íd—Í‚ğ•t—^‚µ‚È‚¢
	return returnVec;
}
