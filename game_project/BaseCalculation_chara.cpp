#include "common.h"
#include "BaseCalculation_chara.h"

float BaseCalculation_chara::Gravity(const bool& isGround,const float& jumpPower)
{
	float returnPower = jumpPower;

	if (!isGround)
	{
		return returnPower += gravity;
	}
	
	//Ú’n’†‚Íd—Í‚ğ•t—^‚µ‚È‚¢
	return returnPower;
}
