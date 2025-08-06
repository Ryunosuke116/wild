#pragma once
#include "PlayerStateActionBase.h"
#include "PlayerData.h"

class Player;

class Jump : public PlayerStateActionBase
{
public:
	Jump(int& modelHandle, int& bottomHandle, AnimState& oldAnimState,
		AnimState& nowAnimState, PlayerData& playerData);
	~Jump();

	void Initialize(int& modelHandle, int& bottomHandle, PlayerData& playerData)override;
	std::pair<VECTOR, PlayerData> Update(const VECTOR& cameraDirection,
		const std::vector<std::shared_ptr<BaseObject>>& fieldObjects, Player& player)override;

	bool MotionUpdate(PlayerData& playerData)override;


	void Enter(PlayerData& playerData) override;		//��Ԃɓ������Ƃ�
	void Exit(PlayerData& playerData) override;			//��Ԃ𔲂���Ƃ�

	VECTOR Command(const VECTOR& cameraDirection, PlayerData& playerData, Player& player)override;

private:
	static constexpr float playAnimSpeed_jump = 0.8f;	    // �ړ����x
	static constexpr float playAnimSpeed_runJump = 0.5f;

	bool isRun;
	bool isMove;
};

