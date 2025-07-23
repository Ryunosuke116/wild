#include "common.h"
#include "AnimState.h"
#include "PLayerData.h"
#include "PadInput.h"
#include "AnimationChanger.h"
#include "playerAnim_all.h"

AnimationChanger::AnimationChanger()
{

}

AnimationChanger::~AnimationChanger()
{

}

/// <summary>
/// アニメーション変更
/// </summary>
std::shared_ptr<PlayerStateActionBase> AnimationChanger::ChangeState(int& modelHandle,
    PlayerData& playerData, std::shared_ptr<PlayerStateActionBase>& nowState)
{
    std::shared_ptr<PlayerStateActionBase> newState = nullptr;

    //立ち
    if (playerData.isIdle && animNumber_Now != animNum::idle)
    {
        //nowState内のアニメーション情報を保存
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        //newStateを生成
        newState = std::make_shared<Idle>(modelHandle, oldAnimState,
            nowAnimState, playerData);

        newState->SetAnimNumber_old(animNumber_Now);
        animNumber_Now = animNum::idle;
    }

    //走る
    if (playerData.isRun && animNumber_Now != animNum::standing_Run)
    {
        //nowState内のアニメーション情報を保存
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        //newStateを生成
        newState = std::make_shared<Run_standard>(modelHandle, oldAnimState,
            nowAnimState, playerData);

        newState->SetAnimNumber_old(animNumber_Now);
        animNumber_Now = animNum::standing_Run;
    }

    if(newState) return newState;

    return nowState;
}

/// <summary>
/// アニメーション情報をセット
/// </summary>
/// <param name="AnimState"></param>
void AnimationChanger::SetOldAnimState(PlayerStateActionBase::AnimState animState)
{
    oldAnimState.AttachIndex = animState.AttachIndex;
    oldAnimState.PlayAnimSpeed = animState.PlayAnimSpeed;
    oldAnimState.PlayTime_anim = animState.PlayTime_anim;
    oldAnimState.TotalPlayTime_anim = animState.TotalPlayTime_anim;
}

void AnimationChanger::SetNowAnimState(PlayerStateActionBase::AnimState animState)
{
    nowAnimState.AttachIndex = animState.AttachIndex;
    nowAnimState.PlayAnimSpeed = animState.PlayAnimSpeed;
    nowAnimState.PlayTime_anim = animState.PlayTime_anim;
    nowAnimState.TotalPlayTime_anim = animState.TotalPlayTime_anim;
}