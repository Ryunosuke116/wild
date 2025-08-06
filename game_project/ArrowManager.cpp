#include "common.h"
#include <memory>
#include "ArrowManager.h"
#include "Arrow.h"
#include "Player.h"

/// <summary>
/// コンストラクタ
/// </summary>
ArrowManager::ArrowManager():
	isCreate_arrow(false)
{

}

/// <summary>
/// デストラクタ
/// </summary>
ArrowManager::~ArrowManager()
{

}

/// <summary>
/// 生成
/// </summary>
void ArrowManager::Create()
{

}

/// <summary>
/// 初期化
/// </summary>
void ArrowManager::Initialize()
{

}

/// <summary>
/// 更新
/// </summary>
void ArrowManager::Update(const std::shared_ptr<Player>& player)
{
	//playerが矢を放ったら追加
	if (isCreate_arrow)
	{
		Add(player->GetModelHandle(),
			player->GetFrameName(), player->GetAngle());
		isCreate_arrow = false;
	}

	for (auto arrow : arrows)
	{
		arrow->Update();
	}
}

/// <summary>
/// 描画
/// </summary>
void ArrowManager::Draw()
{
	for (auto arrow : arrows)
	{
		arrow->Draw();
	}
}

void ArrowManager::Add(const int& modelHandle_player,
	const std::string& frameName, const float& angle_player)
{
	arrows.push_back(std::make_shared<Arrow>());
	std::shared_ptr<Arrow> arrow_actual = std::dynamic_pointer_cast<Arrow>(arrows.back());

	arrow_actual->Initialize(modelHandle_player, frameName, angle_player);
}

void ArrowManager::Update(){}