#include "common.h"
#include "PlayerData.h"
#include "PlayerCalculation.h"
#include "AnimState.h"

PlayerCalculation::PlayerCalculation()
{
    isJumpPower_add = false;
    jumpPower_now = 0.0f;
    deltaTime = 0.0f;
    velocity_gravity = 0.0f;
    moveSpeed_now = 0.0f;
    decelerationSpeed = 0.0f;
    moveVec_old = VGet(0.0f, 0.0f, 0.0f);
}

VECTOR PlayerCalculation::Update(const VECTOR& moveDirection,
    const float playTime_anim, const int& animNumber_Now,
    const PlayerData& playerData,const float& deltaTime_new)
{
    VECTOR moveVec = moveDirection;
    deltaTime = deltaTime_new;

    moveVec = Move(moveVec, playerData);

    //�d�͂����O�t���[���̃��m���g�p
    moveVec.y = moveVec_old.y;

    //�d�͌v�Z
    moveVec = Gravity(playerData.isGround, moveVec);

    //�W�����v�v�Z
    moveVec = Jump(moveVec, animNumber_Now, playerData);

    //moveVec��ۑ�
    moveVec_old = moveVec;

    return moveVec;
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

    //deltaTime�v�Z
    float velocity = moveSpeed_now;

    return VScale(moveDirection, velocity);
}

VECTOR PlayerCalculation::Jump(const VECTOR& moveVec, const int& animNumber_Now,const PlayerData& playerData)
{
    VECTOR move = moveVec;

    if (isJumpPower_add && playerData.isGround)
    {
        move.y += jumpPower_now;
        isJumpPower_add = false;
    }

    return move;
}