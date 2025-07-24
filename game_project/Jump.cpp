#include "common.h"
#include <memory>
#include <vector>
#include "BaseObject.h"
#include "PlayerStateActionBase.h"
#include "AnimState.h"
#include "Idle.h"
#include "Jump.h"
#include "Player.h"
#include "PadInput.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="modelHandle"></param>
Jump::Jump(int& modelHandle,
    AnimState& oldAnimState, AnimState& nowAnimState, PlayerData& playerData) :
    PlayerStateActionBase(modelHandle, oldAnimState, nowAnimState)
{

    //�|��e���Ă�����true
    if (animNumber_old == animNum::standing_Aim_Recoil ||
        animNumber_old == animNum::standing_Aim_Walk_Back ||
        animNumber_old == animNum::standing_Aim_Walk_Forward ||
        animNumber_old == animNum::standing_Aim_Walk_Forward ||
        animNumber_old == animNum::standing_Aim_Walk_Left ||
        animNumber_old == animNum::standing_Aim_Walk_Right ||
        animNumber_old == animNum::standing_Aim_OverDraw ||
        animNumber_old == animNum::standing_Draw_Arrow)
    {
        isMove_Bow = true;
    }
    else
    {
        isMove_Bow = false;
    }
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Jump::~Jump()
{
    //  MV1DetachAnim(modelHandle, this->nowAnimState.AttachIndex);
}

/// <summary>
/// ������
/// </summary>
/// <param name="modelHandle"></param>
void Jump::Initialize(int& modelHandle)
{
    // �R�c���f���̂O�Ԗڂ̃A�j���[�V�������A�^�b�`����
    //�����W�����v
    if (animNumber_old == animNum::fast_run ||
        animNumber_old == animNum::standard_Run)
    {
        this->nowAnimState.AttachIndex = MV1AttachAnim(modelHandle, animNum::runJump);
        this->nowAnimState.PlayAnimSpeed = playAnimSpeed_runJump;
        isRun = true;
    }
    
    //�ʏ�W�����v
    if (animNumber_old == animNum::idle)
    {
        this->nowAnimState.AttachIndex = MV1AttachAnim(modelHandle, animNum::jump);
        this->nowAnimState.PlayAnimSpeed = playAnimSpeed_jump;
        isRun = false;
    }

    //�|���\���Ȃ���W�����v
    if (isMove_Bow)
    {
        this->nowAnimState.AttachIndex = MV1AttachAnim(modelHandle, animNum::standing_Jump_up);
    }

    this->nowAnimState.PlayTime_anim = 0.0f;
}

/// <summary>
/// �X�V
/// </summary>
/// <param name="cameraDirection"></param>
/// <param name="fieldObjects"></param>
/// <param name="player"></param>
/// <returns></returns>
std::pair<VECTOR, PlayerData> Jump::Update(const VECTOR& cameraDirection,
    const std::vector<std::shared_ptr<BaseObject>>& fieldObjects, Player& player)
{
    VECTOR moveDirection = VGet(0.0f, 0.0f, 0.0f);

    PlayerData playerData = player.GetData();

    moveDirection = Command(cameraDirection, playerData, player);

    //�ڒn�����ꍇ�A���n�A�j���[�V�����ɐ؂�ւ�
    if (playerData.isGround && isMove_Bow)
    {
        playerData.isLanding = true;
    }
    else if(playerData.isGround && !isRun)
    {
        playerData.isIdle = true;
    }
    else if (playerData.isGround && isRun)
    {
        playerData.isRun = true;
    }

    return std::make_pair(moveDirection, playerData);
}

/// <summary>
/// �A�j���[�V�����X�V
/// </summary>
/// <returns></returns>
bool Jump::MotionUpdate(PlayerData& playerData)
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

    //���݂̃A�j���[�V��������
    if (nowAnimState.AttachIndex != -1)
    {
        // �A�^�b�`�����A�j���[�V�����̑��Đ����Ԃ��擾����
        totalTime_anim = MV1GetAttachAnimTotalTime(modelHandle, nowAnimState.AttachIndex);

        //�Đ����ԍX�V
        nowAnimState.PlayTime_anim += nowAnimState.PlayAnimSpeed;

        //���Đ����Ԃ𒴂����烊�Z�b�g
        if (nowAnimState.PlayTime_anim >= totalTime_anim)
        {
            nowAnimState.PlayTime_anim = totalTime_anim;
        }

        // �Đ����Ԃ��Z�b�g����
        MV1SetAttachAnimTime(modelHandle, nowAnimState.AttachIndex, nowAnimState.PlayTime_anim);

        //�A�j���[�V�����̃��f���ɑ΂��锽�f�����Z�b�g
        MV1SetAttachAnimBlendRate(modelHandle, nowAnimState.AttachIndex, animBlendRate);
    }


    //�O�̃A�j���[�V��������
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

void Jump::Enter(PlayerData& playerData)
{
    playerData.isJump = true;
}

void Jump::Exit(PlayerData& playerData)
{
    playerData.isJump = false;
}

VECTOR Jump::Command(const VECTOR& cameraDirection, PlayerData& playerData, Player& player)
{
    VECTOR moveDirection = VGet(0.0f, 0.0f, 0.0f);

    //moveDir���擾����
    moveDirection = Move(cameraDirection, playerData);

    return moveDirection;
}