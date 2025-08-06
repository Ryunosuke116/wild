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
	characterManager	= std::make_shared<CharacterManager>();
	arrowManager		= std::make_shared<ArrowManager>();
	camera				= std::make_shared<Camera>();

	skyDome_actual			= std::dynamic_pointer_cast<SkyDome>(skyDome);
	characterManager_actual = std::dynamic_pointer_cast<CharacterManager>(characterManager);
	arrowManager_actual		= std::dynamic_pointer_cast<ArrowManager>(arrowManager);
	
	fieldObjects.push_back(std::make_shared<Field>("material/mv1/field.mv1"));

	characterManager->Create();
	characterManager_actual->AddObserver_player(arrowManager_actual);

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
	characterManager->Initialize();
	camera->Initialize(characterManager_actual->GetPlayer()->GetPosition());
	PadInput::Initialize();

	isCamera = false;
	isPush = false;

}

/// <summary>
/// 更新
/// </summary>
void GameObjectManager::Update(const float& gameTimer)
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
		camera->Update(characterManager_actual->GetPlayer()->GetPosition(),
			characterManager_actual->GetPlayer()->GetData(),
			fieldObjects);

		characterManager_actual->Update(fieldObjects, camera->GetCameraDirection(), gameTimer);
		skyDome_actual->Update(characterManager_actual->GetPlayer()->GetPosition());
		arrowManager_actual->Update(characterManager_actual->GetPlayer());
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
	characterManager->Draw();
	arrowManager->Draw();
	camera->Draw();
	skyDome->Draw();
	field->Draw();
	for (auto& fieldObject : fieldObjects)
	{
		fieldObject->Draw();
	}

	DrawLine3D(VGet(0.0f, 15.0f, 0.0f), VGet(10.0f, 15.0f, 0.0f), GetColor(255, 0, 0));
	DrawLine3D(VGet(0.0f, 15.0f, 0.0f), VGet(0.0f, 25.0f, 0.0f), GetColor(0, 255, 0));
	DrawLine3D(VGet(0.0f, 15.0f, 0.0f), VGet(0.0f, 15.0f, 10.0f), GetColor(0, 0, 255));
	return true;
}
