#include "common.h"
#include "HitCheck.h"
#include "Calculation.h"

/// @brief �R���X�g���N�^
/// @return 
HitCheck::HitCheck()
{

}

/// @brief �f�X�g���N�^
/// @return 
HitCheck::~HitCheck()
{

}

/// @brief ray�̓����蔻��
/// @param modelHandle 
/// @param frameIndex 
/// @param linePos_start 
/// @param linePos_end 
/// @param hitPoly 
/// @return 
bool HitCheck::HitRayJudge(const int& modelHandle, int frameIndex,
	VECTOR linePos_start, VECTOR linePos_end, MV1_COLL_RESULT_POLY& hitPoly)
{
	hitPoly = MV1CollCheck_Line(modelHandle, frameIndex, linePos_start, linePos_end);

	return hitPoly.HitFlag;
}

/// @brief ���̓����蔻��
/// @param modelHandle 
/// @param frameIndex 
/// @param linePos_end 
/// @param hitPoly 
/// @return 
bool HitCheck::SphereHitJudge(const int& modelHandle, int frameIndex, VECTOR linePos_end, MV1_COLL_RESULT_POLY_DIM& hitPoly)
{
	hitPoly = MV1CollCheck_Sphere(modelHandle, frameIndex, linePos_end, 3.5f);

	if (hitPoly.HitNum >= 1)return true;
	return false;
}

/// @brief �J�v�Z���̓����蔻��
/// @param modelHandle 
/// @param frameIndex 
/// @param linePos_start 
/// @param linePos_end 
/// @param hitPoly 
void HitCheck::CapsuleHitWallJudge(const int& modelHandle, int frameIndex, float radius,
	VECTOR linePos_start, VECTOR linePos_end, MV1_COLL_RESULT_POLY_DIM& hitPoly)
{
	hitPoly = MV1CollCheck_Capsule(modelHandle, frameIndex, linePos_start, linePos_end, radius);
}

/// <summary>
/// �����̑��̌v�Z
/// </summary>
/// <param name="point"></param>
/// <param name="P"></param>
/// <param name="Q"></param>
/// <returns></returns>
float HitCheck::projectionCalc(const VECTOR& point, const VECTOR& P, const VECTOR& Q)
{
	////�����x�N�g��
	//VECTOR PQ = VSub(Q, P);
	//VECTOR P_point = VSub(point, P);
	//
	////PQ�̓���(2��)
	//float len2 = VDot(PQ, PQ);

	//float t = VDot(PQ, P_point) / len2;

	//VECTOR foot = VAdd(P, VScale(PQ, t));
	//foot = VSub(point, foot);
	//float D = VSize(foot);

	VECTOR a, normal;
	VECTOR AP = VSub(point, a);

	float D = VDot(AP, normal) / VDot(normal, normal);

	return D;
}

/// @brief �ʂƂ̐ڐG���W�̌v�Z
/// @param centerPos 
/// @param a 
/// @param b 
/// @param c 
/// @return 
VECTOR HitCheck::ClosestPtToPointTriangle(VECTOR centerPos, VECTOR a, VECTOR b, VECTOR c)
{

	//P��A�̊O���̒��_���W�̒��ɂ��邩�ǂ����`�F�b�N
	VECTOR ab = VSub(b, a);
	VECTOR ac = VSub(c, a);
	VECTOR ap = VSub(centerPos, a);

	float d1 = VDot(ab, ap);
	float d2 = VDot(ac, ap);

	//P��B�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	VECTOR bp = VSub(centerPos, b);
	float d3 = VDot(ab, bp);
	float d4 = VDot(ac, bp);

	//P��AB�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����P��AB��ɑ΂���ˉe��Ԃ�
	float vc = d1 * d4 - d3 * d2;

	//P��C�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	VECTOR cp = VSub(centerPos, c);
	float d5 = VDot(ab, cp);
	float d6 = VDot(ac, cp);

	//P��AC�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����P��AC��ɑ΂���ˉe��Ԃ�
	float vb = d5 * d2 - d1 * d6;

	//P��BC�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����P��BC��ɑ΂���ˉe��Ԃ�
	float va = d3 * d6 - d5 * d4;

	//P�͖ʗ̈�̒��ɂ���BQ�����̏d�S���W(u,v,w)��p���Čv�Z
	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;

	return VAdd(a, VAdd(VScale(ab, v), VScale(ac, w)));

}



void HitCheck::AABB()
{
	float r = 3.5f;
	float min_X, min_Y, min_Z;
	float max_X, max_Y, max_Z;

	min_X = r * -1.0f;
	min_X = r * -1.0f;
	min_Z = r * -1.0f;
	max_X = r * 1.0f;
	max_Y = r * 1.0f;
	max_Z = r * 1.0f;

}

/// <summary>
/// ���Ƌ��̓����蔻��
/// </summary>
/// <param name="position_1"></param>
/// <param name="position_2"></param>
/// <param name="radius_1"></param>
/// <param name="radius_2"></param>
/// <returns></returns>
bool HitCheck::HitConfirmation(VECTOR position_1, VECTOR position_2, const float radius_1, const float radius_2)
{

	VECTOR positionSub = VSub(position_1, position_2);

	//���Ƌ��̒��S�_�Ƃ̋���
	float positionRange = VSize(positionSub);

	//���a�̍��v
	float radiusAdd = radius_1 + radius_2;

	// 1��2�̔��a�������Ƌ��̒��S�_�Ƃ̋������߂���Γ������Ă���
	if (radiusAdd > positionRange)
	{
		return true;
	}
	return false;
}

