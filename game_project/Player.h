#pragma once
#include "BaseChara.h"
#include "Calculation.h"
#include  "nlohmann/json.hpp"
#include "PlayerData.h"
#include "PlayerStateActionBase.h"
#include "AnimationChanger.h"
#include "PlayerCalculation.h"
#include "ArrowObserver.h"

class PlayerStateActionBase;
class Idle;
class Run;

class Player : public BaseChara
{
private:

	static constexpr float modelScale = 0.1f;
	static constexpr float modelScale_arrow = 0.01f;
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
	
	int arrowHandle;
	int upHandle;
	int attachIndex;
	int bottomHandle;

	float radian_wall;
	float degree_pad_now;
	float degree_difference;
	float degree_pad_wall_difference;
	float angle_aim;

	bool isPush;					//ボタンを押したか
	bool isChange_falling;				//アニメーションを変更するか
	bool isCalc;
	bool isCalc_moveVec;

	std::string frameName;

	//他クラス
	std::shared_ptr<PlayerStateActionBase> nowState;
	PlayerStateActionBase::AnimState oldAnimState;
	PlayerStateActionBase::AnimState nowAnimState;
	PlayerData playerData;
	std::shared_ptr<AnimationChanger> animationChacger = NULL;
	std::vector<std::weak_ptr<ArrowObserver>> observers;

public:

	void AddObserver(std::shared_ptr<ArrowObserver> observer) { observers.push_back(observer); }
	void RemoveObserver(std::shared_ptr<ArrowObserver> observer);
	void NotifyRecoilArrow();

	Player();
	~Player();

	void Initialize()override;
	void Update()override;
	void Update(const float& deltaTime, const VECTOR& cameraDirection,
		const std::vector<std::shared_ptr<BaseObject>>& fieldObjects);
	
	bool Draw()override;
	void Draw_log()override;
	void MotionUpdate()override;

	void ChengeState();

	VECTOR Command(const VECTOR& cameraDirection, PlayerData& playerData);
	VECTOR Move(const VECTOR& cameraDirection, PlayerData& playerData);
	void JumpMove(PlayerData& playerData);
	void Receive_CollisionResult();

	std::shared_ptr<PlayerCalculation> playerCalculation = NULL;
	std::vector<MATRIX> matrixs;
	//////////////////////////////////
	//　ゲッター
	///////////////////////////////////

	int GetArrowHandle() { return arrowHandle; }
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
	bool GetIsRecoil_arrow()const { return playerData.isRecoil_arrow; }
	std::string GetFrameName()const { return frameName; }

	//////////////////////////////////
	/// セッター
	///////////////////////////////// 
	void SetPos(const VECTOR set) { position = set; }
	void SetIsRecoil_arrow(const bool set) { playerData.isRecoil_arrow = set; }
	void SetAngle_aim(const float set) { angle_aim = set; }
};

