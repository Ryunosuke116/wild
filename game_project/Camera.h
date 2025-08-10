#pragma once
#include <vector>
#include <memory>
#include "BaseObject.h"
#include "PlayerData.h"

class Camera
{
private:
	VECTOR cameraPos;			//カメラの座標
	VECTOR cameraPos_aim;		//エイム時のカメラ
	VECTOR aimPosition_usual;	
	VECTOR cameraDirection;		//カメラの向いている方向
	VECTOR centerPos;			//カメラと注視点の間の点
	VECTOR centerPos_aim;		//エイム時のカメラと注視点の間の点
	VECTOR recoilPos;
	VECTOR targetPos;			//カメラの注視点	
	VECTOR targetPos_aim;		//エイム時の注視点
	VECTOR aimDirection;		//エイムの方向
	VECTOR oldTargetPos;		

	float angle;
	float distance;
	float a;
	float distance_aim;
	float t;

	bool isCalc;

	static constexpr float cameraSpeed = 0.02f;
	static constexpr float cameraSpeed_ = 0.01f;
	static constexpr float radius = 1.0f;

	static constexpr float distance_max = 60.0f;

public:
	Camera();
	~Camera();

	void Initialize(const VECTOR& playerPosition);
	void Update(const VECTOR& playerPosition,
		const PlayerData& playerData,
		const std::vector<std::shared_ptr<BaseObject>>& fieldObjects);
	void Update_layout();
	void Update_aim();
	bool Draw();
	void RotateUpdate(const PlayerData& playerData);
	void targetPosCalc();
	void Leap(VECTOR& changePosition, const VECTOR& playerPosition, const float& speed);


	void CameraPosCalc(const int& mapHandle);

	///////////////////////////////////////////////////////
	// ゲッター
	///////////////////////////////////////////////////////
	VECTOR GetCameraDirection() { return cameraDirection; }
	VECTOR GetTargetPosition() { return targetPos; }

};

