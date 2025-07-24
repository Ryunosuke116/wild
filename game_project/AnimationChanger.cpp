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
/// �A�j���[�V�����ύX
/// </summary>
std::shared_ptr<PlayerStateActionBase> AnimationChanger::ChangeState(int& modelHandle,
    PlayerData& playerData, std::shared_ptr<PlayerStateActionBase>& nowState)
{
    std::shared_ptr<PlayerStateActionBase> newState = nullptr;

    //����
    if (playerData.isIdle && animNumber_Now != animNum::idle)
    {
        //nowState���̃A�j���[�V��������ۑ�
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        //newState�𐶐�
        newState = std::make_shared<Idle>(modelHandle, oldAnimState,
            nowAnimState, playerData);

        newState->SetAnimNumber_old(animNumber_Now);
        animNumber_Now = animNum::idle;
    }

    //����
    if (playerData.isRun && animNumber_Now != animNum::standard_Run)
    {
        //nowState���̃A�j���[�V��������ۑ�
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        //newState�𐶐�
        newState = std::make_shared<Run_standard>(modelHandle, oldAnimState,
            nowAnimState, playerData);

        newState->SetAnimNumber_old(animNumber_Now);
        animNumber_Now = animNum::standard_Run;
    }

    //�W�����v
    if (playerData.isJump &&
        animNumber_Now != animNum::jump &&
        animNumber_Now != animNum::runJump &&
        animNumber_Now != animNum::standing_Jump_up)
    {
        //nowState���̃A�j���[�V��������ۑ�
        SetNowAnimState(nowState->GetNowAnimState());
        SetOldAnimState(nowState->GetOldAnimState());

        //newState�𐶐�
        newState = std::make_shared<Jump>(modelHandle, oldAnimState,
            nowAnimState, playerData);

        newState->SetAnimNumber_old(animNumber_Now);
        animNumber_Now = animNum::jump;
    }

    //�|���\����
    if (playerData.isAim && animNumber_Now != animNum::standing_Draw_Arrow)
    {
    }

    if (newState)
    {
        newState->Initialize(modelHandle);
        return newState;
    }

    return nowState;
}

/// <summary>
/// �A�j���[�V���������Z�b�g
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