#include "common.h"
#include "Player.h"
#include "BaseObject.h"
#include "PlayerManager.h"
#include "Calculation.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayerManager::PlayerManager()
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerManager::~PlayerManager()
{

}

void PlayerManager::Create()
{
	player = std::make_shared<Player>();
	actualPlayer = std::dynamic_pointer_cast<Player>(player);
}

/// <summary>
/// ������
/// </summary>
void PlayerManager::Initialize()
{
	player->Initialize();
}

/// <summary>
/// �X�V
/// </summary>
/// <param name="mapHandle"></param>
/// <param name="player"></param>
void PlayerManager::Update(const std::vector<std::shared_ptr<BaseObject>>& fieldObjects,
	const VECTOR& cameraDirection)
{
	actualPlayer->Update(cameraDirection, fieldObjects);

	/*if(actualPlayer->GetData().isHanging)
	{
		actualPlayer->SetPos(PositionCheck(actualPlayer->GetHangingPoint(), actualPlayer->GetPosition()));
	}*/

	// �v���C���[�̃��f���̍��W���X�V����
	//MV1SetPosition(actualPlayer->GetModelHandle(), actualPlayer->GetPosition());
}

/// <summary>
/// �`��
/// </summary>
bool PlayerManager::Draw()
{
	player->Draw();
	return true;
}

VECTOR PlayerManager::PositionCheck(const VECTOR& hangingPos, const VECTOR& playerPos)
{
	VECTOR newPlayerPos = playerPos;
	VECTOR hanging = hangingPos;
	hanging.y = playerPos.y;

	Calculation::Leap(newPlayerPos, hanging, 0.1f);

	return newPlayerPos;
}

void PlayerManager::Update() {}
void PlayerManager::Add() {}