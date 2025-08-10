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
    int& bottomHandle, PlayerData& playerData,
    std::shared_ptr<PlayerStateActionBase>& nowState)
{
    std::shared_ptr<PlayerStateActionBase> newState = nullptr;
    bool isUse_bow = SetIsChange_bow();
    bool isChenge_sword = SetIsChange_sword();

    //立ち
    if (playerData.isIdle && animNumber_Now != animNum::idle)
    {
        //nowState内のアニメーション情報を保存
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        //newStateを生成
        newState = std::make_shared<Idle>(modelHandle, bottomHandle, oldAnimState,
            nowAnimState, playerData);

        newState->SetAnimNumber_old(animNumber_Now);
        animNumber_Now = animNum::idle;
    }

    //走る
    if (playerData.isRun && animNumber_Now != animNum::standard_Run)
    {
        //nowState内のアニメーション情報を保存
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        //newStateを生成
        newState = std::make_shared<Run_standard>(modelHandle, bottomHandle, oldAnimState,
            nowAnimState, playerData);

        newState->SetAnimNumber_old(animNumber_Now);
        animNumber_Now = animNum::standard_Run;
    }

    //ジャンプ
    if (playerData.isJump &&
        animNumber_Now != animNum::jump &&
        animNumber_Now != animNum::runJump &&
        animNumber_Now != animNum::standing_Jump_up)
    {
        //nowState内のアニメーション情報を保存
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        //newStateを生成
        newState = std::make_shared<Jump>(modelHandle, bottomHandle, oldAnimState,
            nowAnimState, playerData);

        newState->SetAnimNumber_old(animNumber_Now);
        animNumber_Now = animNum::jump;
    }

    //弓を構える
    if (playerData.isAim && !isUse_bow)
    {
        //nowState内のアニメーション情報を保存
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        //newStateを生成
        newState = std::make_shared<Aim>(modelHandle, bottomHandle, oldAnimState,
            nowAnimState, playerData);

        newState->SetAnimNumber_old(animNumber_Now);
        animNumber_Now = animNum::standing_Draw_Arrow;
    }

    //剣を構える
    if (playerData.isSwordState && !isChenge_sword)
    {
        //nowState内のアニメーション情報を保存
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        //newStateを生成
        newState = std::make_shared<SwordState>(modelHandle, 
            bottomHandle, oldAnimState,
            nowAnimState, playerData);

        newState->SetAnimNumber_old(animNumber_Now);
        animNumber_Now = animNum::sword_Idle;
    }

    if (newState)
    {
        newState->Initialize(modelHandle,bottomHandle, playerData);
        newState->Enter(playerData);
        return newState;
    }

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

/// <summary>
/// 弓を使用しているか
/// </summary>
/// <param name="playerData"></param>
/// <returns></returns>
bool AnimationChanger::SetIsChange_bow()
{
    return (animNumber_Now == animNum::draw_Walk_Back ||
        animNumber_Now == animNum::draw_Walk_Forward || 
        animNumber_Now == animNum::draw_Walk_Left || 
        animNumber_Now == animNum::draw_Walk_Right || 
        animNumber_Now == animNum::standing_Draw_Arrow || 
        animNumber_Now == animNum::standing_Aim_OverDraw || 
        animNumber_Now == animNum::standing_Aim_Walk_Back ||
        animNumber_Now == animNum::standing_Aim_Walk_Forward ||
        animNumber_Now == animNum::standing_Aim_Walk_Left ||
        animNumber_Now == animNum::standing_Aim_Walk_Right ||
        animNumber_Now == animNum::standing_Jump_down ||
        animNumber_Now == animNum::standing_Jump_up ||
        animNumber_Now == animNum::standing_Aim_Recoil ||
        animNumber_Now == animNum::recoil_Walk_Back ||
        animNumber_Now == animNum::recoil_Walk_Forward ||
        animNumber_Now == animNum::recoil_Walk_Left ||
        animNumber_Now == animNum::recoil_Walk_Right ||
        animNumber_Now == animNum::recoil_Jump_down ||
        animNumber_Now == animNum::recoil_Jump_Up
        ) ? true : false;
}

bool AnimationChanger::SetIsChange_sword()
{
    return (animNumber_Now == animNum::draw_Sword ||
        animNumber_Now == animNum::run_Sword ||
        animNumber_Now == animNum::sheath_sword ||
        animNumber_Now == animNum::slash ||
        animNumber_Now == animNum::slash_combo ||
        animNumber_Now == animNum::slash_high ||
        animNumber_Now == animNum::sword_Idle
        ) ? true : false;
}