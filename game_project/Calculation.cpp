#include "common.h"
#include <cassert>
#include <unordered_map>
#include <string_view>
#include <array>
#include <vector>
#include <math.h>    
#include <cmath>
#include <memory>
#include "Calculation.h"


/// <summary>
/// �ʐόv�Z
/// </summary>
/// <param name="a"></param>
/// <param name="b"></param>
/// <param name="c"></param>
/// <returns></returns>
float Calculation::area(const VECTOR& a, const VECTOR& b, const VECTOR& c)
{
	VECTOR AB = VSub(b, a);
	VECTOR AC = VSub(c, a);
	VECTOR cross = VCross(AB, AC);

	float area = 0.5f * VSize(cross);
	return 0.5f * VSize(cross);
}

/// <summary>
/// �����Ɠ_�̍Őڋߓ_
/// </summary>
/// <param name="position_1"></param>
/// <param name="position_2"></param>
/// <param name="point"></param>
/// <returns></returns>
VECTOR Calculation::NearestPoint(const VECTOR& position_1, const VECTOR& position_2,
	const VECTOR& point)
{
	//�����Ɠ_�̍ŋߓ_
	VECTOR AB = VSub(position_1, position_2);
	VECTOR AP = VSub(point, position_1);

	//�x�N�g���̒���
	float vectorLength = pow(VSquareSize(AB), 0.5f);

	//�P�ʃx�N�g��
	VECTOR unitVector = VGet(0, 0, 0);
	unitVector.x = AB.x / vectorLength;
	unitVector.y = AB.y / vectorLength;
	unitVector.z = AB.z / vectorLength;
	//����
	float productionVector = VDot(unitVector, AP);

	VECTOR AX;
	AX = VAdd(position_1, VScale(unitVector, productionVector));

	if (AX.y <= position_2.y)
	{
		AX.y = position_2.y;
	}
	if (AX.y >= position_1.y)
	{
		AX.y = position_1.y;
	}

	return AX;
}

/// <summary>
/// �_�ɑ΂��čł��߂��O�p�`�̕�
/// </summary>
/// <param name="poly"></param>
/// <param name="HitPos_ground"></param>
/// <returns></returns>
VECTOR Calculation::SphereMeshOutsideTriangle(const MV1_COLL_RESULT_POLY& poly, const VECTOR& HitPos_ground)
{
	VECTOR nearestPoint;

	//������̓_�Ƃ̍ŋߓ_
	VECTOR nearPoint_1 = NearestPoint(poly.Position[0], poly.Position[1], HitPos_ground);
	VECTOR nearPoint_2 = NearestPoint(poly.Position[0], poly.Position[2], HitPos_ground);
	VECTOR nearPoint_3 = NearestPoint(poly.Position[1], poly.Position[2], HitPos_ground);

	//�e���������߂�
	float d1 = VSize(VSub(nearPoint_1, HitPos_ground));
	float d2 = VSize(VSub(nearPoint_2, HitPos_ground));
	float d3 = VSize(VSub(nearPoint_3, HitPos_ground));


	//��ԋ߂����W��I������
	if (d1 <= d2 && d1 <= d3)
	{
		nearestPoint = nearPoint_1;
	}
	else if (d2 <= d3)
	{
		nearestPoint = nearPoint_2;
	}
	else
	{
		nearestPoint = nearPoint_3;
	}

	return nearestPoint;
}

