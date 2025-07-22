#include "common.h"
#include <cassert>
#include "BaseScene.h"
#include "SceneManager.h"

SceneManager::SceneManager() :nowScene{ nullptr } {}

SceneManager::~SceneManager()
{
	for (const auto& pair : scenes)
	{
		delete pair.second;
	}
}


void SceneManager::ChangeScene(const std::string_view name)
{
	try
	{
		nowScene = scenes.at(name);
		nowScene->Initialize();
	}
	catch (...)
	{
		assert(!"���݂��Ȃ��L�[���w�肳��܂����B");
	}
}

void SceneManager::Initialize()
{
	nowScene->Initialize();
}

void SceneManager::Update()
{
	nowScene->Update();
}

void SceneManager::Draw()
{
	nowScene->Draw();
}

