#include "common.h"
#include <vector>
#include <memory>
#include "DxLib.h"
#include "PadInput.h"
#include "PlayerStateActionBase.h"

/// <summary>
/// �R���X�g���N�^
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

    //���܂܂ŏ���prev�ɕۑ�
    this->oldAnimState.AttachIndex = oldAnimState.AttachIndex;

    if (this->oldAnimState.AttachIndex != -1)
    {
        MV1DetachAnim(this->modelHandle, this->oldAnimState.AttachIndex);
        this->oldAnimState.AttachIndex = -1;
    }

    //���܂܂ŏ���prev�ɕۑ�
    this->oldAnimState.AttachIndex = nowAnimState.AttachIndex;
    this->oldAnimState.PlayTime_anim = nowAnimState.PlayTime_anim;
    this->oldAnimState.PlayAnimSpeed = nowAnimState.PlayAnimSpeed;

    // �u�����h����Prev���L���ł͂Ȃ��ꍇ�͂P�D�O��( ���݃��[�V�������P�O�O���̏�� )�ɂ���
    animBlendRate = this->oldAnimState.AttachIndex == -1 ? 1.0f : 0.0f;
}

/// <summary>
/// �A�j���[�V�����X�V
/// </summary>
/// <returns></returns>
bool PlayerStateActionBase::MotionUpdate(PlayerData& playerData)
{
    float totalTime_anim;

    // �u�����h�����P�ȉ��̏ꍇ�͂P�ɋ߂Â���
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
        // �A�^�b�`�����A�j���[�V�����̑��Đ����Ԃ��擾����
        totalTime_anim = MV1GetAttachAnimTotalTime(modelHandle, nowAnimState.AttachIndex);

        //�Đ����ԍX�V
        nowAnimState.PlayTime_anim += nowAnimState.PlayAnimSpeed;

        //���Đ����Ԃ𒴂����烊�Z�b�g
        if (nowAnimState.PlayTime_anim >= totalTime_anim)
        {
            nowAnimState.PlayTime_anim = static_cast<float>(fmod(nowAnimState.PlayTime_anim, totalTime_anim));
        }

        // �Đ����Ԃ��Z�b�g����
        MV1SetAttachAnimTime(modelHandle, nowAnimState.AttachIndex, nowAnimState.PlayTime_anim);

        //�A�j���[�V�����̃��f���ɑ΂��锽�f�����Z�b�g
        MV1SetAttachAnimBlendRate(modelHandle, nowAnimState.AttachIndex, animBlendRate);
    }


    //�Đ����Ă���A�j���[�V�����Q�̏���
    if (oldAnimState.AttachIndex != -1)
    {
        // �A�j���[�V�����̑����Ԃ��擾
        totalTime_anim = MV1GetAttachAnimTotalTime(modelHandle, oldAnimState.AttachIndex);

        // �Đ����Ԃ�i�߂�
       // oldAnimState.PlayTime_anim += oldAnimState.PlayAnimSpeed;

        // �Đ����Ԃ������Ԃɓ��B���Ă�����Đ����Ԃ����[�v������
        if (oldAnimState.PlayTime_anim > totalTime_anim)
        {
            //oldAnimState.PlayTime_anim = static_cast<float>(fmod(oldAnimState.PlayTime_anim, totalTime_anim));
        }

        // �ύX�����Đ����Ԃ����f���ɔ��f������
        MV1SetAttachAnimTime(modelHandle, oldAnimState.AttachIndex, oldAnimState.PlayTime_anim);

        // �A�j���[�V�����Q�̃��f���ɑ΂��锽�f�����Z�b�g
        MV1SetAttachAnimBlendRate(modelHandle, oldAnimState.AttachIndex, 1.0f - animBlendRate);
    }

    return false;
}

/// <summary>
/// �A�j���[�V������ύX����
/// </summary>
/// <param name="animNum"></param>
void PlayerStateActionBase::SwitchingAnimation(const int& animNum)
{
    //�Â������폜
    if (this->oldAnimState.AttachIndex != -1)
    {
        MV1DetachAnim(modelHandle, this->oldAnimState.AttachIndex);
        this->oldAnimState.AttachIndex = -1;
        animBlendRate = 0.0f;
    }

    ////���܂܂ŏ���prev�ɕۑ�
    this->oldAnimState.AttachIndex = nowAnimState.AttachIndex;
    this->oldAnimState.PlayTime_anim = nowAnimState.PlayTime_anim;

    //�A�j���[�V�������A�^�b�`
    this->nowAnimState.AttachIndex = MV1AttachAnim(modelHandle, animNum);

}

/// <summary>
/// �`��
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
/// �ړ�����
/// </summary>
/// <param name="input"></param>
/// <param name="moveVec"></param>
VECTOR PlayerStateActionBase::Move(const VECTOR& cameraDirection, PlayerData& playerData)
{
    VECTOR moveDirection = VGet(0.0f, 0.0f, 0.0f);
    VECTOR moveDirection_old = VGet(0.0f, 0.0f, 0.0f);

    playerData.isMove = false;
    VECTOR rightMove = VCross(cameraDirection, VGet(0.0f, 1.0f, 0.0f));

    //���K��
    rightMove = VNorm(rightMove);
    VECTOR upMove = VNorm(cameraDirection);

    upMove.y = 0.0f;
    rightMove.y = 0.0f;

    //�O�t���[���̃p�b�h�X�e�B�b�N���
    moveDirection_old = VAdd(VScale(rightMove, -PadInput::GetJoyPad_old_x_left()),
        VScale(upMove, -PadInput::GetJoyPad_old_y_left()));

    //�p�b�h or arrow�L�[�̓��͕����Ōv�Z
    moveDirection = VAdd(VScale(rightMove, -PadInput::GetJoyPad_x_left()),
        VScale(upMove, -PadInput::GetJoyPad_y_left()));

    //0�łȂ���ΐ��K��
    if (VSize(moveDirection) != 0)
    {
        moveDirection = VNorm(moveDirection);
    }

    //�O�t���[���ƌ��݂̃t���[���œ��͂���ĂȂ���Γ����ĂȂ�
    if (VSize(moveDirection_old) == 0 &&
        VSize(moveDirection) == 0)
    {
        playerData.isMove = false;
    }
    else
    {
        playerData.isMove = true;
    }

    //�K�����K�����ꂽ���̂�0��Ԃ�
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