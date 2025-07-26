#include "common.h"
#include <memory>
#include <vector>
#include "BaseObject.h"
#include "PlayerStateActionBase.h"
#include "AnimState.h"
#include "Aim.h"
#include "Player.h"
#include "PadInput.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="modelHandle"></param>
Aim::Aim(int& modelHandle,
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
    arrowStateNum_now = -1;
    animationNum_now = -1;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Aim::~Aim()
{
    //  MV1DetachAnim(modelHandle, this->nowAnimState.AttachIndex);
}

/// <summary>
/// ������
/// </summary>
/// <param name="modelHandle"></param>
void Aim::Initialize(int& modelHandle, PlayerData& playerData)
{
    // �R�c���f���̂O�Ԗڂ̃A�j���[�V�������A�^�b�`����
    if (!playerData.isMove)
    {
        this->nowAnimState.AttachIndex = MV1AttachAnim(modelHandle, animNum::standing_Draw_Arrow);
        animationNum_now = standing_Draw_Arrow;
    }
    if (playerData.isWalk_forward)
    {
        this->nowAnimState.AttachIndex = MV1AttachAnim(modelHandle, animNum::draw_Walk_Forward);
        animationNum_now = draw_Walk_Forward;
    }
    else if (playerData.isWalk_back)
    {
        this->nowAnimState.AttachIndex = MV1AttachAnim(modelHandle, animNum::draw_Walk_Back);
        animationNum_now = draw_Walk_Back;
    }
    else if (playerData.isWalk_left)
    {
        this->nowAnimState.AttachIndex = MV1AttachAnim(modelHandle, animNum::draw_Walk_Left);
        animationNum_now = draw_Walk_Left;
    }
    else if (playerData.isWalk_right)
    {
        this->nowAnimState.AttachIndex = MV1AttachAnim(modelHandle, animNum::draw_Walk_Right);
        animationNum_now = draw_Walk_Right;
    }

    arrowStateNum_now = ArrowState::draw;
    
    this->nowAnimState.PlayTime_anim = 0.0f;
    this->nowAnimState.PlayAnimSpeed = playAnimSpeed;

}

/// <summary>
/// �X�V
/// </summary>
/// <param name="cameraDirection"></param>
/// <param name="fieldObjects"></param>
/// <param name="player"></param>
/// <returns></returns>
std::pair<VECTOR, PlayerData> Aim::Update(const VECTOR& cameraDirection,
    const std::vector<std::shared_ptr<BaseObject>>& fieldObjects, Player& player)
{
    VECTOR moveDirection = VGet(0.0f, 0.0f, 0.0f);

    PlayerData playerData = player.GetData();

    moveDirection = Command(cameraDirection, playerData, player);

    ChangeArrowState(playerData);

    if (!playerData.isAim)
    {
        if (VSize(moveDirection) != 0)
        {
            playerData.isRun = true;
        }
        else
        {
            playerData.isIdle = true;
        }
    }

    return std::make_pair(moveDirection, playerData);
}

/// <summary>
/// �A�j���[�V�����X�V
/// </summary>
/// <returns></returns>
bool Aim::MotionUpdate(PlayerData& playerData)
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

        //�|��e���Ă���Ƃ�
        //���Đ����Ԃ𒴂�����aim�A�j���[�V�����Ɉڍs
        if (arrowStateNum_now == ArrowState::draw &&
            nowAnimState.PlayTime_anim >= totalTime_anim)
        {
            nowAnimState.PlayTime_anim = static_cast<float>(fmod(nowAnimState.PlayTime_anim, totalTime_anim));
            arrowStateNum_now = ArrowState::aim;
        }

        //�G�C����
        // �Đ����Ԃ������Ԃ𒴂�����Đ����Ԃ����[�v������
        if (arrowStateNum_now == ArrowState::aim &&
            nowAnimState.PlayTime_anim >= totalTime_anim)
        {
            nowAnimState.PlayTime_anim = static_cast<float>(fmod(nowAnimState.PlayTime_anim, totalTime_anim));
        }

        //�����Ƃ�
         // �Đ����Ԃ������Ԃ𒴂�����ʏ펞�ɖ߂�
        if (arrowStateNum_now == ArrowState::recoil && 
            nowAnimState.PlayTime_anim >= totalTime_anim)
        {
            nowAnimState.PlayTime_anim = totalTime_anim;
            playerData.isAim = false;
            playerData.isUse_bow = false;
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

        // �ύX�����Đ����Ԃ����f���ɔ��f������
        MV1SetAttachAnimTime(modelHandle, oldAnimState.AttachIndex, oldAnimState.PlayTime_anim);

        // �A�j���[�V�����Q�̃��f���ɑ΂��锽�f�����Z�b�g
        MV1SetAttachAnimBlendRate(modelHandle, oldAnimState.AttachIndex, 1.0f - animBlendRate);
    }

    return false;
}

