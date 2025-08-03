#pragma once
#include <memory>
#include <vector>
#include "BaseObject.h"
#include "PlayerData.h"

class Input;
class Player;

class PlayerStateActionBase
{
public:

	struct AnimState
	{
		int AttachIndex;			//アニメーション情報
		float PlayTime_anim;		//再生時間
		float TotalPlayTime_anim;	//総再生時間
		float PlayAnimSpeed;		//アニメーションスピード
	};

	PlayerStateActionBase(int& modelHandle,
		AnimState& oldAnimState, AnimState& nowAnimState);
	~PlayerStateActionBase() {};

	virtual void Initialize(int& modelHandle, PlayerData& playerData)abstract;

	virtual bool MotionUpdate(PlayerData& playerData);
	
virtual std::pair<VECTOR, PlayerData> Update(const VECTOR& cameraDirection,
		const std::vector<std::shared_ptr<BaseObject>>& fieldObjects, Player& player)abstract;
	virtual void Enter(PlayerData& playerData) abstract;		//状態に入ったとき
	virtual void Exit(PlayerData& playerData) abstract;			//状態を抜けるとき
	virtual VECTOR Move(const VECTOR& cameraDirection, PlayerData& playerData);
	virtual void AimMove(PlayerData& playerData);
	virtual void SwitchingAnimation(const int& animNum);

	void SetOldAnimState();
	void ResetOldAnimState();
	void ResetNowAnimState();
	bool Draw();


	virtual VECTOR Command(const VECTOR& cameraDirection, PlayerData& playerData, Player& player)abstract;

	//////////////////////////////////////////////
	// ゲッター
	//////////////////////////////////////////////
	//int GetPrevAttachIndex() { return oldAnimState.AttachIndex; }
	const AnimState GetOldAnimState() const { return oldAnimState; }
	const AnimState GetNowAnimState() const { return nowAnimState; }
	const float GetAnimBlendRate() { return animBlendRate; }


	void SetPlayAnimSpeed_now(const float set) { nowAnimState.PlayTime_anim = set; }
	void SetAnimNumber_old(const int num) { animNumber_old = num; }

protected:
	static constexpr float	AnimBlendSpeed = 0.1f;		// アニメーションのブレンド率変化速度

	int modelHandle;			//モデルハンドル
	float animBlendRate;
	int animNumber_old;
	bool isPush;
	bool isMove_Bow;			//弓を構えているか

	AnimState oldAnimState;
	AnimState nowAnimState;

	//移動
	//VECTOR moveVec;

	std::shared_ptr<Input> input = NULL;

	Player* player;

};