/// <summary>
/// �J�v�Z������ł��߂������v�Z
/// </summary>
/// <param name="capsulePosition_1"></param>
/// <param name="capsulePosition_2"></param>
/// <param name="position"></param>
/// <param name="radius_1"></param>
/// <param name="radius_2"></param>
/// <returns></returns>
VECTOR HitCheck::CapsuleHitConfirmation(VECTOR capsulePosition_1, VECTOR capsulePosition_2, VECTOR position, const float radius_1, const float radius_2)
{
	VECTOR AB = VSub(capsulePosition_2, capsulePosition_1);
	VECTOR AP = VSub(position, capsulePosition_1);

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
	AX.x = capsulePosition_1.x + (unitVector.x * productionVector);
	AX.y = capsulePosition_1.y + (unitVector.y * productionVector);
	AX.z = capsulePosition_1.z + (unitVector.z * productionVector);


	if (AX.y <= capsulePosition_2.y)
	{
		AX.y = capsulePosition_2.y;
	}
	if (AX.y >= capsulePosition_1.y)
	{
		AX.y = capsulePosition_1.y;
	}

	return AX;

}

/// <summary>
/// �ʐς��g�����_�̓��O����v�Z
/// </summary>
/// <param name="p"></param>
/// <param name="q"></param>
/// <param name="a"></param>
/// <param name="b"></param>
/// <param name="c"></param>
/// <param name="normal"></param>
/// <returns></returns>
std::pair<VECTOR, VECTOR> HitCheck::SegmentTriangleDistance(const VECTOR& p, const VECTOR& q, const VECTOR& a, const VECTOR& b, const VECTOR& c, const VECTOR& normal)
{
	//�����̕����x�N�g��
	VECTOR PQ = VSub(q, p);

	//������x�������Ĉ�����ׂ�
	const int num = 100;
	float minSize = 1000;
	VECTOR returnPoint;		//�ʂƂ̐ڐG�_
	VECTOR returnPT;		//�ʂƈ�ԋ߂������_

	VECTOR point_P = ClosestPtToPointTriangle(p, a, b, c);
	VECTOR point_Q = ClosestPtToPointTriangle(q, a, b, c);

	if (TriangleAreaCheck(point_P, a, b, c) && TriangleAreaCheck(point_Q, a, b, c))
	{
		//����������Ԃ�
		if (VSize(point_P) < VSize(point_Q))
		{
			return std::make_pair(p, point_P);
		}
		else
		{
			return std::make_pair(q, point_Q);
		}
	}
	else
	{
		float a;
	}

	bool flag = false;

	for (int i = 0; i < num; i++)
	{
		float t = float(i) / num;

		//�����̂ǂ��𒲂ׂ邩
		VECTOR PT = VAdd(p, VScale(PQ, t));

		//�ʂ̂ǂ��ɓ������Ă��邩
		VECTOR point = ClosestPtToPointTriangle(PT, a, b, c);

		//�O�p�`�̓������ǂ���
		if (TriangleAreaCheck(point, a, b, c))
		{

			//�����̓_����ڐG�ʂ܂ł̑傫��
			VECTOR size = VSub(point, PT);

			//��ԋ������߂����̂�I��
			if (minSize > VSize(size))
			{
				minSize = VSize(size);
				returnPT = PT;
				returnPoint = point;
				flag = true;
			}
		}
	}

	if (!flag)
	{
		return std::make_pair(q, point_Q);
	}

	//��ԋ߂������̓_�Ɩʂ̏Փ˃|�C���g��Ԃ�
	return std::make_pair(returnPT, returnPoint);

}

/// @brief �O�p�`�̓����ɓ_�����邩
/// @param point 
/// @param a 
/// @param b 
/// @param c 
/// @return 
bool HitCheck::TriangleAreaCheck(const VECTOR& point, const VECTOR& a, const VECTOR& b, const VECTOR& c)
{
	//�ʐς����߂�
	float area = fabs(Calculation::area(a, b, c));
	float area_1 = fabs(Calculation::area(a, b, point));
	float area_2 = fabs(Calculation::area(b, c, point));
	float area_3 = fabs(Calculation::area(c, a, point));

	//���ʐςƓ_���g�����ʐς̍��v�̍���������
	float abs_ = abs((area_1 + area_2 + area_3) - area);

	//0���ォ�������邩
	bool area_equal = abs_ < 1e-10;
	bool inside = area_1 > 0 && area_2 > 0 && area_3 > 0;

	return (area_equal && inside) ? true : false;
}

/// <summary>
/// �O�p�`�̓����ɓ_�����邩
///			�����͓����Ƃ���
/// </summary>
/// <param name="point"></param>
/// <param name="a"></param>
/// <param name="b"></param>
/// <param name="c"></param>
/// <returns></returns>
bool HitCheck::TriangleAreaCheck_ground(const VECTOR& point, const VECTOR& a, const VECTOR& b, const VECTOR& c)
{

	VECTOR point_ = point;
	VECTOR a_ = a;
	VECTOR b_ = b;
	VECTOR c_ = c;

	point_.y = 0.0f;
	a_.y = 0.0f;
	b_.y = 0.0f;
	c_.y = 0.0f;

	//�ʐς����߂�
	float area = fabs(Calculation::area(a_, b_, c_));
	float area_1 = fabs(Calculation::area(a_, b_, point_));
	float area_2 = fabs(Calculation::area(b_, c_, point_));
	float area_3 = fabs(Calculation::area(c_, a_, point_));

	//���ʐςƓ_���g�����ʐς̍��v�̍���������
	float abs_ = abs((area_1 + area_2 + area_3) - area);

	//0���ォ�������邩
	bool area_equal = abs_ < 1e-1;
	bool inside = area_1 > 0 && area_2 > 0 && area_3 > 0;

	return (area_equal && inside) ? true : false;
}
