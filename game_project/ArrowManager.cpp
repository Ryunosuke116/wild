#include "common.h"
#include <memory>
#include "ArrowManager.h"
#include "Arrow.h"
#include "Player.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
ArrowManager::ArrowManager():
	isCreate_arrow(false)
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
ArrowManager::~ArrowManager()
{

}

/// <summary>
/// ����
/// </summary>
void ArrowManager::Create()
{

}

/// <summary>
/// ������
/// </summary>
void ArrowManager::Initialize()
{

}

/// <summary>
/// �X�V
/// </summary>
void ArrowManager::Update(const std::shared_ptr<Player>& player)
{
	//player������������ǉ�
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
/// �`��
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