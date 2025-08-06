#pragma once
#include "BaseGameObjectManager.h"
#include "BaseObject.h"
#include "ArrowObserver.h"
#include "CollisionManager.h"

class Player;

class ArrowManager : public BaseGameObjectManager,public ArrowObserver
{
public:

	ArrowManager();
	~ArrowManager();

	void Update(const std::shared_ptr<Player>& player);
	void Add(const int& modelHandle_player,
		const std::string& frameName, const float& angle_player);

	void Create()		override;
	void Initialize()	override;
	void Update()		override;
	void Draw()			override;
	void Add()			override {}
	void OnRecoilArrow(bool flag)override { isCreate_arrow = flag; }

private:
	std::vector<std::shared_ptr<BaseObject>> arrows;
	std::shared_ptr<CollisionManager> collisionManager;
	bool isCreate_arrow;
};

