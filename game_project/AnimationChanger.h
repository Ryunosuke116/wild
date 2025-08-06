#pragma once
#include "PlayerData.h"
#include "PlayerStateActionBase.h"

class AnimationChanger
{
public:

	AnimationChanger();
	~AnimationChanger();

	std::shared_ptr<PlayerStateActionBase> ChangeState(int& modelHandle, int& bottomHandle,
		PlayerData& playerData,
		std::shared_ptr<PlayerStateActionBase>& nowState);
	
	bool SetIsChange_Bow();

	void SetOldAnimState(PlayerStateActionBase::AnimState animState);
	void SetNowAnimState(PlayerStateActionBase::AnimState animState);

	int GetAnimNumber_now() { return animNumber_Now; }

private:

	int animNumber_Now;

	PlayerStateActionBase::AnimState oldAnimState;
	PlayerStateActionBase::AnimState nowAnimState;
	
};

