#include "common.h"
#include "EffekseerForDXLib.h"
#include "HitCheck.h"
#include "PadInput.h"
#include "BaseObject.h"
#include "Camera.h"
#include "Calculation.h"
#include "DebugDrawer.h"



/// <summary>
/// インストラクタ
/// </summary>
Camera::Camera() :
	distance(0.0f),
	aimPosition_usual(VGet(0.0f, 0.0f, 0.0f))
{
	// グラフィックの描画先を裏画面にセット
	SetDrawScreen(DX_SCREEN_BACK);

	//奥行0.1～1000までをカメラの描画範囲とする
	SetCameraNearFar(3.5f, 5000.0f);


}

/// <summary>
/// デストラクタ
/// </summary>
Camera::~Camera()
{

}

/// <summary>
/// 初期化
/// </summary>
void Camera::Initialize(const VECTOR& playerPosition)
{
	a = -177.55f;
	distance = 50.0f;
	
	//視点とカメラの間の座標
	centerPos = playerPosition;
	centerPos.y += 14.0f;

	float angle = a * DX_PI_F / 360.0f;
	this->angle = angle;

	//カメラの座標
	cameraPos.x = centerPos.x + distance * cos(angle);
	cameraPos.z = centerPos.z + distance * sin(angle);

	//視点の初期化
	targetPosCalc();
	//最初は座標を平行にする
	cameraPos.y = targetPos.y;

	//カメラの向いている方向
	cameraDirection = VSub(targetPos, cameraPos);
	cameraDirection = VNorm(cameraDirection);

	/////////////////////////////////////////
	//	aim用の座標
	/////////////////////////////////////////
	float radian = -35.0f * DX_PI_F / 180.0f;
	//視点とカメラの間の座標
	centerPos_aim = centerPos;
	centerPos_aim.y += 10.0f;
	centerPos_aim = Calculation::Rodrigues(centerPos, centerPos_aim, cameraDirection, radian);

	//カメラの座標
	cameraPos_aim = cameraPos;
	cameraPos_aim.y += 5.0f;
	cameraPos_aim = Calculation::Rodrigues(cameraPos, cameraPos_aim, cameraDirection, radian);
	
	//視点座標
	targetPos_aim = targetPos;
	targetPos_aim.y += 10.0f;
	targetPos_aim = Calculation::Rodrigues(targetPos,
		targetPos_aim, cameraDirection, radian);

	distance_aim = 15.0f;
	t = 0.0f;


	isCalc = false;

	SetCameraPositionAndTarget_UpVecY(cameraPos, targetPos);

	// DXライブラリのカメラとEffekseerのカメラを同期する。
	Effekseer_Sync3DSetting();
}

