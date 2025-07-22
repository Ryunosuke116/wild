#include "common.h"
#include "Player.h"

/// <summary>
/// コンストラクタ
/// </summary>
Player::Player()
{
	modelHandle = MV1LoadModel("material/mv1/player/arrow.mv1");
	position = VGet(0.0f, 10.0f, 0.0f);
	MV1SetScale(modelHandle, VGet(modelScale, modelScale, modelScale));
	MV1SetPosition(modelHandle, position);
	nowFrameNumber = 0;
}

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player()
{

}

/// <summary>
/// 初期化
/// </summary>
void Player::Initialize()
{

}

/// <summary>
/// 更新
/// </summary>
void Player::Update()
{
	if (CheckHitKey(KEY_INPUT_3))
	{
		nowFrameNumber++;
	}
	else if (CheckHitKey(KEY_INPUT_2))
	{
		nowFrameNumber--;
	}

	//矢先 5
	//持ち手（?）2
}

/// <summary>
/// 描画
/// </summary>
bool Player::Draw()
{
	MV1DrawModel(modelHandle);

	VECTOR nowFrame = MV1GetFramePosition(modelHandle, nowFrameNumber);

	DrawSphere3D(nowFrame, 2.0f, 30, GetColor(0, 0, 0),
		GetColor(255, 0, 255), FALSE);

	printfDx("frame現在数%d\n", nowFrameNumber);
	return false;
}

