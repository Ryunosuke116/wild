#include "common.h"
#include <vector>
#include "Player.h"
#include "BaseObject.h"
#include "CharacterManager.h"
#include "Calculation.h"


/// <summary>
/// コンストラクタ
/// </summary>
CharacterManager::CharacterManager()
{
}

/// <summary>
/// デストラクタ
/// </summary>
CharacterManager::~CharacterManager()
{

}

void CharacterManager::Create()
{
	collisionManager = std::make_shared<CollisionManager>();
	player = std::make_shared<Player>();
	actualPlayer = std::dynamic_pointer_cast<Player>(player);
	
	characters.push_back(std::make_shared<Player>());
}

/// <summary>
/// 初期化
/// </summary>
void CharacterManager::Initialize()
{
	for (auto& chara : characters)
	{
		chara->Initialize();
	}
}

/// <summary>
/// 更新
/// </summary>
/// <param name="mapHandle"></param>
/// <param name="player"></param>
void CharacterManager::Update(const std::vector<std::shared_ptr<BaseObject>>& fieldObjects,
	const VECTOR& cameraDirection, const float& deltaTime)
{
	for (auto& chara : characters)
	{
		if (auto player = std::dynamic_pointer_cast<Player>(chara))
		{
			player->Update(deltaTime,cameraDirection, fieldObjects);
		}
		else
		{
			chara->Update();
		}
	}

	collisionManager->Update(characters, fieldObjects);

	for (auto& chara : characters)
	{
		if (auto player = std::dynamic_pointer_cast<Player>(chara))
		{
			player->Receive_CollisionResult();
		}
	}

	for (auto& chara : characters)
	{
		chara->PositionUpdate();
	}

}

/// <summary>
/// 描画
/// </summary>
bool CharacterManager::Draw()
{
	for (auto& chara : characters)
	{
		chara->Draw();
		chara->Draw_log();
	}
	collisionManager->Draw();

	return true;
}

VECTOR CharacterManager::PositionCheck(const VECTOR& hangingPos, const VECTOR& playerPos)
{
	VECTOR newPlayerPos = playerPos;
	VECTOR hanging = hangingPos;
	hanging.y = playerPos.y;

	Calculation::Leap(newPlayerPos, hanging, 0.1f);

	return newPlayerPos;
}

void CharacterManager::Update() {}
void CharacterManager::Add() {}