#include "common.h"
#include <string>
#include "Arrow.h"

/// <summary>
/// コンストラクタ
/// </summary>
Arrow::Arrow()
{
	modelHandle = MV1LoadModel("material/mv1/player/arrow.mv1");
}

/// <summary>
/// デストラクタ
/// </summary>
Arrow::~Arrow()
{

}

/// <summary>
/// 初期化
/// </summary>
void Arrow::Initialize(const int& modelHandle_player,
	const std::string& frameName,const float& angle_player)
{
	int frameNum = MV1SearchFrame(modelHandle_player, frameName.c_str());

	position = MV1GetFramePosition(modelHandle_player, frameNum);
	MV1SetPosition(modelHandle, position);
	MV1SetScale(modelHandle, VGet(0.1f, 0.1f, 0.1f));

	MV1SetRotationXYZ(modelHandle, VGet(-90.0f * DX_PI_F / 180.0f, angle_player, 0.0f));
	angle = angle_player;
	
	MATRIX rotY = MGetRotY(angle);
	VECTOR forward = VGet(0.0f, 0.0f, 1.0f);

	direction = VTransform(forward, rotY);
	direction = VNorm(VScale(direction, -1.0f));
}

/// <summary>
/// 更新処理
/// </summary>
void Arrow::Update()
{
	position = VAdd(position, direction);
	MV1SetPosition(modelHandle, position);
}

/// <summary>
/// 描画
/// </summary>
/// <returns></returns>
bool Arrow::Draw()
{
	MV1DrawModel(modelHandle);

	return true;
}

void Arrow::Initialize() {};