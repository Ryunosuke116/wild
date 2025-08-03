#include "common.h"
#include <vector>
#include <utility>
#include "HitCheck.h"
#include "Calculation.h"
#include "common.h"
#include "PlayerStateActionBase.h"
#include "CollisionManager.h"
#include "PlayerData.h"


void CollisionManager::Update(std::vector<std::shared_ptr<BaseChara>>& characters,
	const std::vector<std::shared_ptr<BaseObject>>& fieldObjects)
{
	for (auto& chara : characters)
	{
		chara->SetCollision_result(Check_all(fieldObjects,
			chara->GetPosition(), chara->GetMoveVec(), chara->GetRadius(),
			chara->GetPositionData()));
	}
}

/// <summary>
/// �S�Փ˔���`�F�b�N
/// </summary>
/// <param name="fieldObjects"></param>
/// <param name="playerPos"></param>
/// <param name="moveVec"></param>
/// <param name="radius"></param>
/// <param name="positionData"></param>
/// <returns></returns>
CollisionResult CollisionManager::Check_all (const std::vector<std::shared_ptr<BaseObject>>& fieldObjects,
	const VECTOR& playerPos, const VECTOR& moveVec,const float& radius,
	const PositionData& positionData)
{
	VECTOR oldPos = playerPos;
	VECTOR newPos = VAdd(oldPos, moveVec);

	CollisionResult result;

	//�ǏՓ˔���
	auto hitWall = WallCollisionCheck(fieldObjects, newPos, moveVec, positionData, radius);

	//����Փ˔���
	HeadCollisionCheck(fieldObjects, newPos, moveVec, positionData, radius);

	//���Փ˔���
	auto returnGround = GroundCollisionCheck(fieldObjects, newPos, moveVec, positionData);

	//���Փ˔���
	return { 
		newPos,
		returnGround.first,
		hitWall.first,
		hitWall.second,
		returnGround.second
	};
}

/// <summary>
/// ����̓����蔻��
/// </summary>
/// <param name="modelHandle"></param>
/// <param name="newPos"></param>
/// <param name="positionData.addTopPos"></param>
/// <param name="positionData.radius"></param>
/// <param name="positionData.addBottomPos"></param>
/// <returns></returns>
bool CollisionManager::HeadCollisionCheck(const std::vector<std::shared_ptr<BaseObject>>& fieldObjects,
	VECTOR& newPos, const VECTOR& moveVec, const PositionData& positionData,const float& radius)
{
	MV1_COLL_RESULT_POLY_DIM hitPoly_head;
	VECTOR position_top_new = VAdd(positionData.position_top, moveVec);
	VECTOR position_bottom_new = VAdd(positionData.position_bottom, moveVec);

	//fieldObject�̗v�f���m�F
	for (auto& fieldObject : fieldObjects)
	{
		bool isHitHead = HitCheck::SphereHitJudge(fieldObject->GetModelHandle(), -1, position_top_new, hitPoly_head);

		if (isHitHead)
		{
			VECTOR addPos = VGet(0.0f, 0.0f, 0.0f);

			for (int i = 0; i < hitPoly_head.HitNum; i++)
			{
				MV1_COLL_RESULT_POLY poly = hitPoly_head.Dim[i];
				VECTOR newAddPos = VGet(0.0f, 0.0f, 0.0f);

				////////////////////////////////////////
				// todo::
				// �@���ł͂Ȃ��p�x�łł���悤��
				/////////////////////////////////////////
				if (poly.Normal.y <= -0.7f || poly.Normal.y >= 0.7f)
				{
					hitPos_head = HitCheck::ClosestPtToPointTriangle(position_top_new, poly.Position[0], poly.Position[1], poly.Position[2]);

					VECTOR hitDirection = VSub(hitPos_head, position_top_new);
					hitDirection = VNorm(hitDirection);
					hitDirection = VScale(hitDirection, radius);

					VECTOR hitPos_sphere = VAdd(position_top_new, hitDirection);


					newAddPos.y = hitPos_head.y - hitPos_sphere.y;
				}

				if (addPos.y > newAddPos.y)
				{
					addPos = newAddPos;
				}
			}

			newPos.y = newPos.y + addPos.y;
		}
	}


	return false;
}

