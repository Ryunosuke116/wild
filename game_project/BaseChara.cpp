#include "common.h"
#include "Input.h"
#include "BaseChara.h"


BaseChara::BaseChara() :
    modelHandle(-1),
    currentAttachIndex(-1),
    motionNum(-1),
    nowFrameNumber(-1),
    keepPlayTime_anim(-1),
    totalTime_anim(-1),
    angle(-1),
    prevAttachIndex(-1),
    animBlendRate(-1),
    prevPlayTime_anim(-1),
    currentPlayAnimSpeed(-1),
    prevPlayAnimSpeed(-1),
    rotate_x(0.0f),
    targetMoveDirection(VGet(0.0f, 0.0f, 0.0f)),
    position(VGet(0.0f, 0.0f, 0.0f))
{

}

BaseChara::~BaseChara()
{
}

/// <summary>
/// ������
/// </summary>
void BaseChara::Initialize()
{

}

/// <summary>
/// �X�V
/// </summary>
void BaseChara::Update() {}

/// <summary>
/// �`��
/// </summary>
bool BaseChara::Draw()
{
    return true;
}

/// <summary>
/// ����
/// </summary>
void BaseChara::UpdateAngle(const VECTOR& direction, bool& isTurn_right)
{
    // �v���C���[�̈ړ������Ƀ��f���̕������߂Â���
    float targetAngle;			// �ڕW�p�x
    float difference;			// �ڕW�p�x�ƌ��݂̊p�x�Ƃ̍�

    // �ڕW�̕����x�N�g������p�x�l���Z�o����
    targetAngle = static_cast<float>(atan2(-direction.x, -direction.z));

    // �ڕW�̊p�x�ƌ��݂̊p�x�Ƃ̍�������o��
    // �ŏ��͒P���Ɉ����Z
    difference = targetAngle - angle;

    // ����������炠������̍����P�W�O�x�ȏ�ɂȂ邱�Ƃ͖����̂�
    // ���̒l���P�W�O�x�ȏ�ɂȂ��Ă�����C������
    if (difference < -DX_PI_F)
    {
        difference += DX_TWO_PI_F;
    }
    else if (difference > DX_PI_F)
    {
        difference -= DX_TWO_PI_F;
    }

    // �p�x�̍����O�ɋ߂Â���
    if (difference > 0.0f)
    {
        isTurn_right = true;

        // �����v���X�̏ꍇ�͈���
        difference -= angleSpeed;
        if (difference < 0.0f)
        {
            difference = 0.0f;
        }
    }
    else
    {
        isTurn_right = false;

        // �����}�C�i�X�̏ꍇ�͑���
        difference += angleSpeed;
        if (difference > 0.0f)
        {
            difference = 0.0f;
        }
    }

    // ���f���̊p�x���X�V
    angle = targetAngle - difference;
    MV1SetRotationXYZ(modelHandle, VGet(rotate_x * DX_PI_F / 180.0f, angle, 0.0f));
}

/// <summary>
/// ���[�V�����ύX
/// </summary>
/// <param name="motionNum"></param>
void BaseChara::ChangeMotion(const int& motionNum, const float playAnimSpeed)
{

    if (prevAttachIndex != -1)
    {
        MV1DetachAnim(modelHandle, prevAttachIndex);
        prevAttachIndex = -1;
    }

    //���܂܂ŏ���prev�ɕۑ�
    prevAttachIndex = currentAttachIndex;
    prevPlayTime_anim = currentPlayTime_anim;
    prevPlayAnimSpeed = currentPlayAnimSpeed;

    //������
  //  MV1DetachAnim(modelHandle, currentAttachIndex);

    //// �Đ����Ԃ��Z�b�g����
    //MV1SetAttachAnimTime(modelHandle, currentAttachIndex, playTime_anim);

    this->motionNum = motionNum;

    // �R�c���f���̂O�Ԗڂ̃A�j���[�V�������A�^�b�`����
    currentAttachIndex = MV1AttachAnim(modelHandle, motionNum, -1, FALSE);

    // �Đ����Ԃ̏�����
    currentPlayTime_anim = 0.0f;
    currentPlayAnimSpeed = playAnimSpeed;

    // �A�^�b�`�����A�j���[�V�����̑��Đ����Ԃ��擾����
    totalTime_anim = MV1GetAttachAnimTotalTime(modelHandle, currentAttachIndex);

    // �u�����h����Prev���L���ł͂Ȃ��ꍇ�͂P�D�O��( ���݃��[�V�������P�O�O���̏�� )�ɂ���
    animBlendRate = prevAttachIndex == -1 ? 1.0f : 0.0f;


}

/// <summary>
/// ���[�V�����X�V
/// </summary>
void BaseChara::MotionUpdate()
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

    if (currentAttachIndex != -1)
    {
        // �A�^�b�`�����A�j���[�V�����̑��Đ����Ԃ��擾����
        totalTime_anim = MV1GetAttachAnimTotalTime(modelHandle, currentAttachIndex);

        //�Đ����ԍX�V
        currentPlayTime_anim += currentPlayAnimSpeed;
        keepPlayTime_anim = currentPlayTime_anim;

        //���Đ����Ԃ𒴂����烊�Z�b�g
        if (currentPlayTime_anim >= totalTime_anim)
        {
            keepPlayTime_anim = currentPlayTime_anim;
            currentPlayTime_anim = static_cast<float>(fmod(currentPlayTime_anim, totalTime_anim));
        }

        // �Đ����Ԃ��Z�b�g����
        MV1SetAttachAnimTime(modelHandle, currentAttachIndex, currentPlayTime_anim);

        //�A�j���[�V�����̃��f���ɑ΂��锽�f�����Z�b�g
        MV1SetAttachAnimBlendRate(modelHandle, currentAttachIndex, animBlendRate);
    }


    //�Đ����Ă���A�j���[�V�����Q�̏���
    if (prevAttachIndex != -1)
    {
        // �A�j���[�V�����̑����Ԃ��擾
        totalTime_anim = MV1GetAttachAnimTotalTime(modelHandle, prevAttachIndex);

        // �Đ����Ԃ�i�߂�
        prevPlayTime_anim += prevPlayAnimSpeed;

        // �Đ����Ԃ������Ԃɓ��B���Ă�����Đ����Ԃ����[�v������
        if (prevPlayTime_anim > totalTime_anim)
        {
            prevPlayTime_anim = static_cast<float>(fmod(prevPlayTime_anim, totalTime_anim));
        }

        // �ύX�����Đ����Ԃ����f���ɔ��f������
        MV1SetAttachAnimTime(modelHandle, prevAttachIndex, prevPlayTime_anim);

        // �A�j���[�V�����Q�̃��f���ɑ΂��锽�f�����Z�b�g
        MV1SetAttachAnimBlendRate(modelHandle, prevAttachIndex, 1.0f - animBlendRate);
    }
}

void BaseChara::PositionUpdate()
{
    // �v���C���[�̃��f���̍��W���X�V����
    MV1SetPosition(modelHandle, position);
}