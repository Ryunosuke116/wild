#pragma once
#include <vector>
#include <memory>
#include "BaseObject.h"

class Camera
{
private:
	VECTOR aimPosition;
	VECTOR aimPosition_usual;
	VECTOR lookPosition;
	VECTOR keepPlayerPosition;
	VECTOR keepEnemyPosition;
	VECTOR cameraDirection;
	VECTOR spherePosition;
	VECTOR centerPos;
	float angle;
	float distance;
	float a;

	static constexpr float cameraSpeed = 0.02f;
	static constexpr float cameraSpeed_ = 0.01f;
	static constexpr float radius = 1.0f;

	static constexpr float distance_max = 60.0f;

public:
	Camera();
	~Camera();

	void Initialize();
	void Update(const VECTOR& playerPosition, const std::vector<std::shared_ptr<BaseObject>>& fieldObjects);
	void Update_layout();
	bool Draw();
	void RotateUpdate();
	void PosCalc();
	void Leap(VECTOR& changePosition, const VECTOR& playerPosition, const float& speed);


	void CameraPosCalc(const int& mapHandle);

	///////////////////////////////////////////////////////
	// �Q�b�^�[
	///////////////////////////////////////////////////////
	VECTOR GetCameraDirection() { return cameraDirection; }
	VECTOR GetSpherePosition() { return spherePosition; }

};

