#include "common.h"
#include <vector>
#include <memory>
#include "GameObjectManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
GameObjectManager::GameObjectManager()
{

}

/// <summary>
/// デストラクタ	
/// </summary>
GameObjectManager::~GameObjectManager()
{

}

/// <summary>
/// オブジェクト生成
/// </summary>
void GameObjectManager::Create()
{

	skyDome = std::make_shared<SkyDome>("material/skyDome/sunSet.mv1");
	field = std::make_shared<Field>("material/mv1/field.mv1");
	playerManager = std::make_shared<PlayerManager>();
	camera = std::make_shared<Camera>();

	skyDome_actual = std::dynamic_pointer_cast<SkyDome>(skyDome);
	playerManager_actual = std::dynamic_pointer_cast<PlayerManager>(playerManager);

	fieldObjects.push_back(std::make_shared<Field>("material/mv1/field.mv1"));

	playerManager->Create();

}

/// <summary>
/// 初期化
/// </summary>
void GameObjectManager::Initialize()
{

	for (auto& feildObject : fieldObjects)
	{
		feildObject->Initialize();
	}

	skyDome->Initialize();
	field->Initialize();
	playerManager->Initialize();
	camera->Initialize();
	PadInput::Initialize();

	isCamera = false;
	isPush = false;

}

/// <summary>
/// 更新
/// </summary>
void GameObjectManager::Update()
{
	if (CheckHitKey(KEY_INPUT_0))
	{
		if (!isPush)
		{
			if (!isCamera)
			{
				isCamera = true;
			}
			else
			{
				isCamera = false;
			}

			isPush = true;
		}
	}
	else
	{
		isPush = false;
	}

	if (!isCamera)
	{
		camera->Update(playerManager_actual->GetPlayer()->GetPosition(), fieldObjects);

		playerManager_actual->Update(fieldObjects, camera->GetCameraDirection());
		skyDome_actual->Update(playerManager_actual->GetPlayer()->GetPosition());
		field->Update();
		for (auto& feildObject : fieldObjects)
		{
			feildObject->Update();
		}
	}
	else
	{
		camera->Update_layout();
	}

}

/// <summary>
/// 描画
/// </summary>
bool GameObjectManager::Draw()
{
	playerManager->Draw();
	camera->Draw();
	skyDome->Draw();
	field->Draw();
	for (auto& feildObject : fieldObjects)
	{
		feildObject->Draw();
	}

	DrawLine3D(VGet(0.0f, 15.0f, 0.0f), VGet(10.0f, 15.0f, 0.0f), GetColor(255, 0, 0));
	DrawLine3D(VGet(0.0f, 15.0f, 0.0f), VGet(0.0f, 25.0f, 0.0f), GetColor(0, 255, 0));
	DrawLine3D(VGet(0.0f, 15.0f, 0.0f), VGet(0.0f, 15.0f, 10.0f), GetColor(0, 0, 255));
	return true;
}
