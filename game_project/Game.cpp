#include "common.h"
#include "GameObjectManager.h"
#include "SceneManager.h"
#include "BaseScene.h"
#include "Game.h"

/// <summary>
/// �C���X�^���X��
/// </summary>
/// <param name="manager"></param>
Game::Game(SceneManager& manager) :BaseScene{ manager }
{
	gameObjectManager = std::make_shared<GameObjectManager>();
	gameObjectManager->Create();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Game::~Game()
{

}

/// <summary>
/// ������
/// </summary>
void Game::Initialize()
{
	gameObjectManager->Initialize();
}

/// <summary>
/// �X�V
/// </summary>
void Game::Update()
{
	PadInput::Update();
	gameObjectManager->Update();
}

void Game::Draw()
{
	clsDx();
	gameObjectManager->Draw();
}