/// <summary>
/// ���Ƃ̏Փ˔��菈��
/// </summary>
/// <param name="modelHandle"></param>
/// <param name="oldPos"></param>
/// <param name="newPos"></param>
/// <param name="positionData.addTopPos"></param>
/// <param name="positionData.addBottomPos"></param>
/// <param name="positionData.radius"></param>
/// <param name="isJump"></param>
/// <returns></returns>
std::pair<bool, std::string> CollisionManager::GroundCollisionCheck(const std::vector<std::shared_ptr<BaseObject>>& fieldObjects,
	VECTOR& newPos, const VECTOR& moveVec, const PositionData& positionData)
{
	bool isHitGround = false;
	bool returnFlag = false;
	std::string returnTag = "";

	VECTOR position_top_new = VAdd(positionData.position_top, moveVec);
	VECTOR position_bottom_new = VAdd(positionData.position_bottom, moveVec);

	topPos_ray = position_top_new;
	bottomPos_ray = position_bottom_new;

	MV1_COLL_RESULT_POLY rayPoly_ground;

	for (const auto& fieldObject : fieldObjects)
	{
		//ray���������Ă����
		isHitGround = HitCheck::HitRayJudge(fieldObject->GetModelHandle(), -1, position_top_new,
			position_bottom_new, rayPoly_ground);

		if (isHitGround)
		{
			VECTOR playerNormal = VSub(position_top_new,
				position_bottom_new);

			playerNormal = VNorm(playerNormal);

			//player�Ə��̂Ȃ��p�����߂�
			float cosTheta = VDot(playerNormal, rayPoly_ground.Normal) / (VSquareSize(playerNormal) * VSquareSize(rayPoly_ground.Normal));
			float radian = std::acos(cosTheta);
			tiltAngle_degree = radian * 180.0f / DX_PI_F;

			VECTOR newPlayerPos = VGet(0.0f, 0.0f, 0.0f);

			//�� - �v���C���[�̑����ŉ����߂��ʂ��v�Z
			newPlayerPos.y = rayPoly_ground.HitPosition.y - newPos.y;
			newPos.y = newPos.y + newPlayerPos.y;
		}

		if (isHitGround)
		{
			returnFlag = true;
			returnTag = fieldObject->GetTag();
		}
	}

	//�ڒn���Ă��邩
	return std::make_pair(returnFlag, returnTag);

}

/// <summary>
/// ���Ƃ̏Փ˔��菈��
/// </summary>
/// <param name="modelHandle"></param>
/// <param name="oldPos"></param>
/// <param name="newPos"></param>
/// <param name="positionData.addTopPos"></param>
/// <param name="positionData.addBottomPos"></param>
/// <param name="positionData.radius"></param>
/// <param name="isJump"></param>
/// <returns></returns>
std::pair<bool, VECTOR> CollisionManager::GroundCollisionCheck_Hang_to_Crouch(const std::vector<std::shared_ptr<BaseObject>>& fieldObjects,
	const VECTOR& oldPos, const VECTOR& newPos, const VECTOR& foot,
	const PositionData& positionData)
{

	//prev��new��pos�����
	VECTOR newTopPosition = newPos;
	VECTOR newBottomPosition = newPos;
	VECTOR nowBottomPos = oldPos;
	VECTOR nowTopPos = oldPos;
	bool isHitSphere;

	bool isHitGround = false;

	MV1_COLL_RESULT_POLY rayPoly_ground;
	VECTOR returnPos = newPos;

	for (const auto& fieldObject : fieldObjects)
	{
		//ray���������Ă����
		isHitGround = HitCheck::HitRayJudge(fieldObject->GetModelHandle(), -1, nowTopPos, nowBottomPos, rayPoly_ground);

		if (isHitGround)
		{
			VECTOR newPlayerPos = VGet(0.0f, 0.0f, 0.0f);
			//VECTOR footPos = VGet(0.0f, newBottomPosition.y - positionData.radius, 0.0f);

			//�� - �v���C���[�̑����ŉ����߂��ʂ��v�Z
			newPlayerPos.y = rayPoly_ground.HitPosition.y - foot.y;
			returnPos.y = returnPos.y + newPlayerPos.y;
		}
	}

	//�ڒn���Ă��邩
	return std::make_pair(isHitGround, returnPos);

}

