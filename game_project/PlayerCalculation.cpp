#include "common.h"
#include "PlayerData.h"
#include "PlayerCalculation.h"
#include "AnimState.h"

PlayerCalculation::PlayerCalculation()
{
    isJumpPower_add = false;
}

VECTOR PlayerCalculation::Update(const VECTOR& moveDirection, const float playTime_anim,
    const int& animNumber_Now, const PlayerData& playerData)
{
    VECTOR returnVec = moveDirection;

    returnVec = Move(returnVec, playerData);

    //�W�����v�v�Z
    returnVec = Jump(returnVec, animNumber_Now, playerData);

    //�d�͌v�Z
    jumpPower_now = Gravity(playerData.isGround, jumpPower_now);

    return returnVec;
}

VECTOR PlayerCalculation::Move(const VECTOR& moveDirection, const PlayerData& playerData)
{
    //�����Ă���ꍇ�ړ��X�s�[�h�����X�ɏグ��
    if (playerData.isMove)
    {
        //�����X�s�[�h�v�Z���Ă�����false��
        if (isCalc_deceleration)
        {
            isCalc_deceleration = false;
        }

        //���X�ɃX�s�[�h���グ��
        moveSpeed_now += 0.05f;

        //max�ɒB�����炻���Ŏ~�߂�
        if (moveSpeed_now >= MaxMoveSpeed)
        {
            moveSpeed_now = MaxMoveSpeed;
        }
    }
    else
    {
        //�ڒn���Ă���Ƃ��Ɏ~�܂����炷���~�܂�
        if (playerData.isGround)
        {
            //�A�j���[�V�����t���[���ɍ��킹�Č���
            if (!isCalc_deceleration)
            {
                const float stopFrame = 10.0f;
                decelerationSpeed = moveSpeed_now / stopFrame;
                isCalc_deceleration = true;
            }

            moveSpeed_now -= decelerationSpeed;

            if (moveSpeed_now < 0.0f)
            {
                moveSpeed_now = 0.0f;
            }
        }
        //�󒆂ɂ���Ƃ�
        else
        {
            //���X�ɉ�����
            moveSpeed_now -= 0.02f;

            //�~�߂�
            if (moveSpeed_now <= 0.0f)
            {
                moveSpeed_now = 0.0f;
            }
        }
    }

    return VScale(moveDirection, moveSpeed_now);
}

VECTOR PlayerCalculation::Jump(const VECTOR& moveVec, const int& animNumber_Now,const PlayerData& playerData)
{
    VECTOR move = moveVec;

    if (isJumpPower_add)
    {
        move.y += jumpPower_now;
    }
    else if (!playerData.isGround)
    {
        move.y += jumpPower_now;
    }

    return move;
}