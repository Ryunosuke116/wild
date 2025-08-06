#pragma once
#include <vector>
#include <memory>
#include "BaseObject.h"
#include "PlayerData.h"

class Camera
{
private:
	VECTOR cameraPos;
	VECTOR aimPosition_usual;
	VECTOR lookPosition;
	VECTOR keepPlayerPosition;
	VECTOR keepEnemyPosition;
	VECTOR cameraDirection;
	VECTOR centerPos;
	VECTOR centerPos_aim;
	VECTOR recoilPos;
	VECTOR targetPos;
	VECTOR targetPos_aim;
	VECTOR cameraPos_aim;
	VECTOR aimDirection;
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
	// ÉQÉbÉ^Å[
	///////////////////////////////////////////////////////
	VECTOR GetCameraDirection() { return cameraDirection; }
	VECTOR GetTargetPosition() { return targetPos; }

};