/// <summary>
/// �ǂƂ̓����蔻��
/// </summary>
/// <param name="player"></param>
/// <param name="modelHandle"></param>
/// <returns></returns>
std::pair<bool, VECTOR> CollisionManager::WallCollisionCheck(const std::vector<std::shared_ptr<BaseObject>>& fieldObjects, 
	VECTOR& newPos, const VECTOR& moveVec, const PositionData& positionData,
	const float& radius)
{
	VECTOR position_top_new = VAdd(positionData.position_top, moveVec);
	VECTOR position_bottom_new = VAdd(positionData.position_bottom, moveVec);
	bool flag = false;
	VECTOR hitPoly_normal;

	for (auto& fieldObject : fieldObjects)
	{
		//�ǂƏՓ˂��Ă��邩
		HitCheck::CapsuleHitWallJudge(fieldObject->GetModelHandle(), -1, radius, position_top_new,
			VAdd(position_bottom_new, VGet(0.0f, 1.0f, 0.0f)), hitPoly_Wall);

		//�Փ˂��Ă���Ƃ���S�����ׂĉ����߂��ʂ��v�Z����
		if (hitPoly_Wall.HitNum >= 1)
		{
			float maxY = -FLT_MAX;
			int groundIndex = -1;
			VECTOR addPos = VGet(0.0f, 0.0f, 0.0f);

			//�q�b�g�����S�|���S���𒲂ׂ�
			for (int i = 0; i < hitPoly_Wall.HitNum; i++)
			{
				MV1_COLL_RESULT_POLY poly = hitPoly_Wall.Dim[i];

				float degree_x = Calculation::radToDeg(poly.Normal.x);
				float degree_z = Calculation::radToDeg(poly.Normal.z);


				//�ǂ��ǂ����𒲂ׂ�
				if ((poly.Normal.x >= 0.7f || poly.Normal.z >= 0.7f ||
					poly.Normal.x <= -0.7f || poly.Normal.z <= -0.7f) &&
					poly.Normal.y <= 0.7f)
				{
					normal = poly.Normal;
					normal.y = 0.0f;
					normal = VNorm(normal);

					//�����̂ǂ��ɓ���������
					auto result = HitCheck::SegmentTriangleDistance(position_top_new,
						position_bottom_new, poly.Position[0], poly.Position[1],
						poly.Position[2], poly.Normal);

					VECTOR addPos = VScale(poly.Normal, -3.5f);
					addPos.y = 0.0f;

					//���������Ƃ��납�猻�݂Ǝ��t���[���̍��W��raycast
					VECTOR pos_now = VAdd(result.first, addPos);
					MV1_COLL_RESULT_POLY poly_ray;

					HitCheck::HitRayJudge(fieldObject->GetModelHandle(), -1, pos_now,
						result.first, poly_ray);

					//ray���������Ă���ꍇ�����������W�ŏՓ˔���
					if (poly_ray.HitFlag)
					{
						//���̐ڐG���Ă�����W�����߂�
							// ��������ɂ́H��
							//�@�������Ƃ͋t�̕����ɃZ���^�[�|�W�V����������Z����
						VECTOR contact = VScale(poly.Normal, -3.5f);

						//�ʂ̐ڐG�_
						//hitPos_wall = poly_ray.HitPosition;
						hitPos_wall = result.second;

						//���̐ڐG���Ă�����W
						//�ʂɑ΂��Ĉ�ԋ߂������_����ʂɌ������ċ��̔��a�����Z����
						contact = VAdd(result.first, contact);

						//�ʂɏ��������͂�
						contact = VAdd(contact, VScale(poly.Normal, -0.1f));

						//���̐ڐG���W���ʂ̐ڐG���W�����߂�
						VECTOR pos = VSub(result.second, contact);
						pos.y = 0.0f;

						newPos = VAdd(newPos, pos);
						flag = true;
						hitPoly_normal = poly_ray.Normal;
					}


				}
			}
		}
	}

	// ���o�����v���C���[�̎��͂̃|���S�������J������
	MV1CollResultPolyDimTerminate(hitPoly_Wall);
	return std::make_pair(flag, hitPoly_normal);

}

