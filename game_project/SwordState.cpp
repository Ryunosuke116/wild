#include "common.h"
#include <memory>
#include <vector>
#include "BaseObject.h"
#include "AnimState.h"
#include "SwordState.h"
#include "PadInput.h"
#include "Player.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="modelHandle"></param>
SwordState::SwordState(int& modelHandle, int& bottomHandle,
    AnimState& oldAnimState, AnimState& nowAnimState, PlayerData& playerData) :
    PlayerStateActionBase(modelHandle, bottomHandle, oldAnimState, nowAnimState)
{
    animationNum_now = -1;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
SwordState::~SwordState()
{
    //  MV1DetachAnim(modelHandle, this->nowAnimState.AttachIndex);
}

/// <summary>
/// ������
/// </summary>
void SwordState::Initialize(int& modelHandle, int& bottomHandle, PlayerData& playerData)
{
    // �R�c���f���̂O�Ԗڂ̃A�j���[�V�������A�^�b�`����
    this->nowAnimState.AttachIndex = MV1AttachAnim(modelHandle, animNum::sword_Idle);

    this->nowAnimState.PlayTime_anim = 0.0f;
    this->nowAnimState.PlayAnimSpeed = playAnimSpeed;
    animationNum_now = animNum::sword_Idle;
}

void SwordState::MotionUpdate(PlayerData& playerData)
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
        if (nowAnimState.PlayTime_anim >= totalTime_anim &&
            animationNum_now == animNum::sword_Idle)
        {
            nowAnimState.PlayTime_anim = static_cast<float>(fmod(nowAnimState.PlayTime_anim, totalTime_anim));
        }

        if (nowAnimState.PlayTime_anim >= totalTime_anim &&
            animationNum_now == animNum::slash_combo)
        {
            nowAnimState.PlayTime_anim = static_cast<float>(fmod(nowAnimState.PlayTime_anim, totalTime_anim));
            SwitchingAnimation(animNum::sword_Idle,
                modelHandle,
                oldAnimState,
                nowAnimState);
            animBlendRate = 1.0f;
            animationNum_now = animNum::sword_Idle;
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

        // �ύX�����Đ����Ԃ����f���ɔ��f������
        MV1SetAttachAnimTime(modelHandle, oldAnimState.AttachIndex, oldAnimState.PlayTime_anim);

        // �A�j���[�V�����Q�̃��f���ɑ΂��锽�f�����Z�b�g
        MV1SetAttachAnimBlendRate(modelHandle, oldAnimState.AttachIndex, 1.0f - animBlendRate);
    }
}

/// <summary>
/// �X�V
/// </summary>
/// <param name="cameraDirection"></param>
/// <param name="fieldObjects"></param>
/// <param name="player"></param>
/// <returns></returns>
std::pair<VECTOR, PlayerData> SwordState::Update(const VECTOR& cameraDirection,
    const std::vector<std::shared_ptr<BaseObject>>& fieldObjects, Player& player)
{
    VECTOR moveDirection = VGet(0.0f, 0.0f, 0.0f);

    PlayerData playerData = player.GetData();

    moveDirection = Command(cameraDirection, playerData, player);

    Command(cameraDirection, playerData, player);


    return std::make_pair(moveDirection, playerData);
}

void SwordState::Enter(PlayerData& playerData)
{
    playerData.isSwordState = true;
}

void SwordState::Exit(PlayerData& playerData)
{
    playerData.isSwordState = false;
}

VECTOR SwordState::Command(const VECTOR& cameraDirection, PlayerData& playerData, Player& player)
{
    VECTOR returnPos = VGet(0.0f, 0.0f, 0.0f);

    //moveDir���擾����
    returnPos = Move(cameraDirection, playerData);
    AttackMove(playerData);

    return returnPos;
}

void SwordState::AttackMove(PlayerData& playerData)
{
    if (PadInput::isSwordAction() && animationNum_now 
        == animNum::sword_Idle)
    {
        SwitchingAnimation(animNum::slash_combo,
            modelHandle,
            oldAnimState,
            nowAnimState);
        animBlendRate = 1.0f;

        animationNum_now = animNum::slash_combo;

    }
}