#pragma once
#include "PlayerStateActionBase.h"
#include "PlayerData.h"

class Player;

class Aim : public PlayerStateActionBase
{
public:
	Aim(int& modelHandle, AnimState& oldAnimState,
		AnimState& nowAnimState, PlayerData& playerData);
	~Aim();

	void Initialize(int& modelHandle, PlayerData& playerData)override;
	bool MotionUpdate(PlayerData& playerData)override;
	void AimMove(PlayerData& playerData)override;

	std::pair<VECTOR, PlayerData> Update(const VECTOR& cameraDirection,
		const std::vector<std::shared_ptr<BaseObject>>& fieldObjects, Player& player)override;

	void Enter(PlayerData& playerData) override;		//ó‘Ô‚É“ü‚Á‚½‚Æ‚«
	void Exit(PlayerData& playerData) override;			//ó‘Ô‚ğ”²‚¯‚é‚Æ‚«

	VECTOR Command(const VECTOR& cameraDirection, PlayerData& playerData, Player& player)override;
	void ChangeArrowState(PlayerData& playerData);
	void Change_draw(PlayerData& playerData);
	void Change_recoil(PlayerData& playerData);
	void Change_aim(PlayerData& playerData);

private:
	static constexpr float playAnimSpeed = 0.5f;
	int arrowStateNum_now;
	int animationNum_now;

	enum ArrowState : int
	{
		draw,		//’e‚­
		recoil,		//•ú‚Â
		aim			//ƒGƒCƒ€’†
	};
};

