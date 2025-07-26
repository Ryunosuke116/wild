#pragma once
#include "PlayerStateActionBase.h"
#include "PlayerData.h"

class Player;

class Idle : public PlayerStateActionBase
{
public:
	Idle(int& modelHandle, AnimState& oldAnimState,
		AnimState& nowAnimState, PlayerData& playerData);
	~Idle();

	void Initialize(int& modelHandle, PlayerData& playerData)override;
	
	std::pair<VECTOR, PlayerData> Update(const VECTOR& cameraDirection,
		const std::vector<std::shared_ptr<BaseObject>>& fieldObjects, Player& player)override;

	void Enter(PlayerData& playerData) override;		//ó‘Ô‚É“ü‚Á‚½‚Æ‚«
	void Exit(PlayerData& playerData) override;			//ó‘Ô‚ğ”²‚¯‚é‚Æ‚«

	VECTOR Command(const VECTOR& cameraDirection, PlayerData& playerData, Player& player)override;
	
	void JumpMove(PlayerData& playerData,Player& player);

private:
	static constexpr float playAnimSpeed = 0.4f;	    // ˆÚ“®‘¬“x
	
	bool isRun;
	bool isJump;
	bool isMove;
};

