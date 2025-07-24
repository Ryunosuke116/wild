#pragma once
#include "PlayerStateActionBase.h"
#include "BaseObject.h"
#include "BaseChara.h"
#include "CollisionData.h"
#include <vector>
#include <string>

class HitCheck;
class BaseChara;

class CollisionManager
{
public:


	//////////////////////////////////////////
	//�Փ˔���
	/////////////////////////////////////////
	std::pair<bool, std::string> GroundCollisionCheck(const std::vector<std::shared_ptr<BaseObject>>& fieldObjects,
		VECTOR& newPos, const VECTOR& moveVec, const PositionData& positionData);

	bool HeadCollisionCheck(const std::vector<std::shared_ptr<BaseObject>>& fieldObjects, VECTOR& newPos,
		const VECTOR& moveVec, const PositionData& positionData, const float& radius);

	std::pair<bool, VECTOR> WallCollisionCheck(const std::vector<std::shared_ptr<BaseObject>>& fieldObjects, 
		VECTOR& newPos, const VECTOR& moveVec, const PositionData& positionData,
		const float& radius);


	CollisionResult Check_all(const std::vector<std::shared_ptr<BaseObject>>& fieldObjects,
		const VECTOR& playerPos, const VECTOR& moveVec, const float& radius,
		const PositionData& positionData);

	void Update(std::vector<std::shared_ptr<BaseChara>>& characters,
		const std::vector<std::shared_ptr<BaseObject>>& fieldObjects);

	bool TestSphereTriangle(VECTOR centerPos, VECTOR a, VECTOR b, VECTOR c, VECTOR& q, const float radius);

	std::pair<bool, VECTOR> CliffGrabbing(const std::vector<std::shared_ptr<BaseObject>>& fieldObjects,
		const VECTOR& topPosition, const VECTOR& moveDirection, const bool isFalling);
	
	VECTOR CalcPushBack_SphereMeshOutsideTriangle(const MV1_COLL_RESULT_POLY& poly, const VECTOR& HitPos_ground, const VECTOR& bottomPos, const float& radius);

	std::pair<bool, VECTOR>GroundCollisionCheck_Hang_to_Crouch(const std::vector<std::shared_ptr<BaseObject>>& fieldObjects,
		const VECTOR& oldPos, const VECTOR& newPos, const VECTOR& foot, const PositionData& positionData);

	float GetTiltAngle_degree()const { return tiltAngle_degree; }
	MV1_COLL_RESULT_POLY GetHangingPoly() const { return HangingPoly; }

	bool Draw();
private:

	MV1_COLL_RESULT_POLY hitPoly_Ground;
	MV1_COLL_RESULT_POLY oldPoly;
	MV1_COLL_RESULT_POLY HangingPoly;

	MV1_COLL_RESULT_POLY_DIM hitPoly_Wall;
	MV1_COLL_RESULT_POLY_DIM hitPoly_Ground_sphere;

	VECTOR hitPos_head;
	VECTOR subPos;
	VECTOR normal;
	VECTOR hitPos_wall;
	VECTOR hitPos_ground;
	VECTOR playerCenterPos;
	VECTOR pos_now;
	VECTOR pos_new;
	VECTOR nearestPoint;
	VECTOR hitSphere;
	VECTOR topPos_ray;
	VECTOR bottomPos_ray;
	VECTOR hitHangingPos;

	VECTOR ray_start_hanging_log;
	VECTOR ray_end_hanging_log;
	float tiltAngle_degree;


	PositionData positionData;

	PositionData GetPositionData() { return positionData; }

};

