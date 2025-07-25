#pragma once
#include "PlayerStateActionBase.h"
#include "PlayerData.h"

class Run_standard : public PlayerStateActionBase
{
public:
	Run_standard(int& modelHandle, AnimState& oldAnimState,
		AnimState& nowAnimState, PlayerData& playerData);
	~Run_standard();

	virtual void Initialize(int& modelHandle, PlayerData& playerData)override;

	std::pair<VECTOR, PlayerData> Update(const VECTOR& cameraDirection,
		const std::vector<std::shared_ptr<BaseObject>>& fieldObjects, Player& player)override;

	void Enter(PlayerData& playerData) override;		//状態に入ったとき
	void Exit(PlayerData& playerData) override;			//状態を抜けるとき

	VECTOR Command(const VECTOR& cameraDirection, PlayerData& playerData, Player& player)override;
	void JumpMove(PlayerData& playerData, Player& player);

private:
	static constexpr float playAnimSpeed = 0.4f;	    // 移動速度

};

