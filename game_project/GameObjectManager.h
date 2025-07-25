#pragma once
#include "BaseManager.h"
#include "BaseChara.h"
#include "BaseObject.h"
#include "objects.h"
#include "BaseGameObjectManager.h"

class GameObjectManager : public BaseManager
{
public:
	GameObjectManager();
	~GameObjectManager();

	void Create()override;
	void Initialize()override;
	void Update(const float& gameTimer)override;
	bool Draw()override;

private:

	std::vector<std::shared_ptr<BaseGameObjectManager>>	objectManager_ilst;

	std::vector<std::shared_ptr<BaseObject>> fieldObjects;

	std::shared_ptr<BaseObject>				skyDome = NULL;
	std::shared_ptr<BaseObject>				field = NULL;
	std::shared_ptr<Camera>					camera = NULL;
	std::shared_ptr<BaseGameObjectManager>	characterManager = NULL;

	std::shared_ptr<CharacterManager>	characterManager_actual = NULL;
	std::shared_ptr<SkyDome>		skyDome_actual		 = NULL;

	bool isCamera;
	bool isPush;
};

