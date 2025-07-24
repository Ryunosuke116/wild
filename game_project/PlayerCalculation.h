#pragma once
#include "BaseCalculation_chara.h"

class PlayerCalculation :
    public BaseCalculation_chara
{
public:

	PlayerCalculation();
	~PlayerCalculation() {}

	VECTOR Update(const VECTOR& moveDirection,
		const float playTime_anim, const int& animNumber_Now,
		const PlayerData& playerData, const float& deltaTime_new);

	VECTOR Move(const VECTOR& moveDirection,const PlayerData& playerData);

	VECTOR Jump(const VECTOR& moveVec, const int& animNumber_Now, const PlayerData& playerData);

	float GetjumpPower_now() const { return jumpPower_now; }
	float GetmoveSpeed_now() const { return moveSpeed_now; }
	float GetmoveSpeed_max() const { return MaxMoveSpeed; }
	bool GetIsJumpPower_add()const { return isJumpPower_add; }
	void SetJumpPower() { jumpPower_now = addJumpPower; }
	void ChangeIsJumpPower_add_ture() { isJumpPower_add = true; }

private:
	static constexpr float MaxMoveSpeed = 1.6f;	    // �ړ����x
	static constexpr float rollMoveSpeed_max = 2.5f;	//���[�����x
	static constexpr float addJumpPower = 1.6f;		//�W�����v�p���[
	static constexpr float gravity = -0.06f;

	float jumpPower_now;			//���݂̃W�����v�X�s�[�h
	float moveSpeed_now;
	float rollMoveSpeed_now;		//���݂̃��[���X�s�[�h
	float decelerationSpeed;		//�������x
	float run_wall_stopTime;		//�ǂɗ��܂�

	bool isCalc_deceleration;		//�~�܂�Ƃ��Ɉ�x���������X�s�[�h���v�Z
	bool isSlip_after;				//slip�����ォ
	bool isRun_Wall_Stop;
	bool isJumpPower_add;
	VECTOR moveVec_old;
};

