#include "common.h"
#include "GameObjectManager.h"
#include "SceneManager.h"
#include "BaseScene.h"
#include "GameTimer.h"
#include "Game.h"
#include "DebugDrawer.h"

/// <summary>
/// �C���X�^���X��
/// </summary>
/// <param name="manager"></param>
Game::Game(SceneManager& manager) :BaseScene{ manager }
{
	gameObjectManager = std::make_shared<GameObjectManager>();
	gameTimer		  = std::make_shared<GameTimer>();
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
	gameTimer->Update();
	PadInput::Update();
	gameObjectManager->Update(gameTimer->GetDeltaTime());

}

void Game::Draw()
{
	clsDx();
	gameObjectManager->Draw();
	DebugDrawer::Instance().Draw();
}