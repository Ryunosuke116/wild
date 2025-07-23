#pragma once
#include "PlayerStateActionBase.h"
#include "PlayerData.h"

class Run_standard : public PlayerStateActionBase
{
public:
	Run_standard(int& modelHandle, AnimState& oldAnimState,
		AnimState& nowAnimState, PlayerData& playerData);
	~Run_standard();

	//bool MotionUpdate(PlayerData& playerData)override;
	std::pair<VECTOR, PlayerData> Update(const VECTOR& cameraDirection,
		const std::vector<std::shared_ptr<BaseObject>>& fieldObjects, Player& player)override;

	void Enter(PlayerData& playerData) override;		//��Ԃɓ������Ƃ�
	void Exit(PlayerData& playerData) override;			//��Ԃ𔲂���Ƃ�
	VECTOR Command(const VECTOR& cameraDirection, PlayerData& playerData, Player& player)override;
	//VECTOR Move(const VECTOR& cameraDirection, PlayerData& playerData)override;
	void JumpMove(PlayerData& playerData, Player& player);

private:
	static constexpr float playAnimSpeed = 0.4f;	    // �ړ����x

};