/// <summary>
/// 更新
/// </summary>
void Camera::Update(const VECTOR& playerPosition,
	const PlayerData& playerData,
	const std::vector<std::shared_ptr<BaseObject>>& fieldObjects)
{
	VECTOR norm = VNorm(VSub(centerPos, cameraPos));
	norm.y = 0.0f;
	
	centerPos = playerPosition;
	centerPos.y += 14.0f;
	centerPos_aim = centerPos;
	centerPos_aim.y += 5.0f;

	float radian = -35.0f * DX_PI_F / 180.0f;
	centerPos_aim = Calculation::Rodrigues(centerPos, centerPos_aim, cameraDirection, radian);

	if (!playerData.isAim)
	{
		//recoilPos = VAdd(centerPos, VScale(norm, -distance_aim));
		//recoilPos.y = cameraPos.y + 10.0f;
	}

	aimPosition_usual.y = targetPos.y + 15.0f;

	//カメラ移動処理
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
	if (CheckHitKey(KEY_INPUT_1) ||
		PadInput::GetJoyPad_y_right() > 0.0f)
	{
		cameraPos.y += 0.5f;
		if (playerData.isAim)
		{
			cameraPos_aim.y += 0.5f;
			t -= 0.01f;
			if (t <= 0.0f)
			{
				t = 0.0f;
			}
		}
	}
	if (CheckHitKey(KEY_INPUT_2) ||
		PadInput::GetJoyPad_y_right() < 0.0f)
	{
		cameraPos.y -= 0.5f;
		if (playerData.isAim)
		{
			cameraPos_aim.y -= 0.5f;
			t += 0.01f;
			if (t >= 1.0f)
			{
				t = 1.0f;
			}
		}
	}

	RotateUpdate(playerData);

	//カメラが地形に埋まらないようにする
	for (const auto& fieldObject : fieldObjects)
	{
		CameraPosCalc(fieldObject->GetModelHandle());
	}
	
	if (playerData.isAim)
	{
		SetCameraPositionAndTarget_UpVecY(cameraPos_aim, targetPos_aim);
	}
	else
	{
		SetCameraPositionAndTarget_UpVecY(cameraPos, targetPos);
	}

	cameraDirection = VSub(targetPos, cameraPos);
	cameraDirection = VNorm(cameraDirection);
	DebugDrawer::Instance().InformationInput_sphere(targetPos, 2.0f, GetColor(255, 0, 255));
	DebugDrawer::Instance().InformationInput_sphere(targetPos_aim, 2.0f, GetColor(255, 0, 255));
	DebugDrawer::Instance().InformationInput_sphere(centerPos_aim, 2.0f, GetColor(0, 0, 255));
	DebugDrawer::Instance().InformationInput_sphere(cameraPos_aim, 2.0f, GetColor(255, 0, 0));
}

/// <summary>
/// layout用カメラ
/// </summary>
void Camera::Update_layout()
{
	//y軸↑移動
	if (CheckHitKey(KEY_INPUT_UP) && CheckHitKey(KEY_INPUT_LCONTROL))
	{
		cameraPos.y += 1.0f;
		targetPos.y += 1.0f;
	}
	//z軸↑移動
	else if (CheckHitKey(KEY_INPUT_UP))
	{
		cameraPos.z += 1.0f;
		targetPos.z += 1.0f;
	}

	//y軸↓移動
	if (CheckHitKey(KEY_INPUT_DOWN) && CheckHitKey(KEY_INPUT_LCONTROL))
	{
		cameraPos.y -= 1.0f;
		targetPos.y -= 1.0f;
	}
	//z軸↓移動
	else if (CheckHitKey(KEY_INPUT_DOWN))
	{
		cameraPos.z -= 1.0f;
		targetPos.z -= 1.0f;
	}

	//右回転
	if (CheckHitKey(KEY_INPUT_RIGHT) &&
		CheckHitKey(KEY_INPUT_LSHIFT))
	{
		a += 2.0f;
	}
	//x軸右移動
	else if (CheckHitKey(KEY_INPUT_RIGHT) ||
		PadInput::GetJoyPad_x_right() < 0.0f)
	{
		cameraPos.x += 1.0f;
		targetPos.x += 1.0f;
	}

	//左回転
	if (CheckHitKey(KEY_INPUT_LEFT) &&
		CheckHitKey(KEY_INPUT_LSHIFT))
	{
		a -= 2.0f;
	}
	//x軸左移動
	else if (CheckHitKey(KEY_INPUT_LEFT) ||
		PadInput::GetJoyPad_x_right() > 0.0f)
	{
		cameraPos.x -= 1.0f;
		targetPos.x -= 1.0f;
	}

	if (CheckHitKey(KEY_INPUT_9))
	{
		a = -177.55f;
	}

	//RotateUpdate();

	SetCameraPositionAndTarget_UpVecY(cameraPos, targetPos);
}

void Camera::Update_aim()
{
	/*targetPos.x = cameraPos.x + distance * cos(angle);
	targetPos.z = cameraPos.z + distance * sin(angle);*/
}

