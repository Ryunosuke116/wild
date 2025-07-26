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

    //重力だけ前フレームのモノを使用
    moveVec.y = moveVec_old.y;

    //重力計算
    moveVec = Gravity(playerData.isGround, moveVec);

    //ジャンプ計算
    moveVec = Jump(moveVec, animNumber_Now, playerData);

    //moveVecを保存
    moveVec_old = moveVec;

    return moveVec;
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

    //deltaTime計算
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