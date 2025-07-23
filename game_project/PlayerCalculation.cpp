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

    //ジャンプ計算
    returnVec = Jump(returnVec, animNumber_Now, playerData);

    //重力計算
    jumpPower_now = Gravity(playerData.isGround, jumpPower_now);

    return returnVec;
}

VECTOR PlayerCalculation::Move(const VECTOR& moveDirection, const PlayerData& playerData)
{
    //動いている場合移動スピードを徐々に上げる
    if (playerData.isMove)
    {
        //減速スピード計算していたらfalseに
        if (isCalc_deceleration)
        {
            isCalc_deceleration = false;
        }

        //徐々にスピードを上げる
        moveSpeed_now += 0.05f;

        //maxに達したらそこで止める
        if (moveSpeed_now >= MaxMoveSpeed)
        {
            moveSpeed_now = MaxMoveSpeed;
        }
    }
    else
    {
        //接地しているときに止まったらすぐ止まる
        if (playerData.isGround)
        {
            //アニメーションフレームに合わせて減速
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
        //空中にいるとき
        else
        {
            //徐々に下げる
            moveSpeed_now -= 0.02f;

            //止める
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