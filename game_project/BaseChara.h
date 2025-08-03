#pragma once

#include "CollisionData.h"

class BaseChara
{
protected:

	static constexpr float	MoveSpeed = 0.4f;	    // 移動速度
	static constexpr float angleSpeed = 0.2f;
	static constexpr float	AnimBlendSpeed = 0.1f;		// アニメーションのブレンド率変化速度
	static constexpr float PlayAnimSpeed = 0.5f;	//アニメ再生速度
	
	int modelHandle;
	int motionNum;
	int HP;
	int nowFrameNumber;
	int animNumber_Now;

	int prevAttachIndex;			//前のアニメーション情報
	float prevPlayTime_anim;		//前回の再生時間
	float prevPlayAnimSpeed;		//前回のアニメーションスピード

	int currentAttachIndex;			//現在のアニメーション情報
	float currentPlayTime_anim;		//現在の再生時間
	float currentPlayAnimSpeed;		//前回のアニメーションスピード

	float animBlendRate;
	float keepPlayTime_anim;

	float totalTime_anim;
	float angle;
	float rotate_x;
	float radius;

	VECTOR targetMoveDirection;
	VECTOR framePosition;
	VECTOR position;
	VECTOR moveVec;
	VECTOR moveDirection;

	PositionData positionData;
	CollisionResult collision_result;
public:

	BaseChara();
	~BaseChara();

	//純粋仮想関数
	virtual void Initialize()abstract;
	virtual void Update()abstract;
	virtual bool Draw()abstract;
	virtual void Draw_log()abstract;

	virtual void ChangeMotion(const int& motionNum, const float playAnimSpeed);
	virtual void MotionUpdate();
	void UpdateAngle(const VECTOR& direction, bool& isTurn_right);

	void PositionUpdate();

	/////////////////////////////////////////////
	// ゲッター
	/////////////////////////////////////////////
	VECTOR GetPosition() const { return position; }
	VECTOR GetFramePosition()const { return framePosition; }
	VECTOR GetMoveVec()const { return moveVec; }
	PositionData GetPositionData() const { return positionData; }
	virtual float GetRadius()const abstract;

	/////////////////////////////////////////////
	// セッター
	/////////////////////////////////////////////

	void SetPositionData(const PositionData& newData) { positionData = newData; }
	void SetCollision_result(const CollisionResult& newResult) { collision_result = newResult; }
};