/// <summary>
/// �_�ɑ΂��čł��߂��O�p�`�̕�
/// </summary>
/// <param name="poly"></param>
/// <param name="HitPos_ground"></param>
/// <returns></returns>
Calculation::NearestResult Calculation::SphereMeshOutsideTriangle_line(const MV1_COLL_RESULT_POLY& poly, const VECTOR& HitPos_ground)
{
	VECTOR nearestPoint;

	NearestResult result;

	//������̓_�Ƃ̍ŋߓ_
	VECTOR nearPoint_1 = NearestPoint(poly.Position[0], poly.Position[1], HitPos_ground);
	VECTOR nearPoint_2 = NearestPoint(poly.Position[0], poly.Position[2], HitPos_ground);
	VECTOR nearPoint_3 = NearestPoint(poly.Position[1], poly.Position[2], HitPos_ground);

	//�e���������߂�
	float d1 = VSize(VSub(nearPoint_1, HitPos_ground));
	float d2 = VSize(VSub(nearPoint_2, HitPos_ground));
	float d3 = VSize(VSub(nearPoint_3, HitPos_ground));


	//��ԋ߂����W��I������
	if (d1 <= d2 && d1 <= d3)
	{
		result.nearestPoint = nearPoint_1;
		result.linePos_start = poly.Position[0];
		result.linePos_end = poly.Position[1];
	}
	else if (d2 <= d3)
	{
		result.nearestPoint = nearPoint_2;
		result.linePos_start = poly.Position[0];
		result.linePos_end = poly.Position[2];
	}
	else
	{
		result.nearestPoint = nearPoint_3;
		result.linePos_start = poly.Position[1];
		result.linePos_end = poly.Position[2];
	}

	return result;
}

/// @brief 
/// @param point 
/// @param a 
/// @param b 
/// @return 
VECTOR Calculation::ProjectionDirection(const VECTOR& point, const VECTOR& a, const VECTOR& b)
{
	VECTOR AB = VSub(b, a);
	VECTOR AP = VSub(point, a);

	//�x�N�g���̒���
	float vectorLength = pow((AB.x * AB.x) + (AB.y * AB.y) + (AB.z * AB.z), 0.5f);

	//�P�ʃx�N�g��(���K��)
	VECTOR unitVector = VGet(0, 0, 0);
	unitVector.x = AB.x / vectorLength;
	unitVector.y = AB.y / vectorLength;
	unitVector.z = AB.z / vectorLength;

	//�_�̎ˉe�ʒu���v�Z(�X�J���[�l)
	float productionVector = (unitVector.x * AP.x) + (unitVector.y * AP.y) + (unitVector.z * AP.z);

	//������̍ŋߓ_���v�Z
	VECTOR AX;
	AX.x = a.x + (unitVector.x * productionVector);
	AX.y = a.y + (unitVector.y * productionVector);
	AX.z = a.z + (unitVector.z * productionVector);

	return AX;
}


/// <summary>
/// ���[�v
/// </summary>
/// <param name="AimPosition"></param>
VECTOR Calculation::Leap(const VECTOR& changePosition, const VECTOR& latestPosition, const float& speed)
{
	VECTOR SubPosition = VSub(latestPosition, changePosition);
	VECTOR scalePosition = VGet(0, 0, 0);

	scalePosition = VScale(SubPosition, speed);

	return VAdd(changePosition, scalePosition);
}

MATRIX Calculation::Rotate(const VECTOR& wall_normal)
{
	VECTOR up = wall_normal;
	VECTOR forward = VGet(0.0f, 1.0f, 0.0f);
	VECTOR right = VGet(1.0f, 0.0f, 0.0f);
	forward = VNorm(VCross(up, right));

	MATRIX rotate;
	rotate.m[0][0] = right.x;	rotate.m[0][1] = right.y;   rotate.m[0][2] = right.z;   rotate.m[0][3] = 0;
	rotate.m[1][0] = up.x;      rotate.m[1][1] = up.y;      rotate.m[1][2] = up.z;      rotate.m[1][3] = 0;
	rotate.m[2][0] = forward.x; rotate.m[2][1] = forward.y; rotate.m[2][2] = forward.z; rotate.m[2][3] = 0;
	rotate.m[3][0] = 0;         rotate.m[3][1] = 0;         rotate.m[3][2] = 0;         rotate.m[3][3] = 1;

	return rotate;
}

/// <summary>
/// ���W�A������x�ɕϊ�
/// </summary>
/// <param name="radian"></param>
/// <returns></returns>
float Calculation::radToDeg(float radian)
{
	return radian * 180.0f / DX_PI_F;
}

void Calculation::Initialize()
{
	/*if (instance != nullptr)
	{
		return;
	}

	instance = new Calculation;*/
}

void Calculation::UnInitialize()
{
	//if (instance)
	//{
	//	delete instance;

	//	instance = nullptr;
	//}
}