/// <summary>
/// �R���ݔ���
/// </summary>
/// <param name="player"></param>
/// <param name="modelHandle"></param>
std::pair<bool, VECTOR> CollisionManager::CliffGrabbing(const std::vector<std::shared_ptr<BaseObject>>& fieldObjects,
	const VECTOR& topPosition, const VECTOR& moveDirection, const bool isFalling)
{
	VECTOR linePos_end = VAdd(topPosition, VScale(moveDirection, 6.0f));
	linePos_end.y = topPosition.y - 7.0f;

	//log�p
	ray_start_hanging_log = topPosition;
	ray_end_hanging_log = linePos_end;

	bool isHitHanging = false;
	bool returnFlag = false;

	//��������player�̏㕔����o�Ă���ray�Ŕ�������
	if (isFalling)
	{
		for (const auto& fieldObject : fieldObjects)
		{
			MV1_COLL_RESULT_POLY poly;
			isHitHanging = HitCheck::HitRayJudge(fieldObject->GetModelHandle(), -1, topPosition, linePos_end, poly);

			if (isHitHanging && poly.Normal.y >= 0.8f)
			{
				hitHangingPos = poly.HitPosition;
				returnFlag = isHitHanging;
				HangingPoly = poly;
			}
			else
			{
				isHitHanging = false;
			}
		}
	}

	return std::make_pair(returnFlag, hitHangingPos);

	//true�̏ꍇ�ɊR�����ނ悤�ɂ���
}


/// <summary>
/// ��s���O�p�`ABC�ƌ������Ă���ꍇ��true��Ԃ��A�����łȂ����false��Ԃ�
/// ���̒��S�ɑ΂���abc��̍ŋߐړ_�ł���_p���Ԃ�
/// </summary>
/// <returns></returns>
bool CollisionManager::TestSphereTriangle(VECTOR centerPos, VECTOR a, VECTOR b, VECTOR c, VECTOR& q, const float radius)
{
	//���̒��S�΂���ŋߐړ_�ł���O�p�`ABC��ɂ���_p��������
	q = HitCheck::ClosestPtToPointTriangle(centerPos, a, b, c);

	//���ƎO�p�`����������̂́A���̒��S����_q�܂ł�(��������)������(��������)���̔��a�����������ꍇ
	VECTOR v = VSub(q, centerPos);

	return VDot(v, v) <= radius * radius;
}


VECTOR CollisionManager::CalcPushBack_SphereMeshOutsideTriangle(const MV1_COLL_RESULT_POLY& poly, const VECTOR& HitPos_ground, const VECTOR& bottomPos, const float& radius)
{

	nearestPoint = Calculation::SphereMeshOutsideTriangle(poly, HitPos_ground);

	//���Ɩʂ̐ڐG�_�����߂�
	//�����v�Z
	hitSphere = VSub(nearestPoint, bottomPos);
	hitSphere = VNorm(hitSphere);

	//�ڐG�_�̕����ɔ��a�����Z
	hitSphere = VScale(hitSphere, radius);

	//���̒��S�_����ڐG�_�̕����ɔ��a���̃x�N�g�������Z���ċ��̐ڐG�_���v�Z
	hitSphere = VAdd(bottomPos, hitSphere);

	//�ʂ̐ڐG���W�Ƌ��̐ڐG�_�ŉ����߂��ʂ��v�Z
	VECTOR addPos = VGet(0.0f, 0.0f, 0.0f);
	addPos.y = nearestPoint.y - hitSphere.y;
	return addPos;
}

/// <summary>
/// �`��
/// </summary>
bool CollisionManager::Draw()
{
	//printfDx("NormalPos.y %f\n", subPos.y);
	//printfDx("NormalPos_Wall.x %f\n", normal.x);
	//printfDx("NormalPos_Wall.z %f\n", normal.z);
	//printfDx("hitPos_ground.x %f\n", hitPos_ground.x);
	//printfDx("hitPos_ground.y %f\n", hitPos_ground.y);
	//printfDx("hitPos_ground.z %f\n", hitPos_ground.z);

	printfDx("tiltAngle_degree %f\n", tiltAngle_degree);

	DrawSphere3D(hitPos_wall, 2.0f, 30, GetColor(0, 0, 0),
		GetColor(0, 255, 0), FALSE);
	DrawSphere3D(hitPos_ground, 2.0f, 30, GetColor(0, 0, 0),
		GetColor(255, 0, 0), FALSE);

	DrawSphere3D(hitPos_head, 2.0f, 30, GetColor(0, 0, 0),
		GetColor(0, 0, 255), FALSE);
	DrawSphere3D(hitHangingPos, 2.0f, 30, GetColor(0, 0, 0),
		GetColor(0, 255, 0), FALSE);

	DrawLine3D(topPos_ray, bottomPos_ray, GetColor(255, 0, 0));
	DrawLine3D(ray_start_hanging_log, ray_end_hanging_log, GetColor(0, 255, 0));

	DrawLine3D(pos_now, pos_new, GetColor(0, 0, 255));
	return true;
}