void Aim::Enter(PlayerData& playerData)
{
    playerData.isAim = true;
}

void Aim::Exit(PlayerData& playerData)
{
    playerData.isAim = false;
}

/// <summary>
/// ����
/// </summary>
/// <param name="cameraDirection"></param>
/// <param name="playerData"></param>
/// <param name="player"></param>
/// <returns></returns>
VECTOR Aim::Command(const VECTOR& cameraDirection, PlayerData& playerData, Player& player)
{
    VECTOR moveDirection = VGet(0.0f, 0.0f, 0.0f);

    //moveDir���擾����
    moveDirection = Move(cameraDirection, playerData);
    playerData.isWalk_forward = PadInput::isUp();
    playerData.isWalk_back = PadInput::isDown();
    playerData.isWalk_left = PadInput::isLeft();
    playerData.isWalk_right = PadInput::isRight();

    AimMove(playerData);

    return moveDirection;
}

void Aim::AimMove(PlayerData& playerData)
{
  /*  if (PadInput::isAim())
    {
        playerData.isAim = true;
        playerData.isUse_bow = true;
    }
    else
    {
        arrowStateNum_now = ArrowState::recoil;
    }*/
}

void Aim::SwitchingAnimation(const int& animNum)
{
    //�Â������폜
    if (this->oldAnimState.AttachIndex != -1)
    {
        MV1DetachAnim(this->modelHandle, this->oldAnimState.AttachIndex);
        this->oldAnimState.AttachIndex = -1;
    }

    ////���܂܂ŏ���prev�ɕۑ�
   // this->oldAnimState.AttachIndex = nowAnimState.AttachIndex;
    this->oldAnimState.PlayTime_anim = nowAnimState.PlayTime_anim;
    animBlendRate = 0.0f;

    //�����폜
    MV1DetachAnim(this->modelHandle, this->nowAnimState.AttachIndex);
    this->nowAnimState.AttachIndex = -1;

    //�A�j���[�V�������A�^�b�`
    this->nowAnimState.AttachIndex = MV1AttachAnim(modelHandle, animNum);

}

/// <summary>
/// ��Ԃɂ���ăA�j���[�V������ύX����
/// </summary>
void Aim::ChangeArrowState(PlayerData& playerData)
{
    //�|��e���Ă���ꍇ
    Change_draw(playerData);
    
    //�G�C����
    Change_aim(playerData);
 
    //����
    Change_recoil(playerData);
}

