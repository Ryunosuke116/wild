#include "common.h"
#include <vector>
#include <memory>
#include "DxLib.h"
#include "PadInput.h"
#include "PlayerStateActionBase.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="modelHandle"></param>
/// <param name="prevAttachIndex"></param>
/// <param name="oldAnimState"></param>
/// <param name="nowAnimState"></param>
PlayerStateActionBase::PlayerStateActionBase(int& modelHandle,
    AnimState& oldAnimState, AnimState& nowAnimState) :
    modelHandle(-1),
    animBlendRate(0.0f),
    isPush(false),
    nowAnimState({ NULL }),
    oldAnimState({ NULL })
{
    this->modelHandle = modelHandle;

    //いままで情報をprevに保存
    this->oldAnimState.AttachIndex = oldAnimState.AttachIndex;

    if (this->oldAnimState.AttachIndex != -1)
    {
        MV1DetachAnim(this->modelHandle, this->oldAnimState.AttachIndex);
        this->oldAnimState.AttachIndex = -1;
    }

    //いままで情報をprevに保存
    this->oldAnimState.AttachIndex = nowAnimState.AttachIndex;
    this->oldAnimState.PlayTime_anim = nowAnimState.PlayTime_anim;
    this->oldAnimState.PlayAnimSpeed = nowAnimState.PlayAnimSpeed;

    // ブレンド率はPrevが有効ではない場合は１．０ｆ( 現在モーションが１００％の状態 )にする
    animBlendRate = this->oldAnimState.AttachIndex == -1 ? 1.0f : 0.0f;
}

/// <summary>
/// アニメーション更新
/// </summary>
/// <returns></returns>
bool PlayerStateActionBase::MotionUpdate(PlayerData& playerData)
{
    float totalTime_anim;

    // ブレンド率が１以下の場合は１に近づける
    if (animBlendRate < 1.0f)
    {
        animBlendRate += AnimBlendSpeed;
        if (animBlendRate > 1.0f)
        {
            animBlendRate = 1.0f;
        }
    }

    if (nowAnimState.AttachIndex != -1)
    {
        // アタッチしたアニメーションの総再生時間を取得する
        totalTime_anim = MV1GetAttachAnimTotalTime(modelHandle, nowAnimState.AttachIndex);

        //再生時間更新
        nowAnimState.PlayTime_anim += nowAnimState.PlayAnimSpeed;

        //総再生時間を超えたらリセット
        if (nowAnimState.PlayTime_anim >= totalTime_anim)
        {
            nowAnimState.PlayTime_anim = static_cast<float>(fmod(nowAnimState.PlayTime_anim, totalTime_anim));
        }

        // 再生時間をセットする
        MV1SetAttachAnimTime(modelHandle, nowAnimState.AttachIndex, nowAnimState.PlayTime_anim);

        //アニメーションのモデルに対する反映率をセット
        MV1SetAttachAnimBlendRate(modelHandle, nowAnimState.AttachIndex, animBlendRate);
    }


    //再生しているアニメーション２の処理
    if (oldAnimState.AttachIndex != -1)
    {
        // アニメーションの総時間を取得
        totalTime_anim = MV1GetAttachAnimTotalTime(modelHandle, oldAnimState.AttachIndex);

        // 再生時間を進める
       // oldAnimState.PlayTime_anim += oldAnimState.PlayAnimSpeed;

        // 再生時間が総時間に到達していたら再生時間をループさせる
        if (oldAnimState.PlayTime_anim > totalTime_anim)
        {
            //oldAnimState.PlayTime_anim = static_cast<float>(fmod(oldAnimState.PlayTime_anim, totalTime_anim));
        }

        // 変更した再生時間をモデルに反映させる
        MV1SetAttachAnimTime(modelHandle, oldAnimState.AttachIndex, oldAnimState.PlayTime_anim);

        // アニメーション２のモデルに対する反映率をセット
        MV1SetAttachAnimBlendRate(modelHandle, oldAnimState.AttachIndex, 1.0f - animBlendRate);
    }

    return false;
}

