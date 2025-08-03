#pragma once
#include "BaseGameObjectManager.h"
#include "Player.h"
#include "BaseChara.h"

#include "CollisionManager.h"

class CharacterManager : public BaseGameObjectManager
{
private:

	std::vector<std::shared_ptr<BaseChara>> characters;
	std::shared_ptr<CollisionManager> collisionManager;
public:

	CharacterManager();
	~CharacterManager();

	void Update(const std::vector<std::shared_ptr<BaseObject>>& fieldObjects,
		const VECTOR& cameraDirection,const float& deltaTime);

	void Create()		override;
	void Initialize()	override;
	void Update()		override;
	bool Draw()			override;
	void Add()			override;


	VECTOR PositionCheck(const VECTOR& hangingPos, const VECTOR& playerPos);

	//std::shared_ptr<Player> GetPlayer()const { return actualPlayer; }

	std::shared_ptr<Player> GetPlayer() const{
		for (auto& chara : characters)
		{
			if (auto player = std::dynamic_pointer_cast<Player>(chara)) 
			{
				return player;
			}
		}
		return nullptr; 
	}
};

