#include "common.h"
#include "SceneManager.h"
#include "BaseScene.h"

/// <summary>
/// �C���X�^���X��
/// </summary>
BaseScene::BaseScene(SceneManager& manager) :manager{ manager } {}

/// <summary>
/// �f�X�g���N�^
/// </summary>
BaseScene::~BaseScene()
{

}

/// <summary>
/// ������
/// </summary>
void BaseScene::Initialize()
{

}

/// <summary>
/// �X�V
/// </summary>
void BaseScene::Update()
{

}

/// <summary>
/// �`��
/// </summary>
void BaseScene::Draw()
{

}

void BaseScene::ChangeScene(const std::string_view name)
{
    manager.ChangeScene(name);
}