/// <summary>
/// アニメーションを変更する
/// </summary>
/// <param name="animNum"></param>
void PlayerStateActionBase::SwitchingAnimation(const int& animNum)
{
    //古い情報を削除
    if (this->oldAnimState.AttachIndex != -1)
    {
        MV1DetachAnim(modelHandle, this->oldAnimState.AttachIndex);
        this->oldAnimState.AttachIndex = -1;
        animBlendRate = 0.0f;
    }

    ////いままで情報をprevに保存
    this->oldAnimState.AttachIndex = nowAnimState.AttachIndex;
    this->oldAnimState.PlayTime_anim = nowAnimState.PlayTime_anim;

    //アニメーションをアタッチ
    this->nowAnimState.AttachIndex = MV1AttachAnim(modelHandle, animNum);

}

/// <summary>
/// 描画
/// </summary>
/// <returns></returns>
bool PlayerStateActionBase::Draw()
{
    printfDx("nowAttachIndex %d\n", nowAnimState.AttachIndex);
    printfDx("nowPlayTime_anim %f\n", nowAnimState.PlayTime_anim);
    printfDx("oldAttachIndex %d\n", oldAnimState.AttachIndex);
    printfDx("oldPlayTime_anim %f\n", oldAnimState.PlayTime_anim);
    return true;
}

/// <summary>
/// 移動処理
/// </summary>
/// <param name="input"></param>
/// <param name="moveVec"></param>
VECTOR PlayerStateActionBase::Move(const VECTOR& cameraDirection, PlayerData& playerData)
{
    VECTOR moveDirection = VGet(0.0f, 0.0f, 0.0f);
    VECTOR moveDirection_old = VGet(0.0f, 0.0f, 0.0f);

    playerData.isMove = false;
    VECTOR rightMove = VCross(cameraDirection, VGet(0.0f, 1.0f, 0.0f));

    //正規化
    rightMove = VNorm(rightMove);
    VECTOR upMove = VNorm(cameraDirection);

    upMove.y = 0.0f;
    rightMove.y = 0.0f;

    //前フレームのパッドスティック情報
    moveDirection_old = VAdd(VScale(rightMove, -PadInput::GetJoyPad_old_x_left()),
        VScale(upMove, -PadInput::GetJoyPad_old_y_left()));

    //パッド or arrowキーの入力方向で計算
    moveDirection = VAdd(VScale(rightMove, -PadInput::GetJoyPad_x_left()),
        VScale(upMove, -PadInput::GetJoyPad_y_left()));

    //0でなければ正規化
    if (VSize(moveDirection) != 0)
    {
        moveDirection = VNorm(moveDirection);
    }

    //前フレームと現在のフレームで入力されてなければ動いてない
    if (VSize(moveDirection_old) == 0 &&
        VSize(moveDirection) == 0)
    {
        playerData.isMove = false;
    }
    else
    {
        playerData.isMove = true;
    }

    //必ず正規化されたものか0を返す
    return moveDirection;

}

void PlayerStateActionBase::AimMove(PlayerData& playerData)
{
    if (PadInput::isAim())
    {
        playerData.isAim = true;
        playerData.isUse_bow = true;
    }
}

void PlayerStateActionBase::SetOldAnimState()
{
    oldAnimState.AttachIndex = nowAnimState.AttachIndex;
    oldAnimState.PlayAnimSpeed = nowAnimState.PlayAnimSpeed;
    oldAnimState.PlayTime_anim = nowAnimState.PlayTime_anim;
    oldAnimState.TotalPlayTime_anim = nowAnimState.TotalPlayTime_anim;
}

void PlayerStateActionBase::ResetOldAnimState()
{
    oldAnimState.AttachIndex = -1;
    oldAnimState.PlayAnimSpeed = 0.0f;
    oldAnimState.PlayTime_anim = 0.0f;
    oldAnimState.TotalPlayTime_anim = 0.0f;

}

void PlayerStateActionBase::ResetNowAnimState()
{
    nowAnimState.AttachIndex = -1;
    nowAnimState.PlayAnimSpeed = 0.0f;
    nowAnimState.PlayTime_anim = 0.0f;
    nowAnimState.TotalPlayTime_anim = 0.0f;
}