#pragma once
#include "BaseChara.h"
#include "Calculation.h"
#include  "nlohmann/json.hpp"
#include "PlayerData.h"
#include "PlayerStateActionBase.h"
#include "AnimationChanger.h"
#include "PlayerCalculation.h"

class PlayerStateActionBase;
class Idle;
class Run;

class Player : public BaseChara
{
private:

	static constexpr float modelScale = 0.1f;
	static constexpr float MaxMoveSpeed = 1.6f;	    // 移動速度
	static constexpr float rollMoveSpeed_max = 2.5f;	//ロール速度
	static constexpr float angleSpeed = 0.3f;
	static constexpr float addJumpPower = 1.7f;		//ジャンプパワー
	static constexpr float gravity = -0.06f;
	static constexpr float run_wall_rotate_x = 30.0f;
	static constexpr float entryDegree_wallRun = 30.0f;
	static constexpr float radius = 5.0f;

	VECTOR linePos_end;
	VECTOR centerPosition;
	VECTOR topPosition;
	VECTOR bottomPosition;
	VECTOR moveVec_normal;
	VECTOR hangingPoint;
	VECTOR headPos;
	VECTOR handPos_right;
	VECTOR handPos_left;
	VECTOR handCenterPos;
	VECTOR moveDirection_now;		//現在向いている方向
	VECTOR padInput_now;
	

	float radian_wall;
	float degree_pad_now;
	float degree_difference;
	float degree_pad_wall_difference;

	bool isPush;					//ボタンを押したか
	bool isChange_falling;				//アニメーションを変更するか
	bool isCalc;
	bool isCalc_moveVec;

	//他クラス
	std::shared_ptr<PlayerStateActionBase> nowState;
	PlayerStateActionBase::AnimState oldAnimState;
	PlayerStateActionBase::AnimState nowAnimState;
	PlayerData playerData;
	std::shared_ptr<AnimationChanger> animationChacger = NULL;

public:
	Player();
	~Player();

	void Initialize()override;
	void Update()override;
	void Update(const float& deltaTime, const VECTOR& cameraDirection,
		const std::vector<std::shared_ptr<BaseObject>>& fieldObjects);
	
	bool Draw()override;

	void ChengeState();

	VECTOR Command(const VECTOR& cameraDirection, PlayerData& playerData);
	VECTOR Move(const VECTOR& cameraDirection, PlayerData& playerData);
	void JumpMove(PlayerData& playerData);
	void Receive_CollisionResult();

	std::shared_ptr<PlayerCalculation> playerCalculation = NULL;

	//////////////////////////////////
	//　ゲッター
	///////////////////////////////////

	VECTOR GetCenterPos() { return centerPosition; }
	VECTOR GetTopPos() { return topPosition; }
	VECTOR GetBottomPos() { return bottomPosition; }
	VECTOR GetlinePos_end() { return linePos_end; }
	VECTOR GetHangingPoint() { return hangingPoint; }
	VECTOR GetHeadPos() { return headPos; }
	VECTOR GetHandPos_right() { return handPos_right; }
	VECTOR GetHandPos_left() { return handPos_left; }
	PlayerData GetData() { return playerData; }
	float GetRadius() const override { return radius; }

	//////////////////////////////////
	/// セッター
	///////////////////////////////// 
	void SetPos(VECTOR newPos) { position = newPos; }

};

