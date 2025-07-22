#include "common.h"
#include "Field.h"

/// <summary>
/// コンストラクタ
/// </summary>
Field::Field(const char* path)
{
	modelHandle = MV1LoadModel(path);

	position = VGet(0, 0, 0);
	//モデルの大きさ調整
	MV1SetScale(modelHandle, VGet(0.9f, 0.9f, 0.9f));

	MV1SetPosition(modelHandle, position);


}

/// <summary>
/// デストラクタ
/// </summary>
Field::~Field()
{

}

/// <summary>
/// 初期化
/// </summary>
void Field::Initialize()
{
	// モデルの０番目のフレームのコリジョン情報を構築
	MV1SetupCollInfo(modelHandle, -1, 1, 1, 1);

	isPoly = false;

}

/// <summary>
/// 更新
/// </summary>
void Field::Update()
{

}

/// <summary>
/// 描画
/// </summary>
bool Field::Draw()
{
	if (isPoly)
	{
		MV1SetWireFrameDrawFlag(modelHandle, TRUE);
	}
	else
	{
		MV1SetWireFrameDrawFlag(modelHandle, FALSE);
	}

	MV1DrawModel(modelHandle);

	//MV1DrawModel(meshHandle);
	return true;
}