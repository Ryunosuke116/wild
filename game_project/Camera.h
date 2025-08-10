#pragma once
#include <vector>
#include <memory>
#include "BaseObject.h"
#include "PlayerData.h"

class Camera
{
private:
	VECTOR cameraPos;			//�J�����̍��W
	VECTOR cameraPos_aim;		//�G�C�����̃J����
	VECTOR aimPosition_usual;	
	VECTOR cameraDirection;		//�J�����̌����Ă������
	VECTOR centerPos;			//�J�����ƒ����_�̊Ԃ̓_
	VECTOR centerPos_aim;		//�G�C�����̃J�����ƒ����_�̊Ԃ̓_
	VECTOR recoilPos;
	VECTOR targetPos;			//�J�����̒����_	
	VECTOR targetPos_aim;		//�G�C�����̒����_
	VECTOR aimDirection;		//�G�C���̕���
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
	// �Q�b�^�[
	///////////////////////////////////////////////////////
	VECTOR GetCameraDirection() { return cameraDirection; }
	VECTOR GetTargetPosition() { return targetPos; }

};