/// <summary>
///  �|��e���Ă���ꍇ
/// </summary>
void Aim::Change_draw(PlayerData& playerData)
{
    //�|��e���Ă���ꍇ
    if (arrowStateNum_now == ArrowState::draw)
    {
        if (playerData.isWalk_forward &&
            animationNum_now != animNum::draw_Walk_Forward)
        {
            SwitchingAnimation(animNum::draw_Walk_Forward);
            animationNum_now = animNum::draw_Walk_Forward;
        }
        else if (playerData.isWalk_back &&
            animationNum_now != animNum::draw_Walk_Back)
        {
            SwitchingAnimation(animNum::draw_Walk_Back);
            animationNum_now = animNum::draw_Walk_Back;
        }
        else if (playerData.isWalk_left &&
            animationNum_now != animNum::draw_Walk_Left)
        {
            SwitchingAnimation(animNum::draw_Walk_Left);
            animationNum_now = animNum::draw_Walk_Left;
        }
        else if (playerData.isWalk_right &&
            animationNum_now != animNum::draw_Walk_Right)
        {
            SwitchingAnimation(animNum::draw_Walk_Right);
            animationNum_now = animNum::draw_Walk_Right;
        }
        else if (!playerData.isMove &&
            animationNum_now != animNum::standing_Draw_Arrow)
        {
            SwitchingAnimation(animNum::standing_Draw_Arrow);
            animationNum_now = animNum::standing_Draw_Arrow;
        }
    }
}

/// <summary>
/// �G�C�����̃A�j���[�V�����؂�ւ�
/// </summary>
void Aim::Change_aim(PlayerData& playerData)
{
    if (arrowStateNum_now == ArrowState::aim)
    {
        if (playerData.isWalk_forward &&
            animationNum_now != animNum::standing_Aim_Walk_Forward)
        {
            SwitchingAnimation(animNum::standing_Aim_Walk_Forward);
            animationNum_now = animNum::standing_Aim_Walk_Forward;
        }
        else if (playerData.isWalk_back &&
            animationNum_now != animNum::standing_Aim_Walk_Back)
        {
            SwitchingAnimation(animNum::standing_Aim_Walk_Back);
            animationNum_now = animNum::standing_Aim_Walk_Back;
        }
        else if (playerData.isWalk_left &&
            animationNum_now != animNum::standing_Aim_Walk_Left)
        {
            SwitchingAnimation(animNum::standing_Aim_Walk_Left);
            animationNum_now = animNum::standing_Aim_Walk_Left;
        }
        else if (playerData.isWalk_right &&
            animationNum_now != animNum::standing_Aim_Walk_Right)
        {
            SwitchingAnimation(animNum::standing_Aim_Walk_Right);
            animationNum_now = animNum::standing_Aim_Walk_Right;
        }
        //�����ĂȂ��ꍇ
        else if (!playerData.isMove &&
            animationNum_now != animNum::standing_Aim_OverDraw)
        {
            SwitchingAnimation(animNum::standing_Aim_OverDraw);
            animationNum_now = animNum::standing_Aim_OverDraw;
        }
    }
}

/// <summary>
/// �����Ԃ̃A�j���[�V�����؂�ւ�
/// </summary>
void Aim::Change_recoil(PlayerData& playerData)
{
    if (arrowStateNum_now == ArrowState::recoil)
    {
        if (playerData.isWalk_forward &&
            animationNum_now != animNum::recoil_Walk_Forward)
        {
            SwitchingAnimation(animNum::recoil_Walk_Forward);
        }
        else if (playerData.isWalk_back &&
            animationNum_now != recoil_Walk_Back)
        {
            SwitchingAnimation(animNum::recoil_Walk_Back);
        }
        else if (playerData.isWalk_left &&
            animationNum_now != animNum::recoil_Walk_Left)
        {
            SwitchingAnimation(animNum::recoil_Walk_Left);
        }
        else if (playerData.isWalk_right &&
            animationNum_now != animNum::recoil_Walk_Right)
        {
            SwitchingAnimation(animNum::recoil_Walk_Right);
        }
        //�����ĂȂ��ꍇ
        else if (!playerData.isMove &&
            animationNum_now != animNum::standing_Aim_Recoil)
        {
            SwitchingAnimation(animNum::standing_Aim_Recoil);
        }
    }
}

