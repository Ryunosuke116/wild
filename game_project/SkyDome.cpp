#include "common.h"
#include "SkyDome.h"
#include "BaseObject.h"

/// <summary>
/// コンストラクタ
/// </summary>
SkyDome::SkyDome(const char* path)
{
	modelHandle = MV1LoadModel(path);
	MV1SetScale(modelHandle, VGet(2.0f, 2.0f, 2.0f));
}

/// <summary>
/// デストラクタ
/// </summary>
SkyDome::~SkyDome()
{

}

/// <summary>
/// 初期化
/// </summary>
void SkyDome::Initialize()
{

}

/// <summary>
/// 更新
/// </summary>
void SkyDome::Update(const VECTOR& playerPos)
{
	MV1SetPosition(modelHandle, playerPos);
}

void SkyDome::Update() {};

/// <summary>
/// 描画
/// </summary>
bool SkyDome::Draw()
{
	MV1DrawModel(modelHandle);
	return true;
}