/// <summary>
/// 描画
/// </summary>
bool Camera::Draw()
{

	/*DrawSphere3D(targetPos, radius, 30, GetColor(0, 0, 0),
			GetColor(255, 0, 0), FALSE);*/

	printfDx("lookPosition.x %f\n", targetPos.x);
	printfDx("lookPosition.y %f\n", targetPos.y);
	printfDx("lookPosition.z %f\n", targetPos.z);
	printfDx("cameraPos.x %f\n", cameraPos.x);
	printfDx("cameraPos.y %f\n", cameraPos.y);
	printfDx("cameraPos.z %f\n", cameraPos.z);
	return true;
}

void Camera::RotateUpdate(const PlayerData& playerData)
{
	float angle = a * DX_PI_F / 360.0f;
	this->angle = angle;
	VECTOR oldCameraPos = cameraPos;

	cameraPos.x = centerPos.x + distance * cos(angle);
	cameraPos.z = centerPos.z + distance * sin(angle);

	float radian = -35.0f * DX_PI_F / 180.0f;
	cameraPos_aim = cameraPos;
	cameraPos_aim.y += 2.0f;
	cameraPos_aim = Calculation::Rodrigues(cameraPos, cameraPos_aim, cameraDirection, radian);
	isCalc = true;

	if (playerData.isAim)
	{
		float min_distance = 10.0f;
		float max_distance = 20.0f;
		float min_Y = centerPos.y - 10.0f;
		float max_Y = centerPos.y + 20.0f;

		float easedT = Calculation::EaseOutQuad(t);

		cameraPos_aim.y = min_Y + (max_Y - min_Y) * easedT;
		distance_aim = min_distance + (max_distance - min_distance) * easedT;

		cameraPos_aim.x = centerPos_aim.x + distance_aim * cos(angle);
		cameraPos_aim.z = centerPos_aim.z + distance_aim * sin(angle);
	}
	else
	{
		isCalc = false;
	}

	//注視する座標からplayerがずれたら修正する
	targetPosCalc();
}

/// <summary>
/// カメラの位置調整
/// </summary>
/// <param name="mapHandle"></param>
void Camera::CameraPosCalc(const int& mapHandle)
{
	MV1_COLL_RESULT_POLY hitPoly;
	VECTOR addPos;

	//rayが当たっている場合カメラの位置をいじる
	if (HitCheck::HitRayJudge(mapHandle, -1, centerPos, cameraPos, hitPoly))
	{
		VECTOR direction;

		addPos = VSub(hitPoly.HitPosition, cameraPos);
		direction = VNorm(addPos);

		//addPos = VAdd(addPos,VScale(direction, 2.0f));

		cameraPos = VAdd(cameraPos, addPos);
	}

	//rayが当たっている場合カメラの位置をいじる
	if (HitCheck::HitRayJudge(mapHandle, -1, centerPos, targetPos, hitPoly))
	{
		VECTOR direction;

		addPos = VSub(hitPoly.HitPosition, targetPos);

		//addPos = VAdd(addPos,VScale(direction, 2.0f));

		targetPos = VAdd(targetPos, addPos);
	}

	//targetPos_aim = VAdd(targetPos_aim, VSub(targetPos, oldTargetPos));
}

/// <summary>
/// ラープ
/// </summary>
/// <param name="AimPosition"></param>
void Camera::Leap(VECTOR& changePosition, const VECTOR& playerPosition, const float& speed)
{
	VECTOR SubPosition = VSub(playerPosition, changePosition);
	VECTOR scalePosition = VGet(0, 0, 0);

	scalePosition = VScale(SubPosition, speed);
	changePosition = VAdd(changePosition, scalePosition);
}



void Camera::targetPosCalc()
{
	//通常時の視点座標
	VECTOR dir = VSub(centerPos, cameraPos);
	dir = VNorm(dir);
	VECTOR scale = VScale(dir, 40.0f);
	targetPos = VAdd(centerPos, scale);

	//エイム時の視点座標
	VECTOR dir_aim = VSub(centerPos_aim, cameraPos_aim);
	dir_aim = VNorm(dir_aim);
	VECTOR scale_aim = VScale(dir_aim, 50.0f);
	targetPos_aim = VAdd(centerPos_aim, scale_aim);

}