#include "common.h"
#include "EffekseerForDXLib.h"
#include "HitCheck.h"
#include "PadInput.h"
#include "BaseObject.h"
#include "Camera.h"
#include "Calculation.h"


/// <summary>
/// �C���X�g���N�^
/// </summary>
Camera::Camera() :
	distance(0.0f),
	aimPosition_usual(VGet(0.0f, 0.0f, 0.0f))
{
	// �O���t�B�b�N�̕`���𗠉�ʂɃZ�b�g
	SetDrawScreen(DX_SCREEN_BACK);

	//���s0.1�`1000�܂ł��J�����̕`��͈͂Ƃ���
	SetCameraNearFar(3.5f, 5000.0f);


}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Camera::~Camera()
{

}

/// <summary>
/// ������
/// </summary>
void Camera::Initialize()
{
	aimPosition = VGet(30.0f, 15, -10);
	spherePosition = VGet(0.0f, 20.0f, 20.0f);
	a = -177.55f;
	distance = 50.0f;

	SetCameraPositionAndTarget_UpVecY(aimPosition, spherePosition);

	// DX���C�u�����̃J������Effekseer�̃J�����𓯊�����B
	Effekseer_Sync3DSetting();
}

/// <summary>
/// �X�V
/// </summary>
void Camera::Update(const VECTOR& playerPosition,
	const std::vector<std::shared_ptr<BaseObject>>& fieldObjects)
{

	centerPos = playerPosition;
	centerPos.y += 14.0f;

	aimPosition_usual.y = spherePosition.y + 15.0f;

	//�J�����ړ�����
	if (CheckHitKey(KEY_INPUT_A) ||
		PadInput::GetJoyPad_x_right() < 0.0f)
	{
		a += 2.0f;
	}
	if (CheckHitKey(KEY_INPUT_D) ||
		PadInput::GetJoyPad_x_right() > 0.0f)
	{
		a -= 2.0f;
	}
	if (CheckHitKey(KEY_INPUT_W))
	{
		aimPosition.y += 0.5f;
	}
	if (CheckHitKey(KEY_INPUT_S))
	{
		aimPosition.y -= 0.5f;
	}

	RotateUpdate();
	for (const auto& fieldObject : fieldObjects)
	{
		CameraPosCalc(fieldObject->GetModelHandle());
	}

	SetCameraPositionAndTarget_UpVecY(aimPosition, spherePosition);

	cameraDirection = VSub(spherePosition, aimPosition);
	cameraDirection = VNorm(cameraDirection);

}

/// <summary>
/// layout�p�J����
/// </summary>
void Camera::Update_layout()
{
	//y�����ړ�
	if (CheckHitKey(KEY_INPUT_UP) && CheckHitKey(KEY_INPUT_LCONTROL))
	{
		aimPosition.y += 1.0f;
		spherePosition.y += 1.0f;
	}
	//z�����ړ�
	else if (CheckHitKey(KEY_INPUT_UP))
	{
		aimPosition.z += 1.0f;
		spherePosition.z += 1.0f;
	}

	//y�����ړ�
	if (CheckHitKey(KEY_INPUT_DOWN) && CheckHitKey(KEY_INPUT_LCONTROL))
	{
		aimPosition.y -= 1.0f;
		spherePosition.y -= 1.0f;
	}
	//z�����ړ�
	else if (CheckHitKey(KEY_INPUT_DOWN))
	{
		aimPosition.z -= 1.0f;
		spherePosition.z -= 1.0f;
	}

	//�E��]
	if (CheckHitKey(KEY_INPUT_RIGHT) &&
		CheckHitKey(KEY_INPUT_LSHIFT))
	{
		a += 2.0f;
	}
	//x���E�ړ�
	else if (CheckHitKey(KEY_INPUT_RIGHT) ||
		PadInput::GetJoyPad_x_right() < 0.0f)
	{
		aimPosition.x += 1.0f;
		spherePosition.x += 1.0f;
	}

	//����]
	if (CheckHitKey(KEY_INPUT_LEFT) &&
		CheckHitKey(KEY_INPUT_LSHIFT))
	{
		a -= 2.0f;
	}
	//x�����ړ�
	else if (CheckHitKey(KEY_INPUT_LEFT) ||
		PadInput::GetJoyPad_x_right() > 0.0f)
	{
		aimPosition.x -= 1.0f;
		spherePosition.x -= 1.0f;
	}

	if (CheckHitKey(KEY_INPUT_9))
	{
		a = -177.55f;
	}

	//RotateUpdate();

	SetCameraPositionAndTarget_UpVecY(aimPosition, spherePosition);
}

/// <summary>
/// �`��
/// </summary>
bool Camera::Draw()
{

	/*DrawSphere3D(spherePosition, radius, 30, GetColor(0, 0, 0),
			GetColor(255, 0, 0), FALSE);*/

	printfDx("lookPosition.x %f\n", lookPosition.x);
	printfDx("lookPosition.y %f\n", lookPosition.y);
	printfDx("lookPosition.z %f\n", lookPosition.z);
	printfDx("aimPosition.x %f\n", aimPosition.x);
	printfDx("aimPosition.y %f\n", aimPosition.y);
	printfDx("aimPosition.z %f\n", aimPosition.z);
	return true;
}

void Camera::RotateUpdate()
{
	float angle = a * DX_PI_F / 360.0f;
	this->angle = angle;

	aimPosition_usual.x = spherePosition.x + distance * cos(angle);
	aimPosition_usual.z = spherePosition.z + distance * sin(angle);

	aimPosition = aimPosition_usual;

	float maxRange = 5.0f;
	float maxRange_ = 10.0f;

	//����������W����player�����ꂽ��C������
	PosCalc();
}

/// <summary>
/// �J�����̈ʒu����
/// </summary>
/// <param name="mapHandle"></param>
void Camera::CameraPosCalc(const int& mapHandle)
{
	MV1_COLL_RESULT_POLY hitPoly;
	VECTOR addPos;

	//ray���������Ă���ꍇ�J�����̈ʒu��������
	if (HitCheck::HitRayJudge(mapHandle, -1, spherePosition, aimPosition, hitPoly))
	{
		VECTOR direction;

		addPos = VSub(hitPoly.HitPosition, aimPosition_usual);
		direction = VNorm(addPos);

		//addPos = VAdd(addPos,VScale(direction, 2.0f));

		aimPosition = VAdd(aimPosition_usual, addPos);
	}

}

/// <summary>
/// ���[�v
/// </summary>
/// <param name="AimPosition"></param>
void Camera::Leap(VECTOR& changePosition, const VECTOR& playerPosition, const float& speed)
{
	VECTOR SubPosition = VSub(playerPosition, changePosition);
	VECTOR scalePosition = VGet(0, 0, 0);

	scalePosition = VScale(SubPosition, speed);
	changePosition = VAdd(changePosition, scalePosition);
}



void Camera::PosCalc()
{
	//lookPos�����̊O���ɂ������ꍇ���̒��S���W�����炷
	if (!HitCheck::HitConfirmation(spherePosition, centerPos, radius, 0.5f))
	{
		Leap(spherePosition, centerPos, 0.1f);
	}
	else
	{
		Leap(spherePosition, centerPos, 0.05f);
	}

}