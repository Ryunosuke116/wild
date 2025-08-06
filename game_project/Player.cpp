#include "common.h"
#include "PlayerData.h"
#include "Player.h"
#include "playerAnim_all.h"
#include "PadInput.h"
#include "DebugDrawer.h"
#include "ArrowManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Player::Player()
{
    upHandle = MV1LoadModel("material/mv1/player/player.mv1");
    bottomHandle = MV1LoadModel("material/mv1/player/bottom.mv1");
	modelHandle = MV1LoadModel("material/mv1/player/player.mv1");
    arrowHandle = MV1LoadModel("material/mv1/player/arrow.mv1");
    frameName = "mixamorig:RightHandThumb4";
    position = VGet(0.0f, 10.0f, 0.0f);
	MV1SetScale(modelHandle, VGet(modelScale, modelScale, modelScale));
	MV1SetPosition(modelHandle, position);
    //MV1SetPosition(arrowHandle, VGet(0.0f, 15.0f, 0.0f));
	nowFrameNumber = 0;
    nowState = std::make_shared<Idle>(modelHandle,bottomHandle, oldAnimState, nowAnimState, playerData);
	animationChacger = std::make_shared<AnimationChanger>();
    playerCalculation = std::make_shared<PlayerCalculation>();

    positionData.position_bottom = VGet(0.0f, 0.0f, 0.0f);
    positionData.position_top = VGet(0.0f, 0.0f, 0.0f);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Player::~Player()
{

}

/// <summary>
/// ������
/// </summary>
void Player::Initialize()
{
    playerData = { false };
    playerData.isIdle = true;
    nowState = std::move(animationChacger->ChangeState(modelHandle,bottomHandle, playerData, nowState));
    nowState->Initialize(modelHandle,bottomHandle,playerData);

    positionData.position_bottom = position;
    positionData.position_top = MV1GetFramePosition(modelHandle, 6);

    int frameIndex = MV1SearchFrame(modelHandle, "mixamorig:RightHandThumb4");

    MATRIX frameMatrix = MV1GetFrameLocalWorldMatrix(modelHandle, frameIndex);

    MV1SetMatrix(arrowHandle, frameMatrix);

    animBlendRate = 1.0f;
    keepPlayTime_anim = 0.0f;
    currentPlayTime_anim = 0.0f;
    currentPlayAnimSpeed = 1.0f;
    angle_aim = 0.0f;
    attachIndex = MV1AttachAnim(bottomHandle, 16);
    // �Đ����Ԃ��Z�b�g����
    MV1SetAttachAnimTime(bottomHandle, attachIndex, currentPlayTime_anim);

    //for (int i = 74; i <= 85; i++)
    //{
    //   // matrixs.push_back(MV1GetFrameLocalWorldMatrix(bottomHandle, i));
    //    MV1SetFrameUserLocalMatrix(modelHandle, i, MV1GetFrameLocalMatrix(bottomHandle, i));
    //}
}

/// <summary>
/// �X�V
/// </summary>
void Player::Update(const float& deltaTime, const VECTOR& cameraDirection,
	const std::vector<std::shared_ptr<BaseObject>>& fieldObjects)
{
    positionData.position_bottom = position;
    positionData.position_top = MV1GetFramePosition(modelHandle, 6);

    //����
    positionData.position_top.x = positionData.position_bottom.x;
    positionData.position_top.z = positionData.position_bottom.z;
    positionData.position_bottom.y -= 0.1f;

	if (CheckHitKey(KEY_INPUT_3))
	{
		nowFrameNumber++;
	}
	else if (CheckHitKey(KEY_INPUT_2))
	{
		nowFrameNumber--;
	}

    moveDirection = VGet(0.0f, 0.0f, 0.0f);

    //state�ɉ�������������
    auto [moveDirection_new, data_new] = nowState->Update(cameraDirection, fieldObjects, *this);

    moveDirection = moveDirection_new;
    playerData = data_new;

    //�|���g���Ă���Ƃ��̓J�����̕�����������
    if (playerData.isUse_bow)
    {
        moveDirection_now = cameraDirection;
    }
    //0�łȂ���Ε����X�V
    else if (VSize(moveDirection) != 0)
    {
        moveDirection_now = moveDirection;
    }
    
    //move�v�Z
    moveVec = playerCalculation->Update(moveDirection,
        nowState->GetNowAnimState().PlayTime_anim,
        animationChacger->GetAnimNumber_now(), playerData,deltaTime);

    //state�ύX
    ChengeState();

    //�A�j���[�V�����X�V
    nowState->MotionUpdate(playerData);

    UpdateAngle(moveDirection_now, playerData.isTurn_right);
    
    //MV1SetRotationXYZ(bottomHandle, VGet(rotate_x * DX_PI_F / 180.0f, angle, 0.0f));

    //MotionUpdate();
    if (!playerData.isAim)
    {
        angle_aim = 0.0f;
    }
     /* MATRIX matrix = MV1GetFrameLocalMatrix(bottomHandle, i);
        MATRIX rotY = MGetRotY(angle_aim);
        matrix = MMult(matrix, rotY);*/
    
    //if (playerData.isAim)
    //{
    //    MV1SetFrameUserLocalMatrix(modelHandle, 1, MV1GetFrameLocalMatrix(bottomHandle, 1));
    //    for (int i = 74; i <= 85; i++)
    //    {
    //        // matrixs.push_back(MV1GetFrameLocalWorldMatrix(bottomHandle, i));
    //        MV1SetFrameUserLocalMatrix(modelHandle, i, MV1GetFrameLocalMatrix(bottomHandle, i));
    //    }
    //}
    //else
    //{
    //    for (int i = 74; i <= 85; i++)
    //    {
    //        // matrixs.push_back(MV1GetFrameLocalWorldMatrix(bottomHandle, i));
    //        MV1ResetFrameUserLocalMatrix(modelHandle, i);
    //    }
    //   
    //}

    /////////////////////////////////////
    /// �f�o�b�O�p
    ///////////////////////////////////////

    DebugDrawer::Instance().InformationInput_string_bool("isGround %d\n", playerData.isGround);
    DebugDrawer::Instance().InformationInput_string_bool("isAim %d\n", playerData.isAim);
    DebugDrawer::Instance().InformationInput_string_bool("isUse_bow %d\n", playerData.isUse_bow);
    DebugDrawer::Instance().InformationInput_string_VECTOR("playerPosition.x %f\nplayerPosition.y %f\nplayerPosition.z %f\n", position);
    DebugDrawer::Instance().InformationInput_string_float("JoyPad_x_left %f\n", PadInput::GetJoyPad_x_left());
    DebugDrawer::Instance().InformationInput_string_float("JoyPad_y_left %f\n", PadInput::GetJoyPad_y_left());
    DebugDrawer::Instance().InformationInput_string_int("mesh�� %d", MV1GetMeshNum(modelHandle));
    DebugDrawer::Instance().InformationInput_string_float("angle_aim %f\n", angle_aim);
    //��� 5
	//������i?�j2
}

/// <summary>
/// �`��
/// </summary>
bool Player::Draw()
{
	MV1DrawModel(modelHandle);
    MV1DrawModel(arrowHandle);

	VECTOR nowFrame = MV1GetFramePosition(modelHandle, nowFrameNumber);

	DrawSphere3D(position, 2.0f, 30, GetColor(0, 0, 0),
		GetColor(255, 0, 0), FALSE);

    return false;
}

/// <summary>
/// �f�o�b�O�`��
/// </summary>
void Player::Draw_log()
{
 /*   printfDx("playerPosition.x %f\nplayerPosition.y %f\nplayerPosition.z %f\n",
        position.x, position.y, position.z);
    printfDx("frame���ݐ�%d\n", nowFrameNumber);
    printfDx("isGround %d\n", playerData.isGround);
    printfDx("AnimTime_now %f\n", nowState->GetNowAnimState().PlayTime_anim);
    printfDx("animBlendRate %f\n", nowState->GetAnimBlendRate());
    printfDx("JoyPad_x_left %f\n", PadInput::GetJoyPad_x_left());
    printfDx("JoyPad_y_left %f\n", PadInput::GetJoyPad_y_left());
    printfDx("GetJoypadDeadZone %f\n", GetJoypadDeadZone(DX_INPUT_KEY_PAD1));*/
}

/// <summary>
/// ���[�V�����X�V
/// </summary>
void Player::MotionUpdate()
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

    if (attachIndex != -1)
    {
        // �A�^�b�`�����A�j���[�V�����̑��Đ����Ԃ��擾����
        totalTime_anim = MV1GetAttachAnimTotalTime(bottomHandle, attachIndex);

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
        MV1SetAttachAnimTime(bottomHandle, attachIndex, currentPlayTime_anim);

        //�A�j���[�V�����̃��f���ɑ΂��锽�f�����Z�b�g
        MV1SetAttachAnimBlendRate(bottomHandle, attachIndex, animBlendRate);
    }


    //�Đ����Ă���A�j���[�V�����Q�̏���
    //if (prevAttachIndex != -1)
    //{
    //    // �A�j���[�V�����̑����Ԃ��擾
    //    totalTime_anim = MV1GetAttachAnimTotalTime(modelHandle, prevAttachIndex);

    //    // �Đ����Ԃ�i�߂�
    //    prevPlayTime_anim += prevPlayAnimSpeed;

    //    // �Đ����Ԃ������Ԃɓ��B���Ă�����Đ����Ԃ����[�v������
    //    if (prevPlayTime_anim > totalTime_anim)
    //    {
    //        prevPlayTime_anim = static_cast<float>(fmod(prevPlayTime_anim, totalTime_anim));
    //    }

    //    // �ύX�����Đ����Ԃ����f���ɔ��f������
    //    MV1SetAttachAnimTime(modelHandle, prevAttachIndex, prevPlayTime_anim);

    //    // �A�j���[�V�����Q�̃��f���ɑ΂��锽�f�����Z�b�g
    //    MV1SetAttachAnimBlendRate(modelHandle, prevAttachIndex, 1.0f - animBlendRate);
    //}
}

void Player::ChengeState()
{
    if (nowState->GetIsChangeState())
    {
	    //�I�����ɒ���
	    nowState->Exit(playerData);

	    nowState = std::move(animationChacger->ChangeState(modelHandle,bottomHandle, playerData, nowState));
    }

}

VECTOR Player::Command(const VECTOR& cameraDirection, PlayerData& playerData)
{
    VECTOR returnPos = VGet(0.0f, 0.0f, 0.0f);

    //moveDir���擾����
    returnPos = Move(cameraDirection, playerData);
    JumpMove(playerData);

    return returnPos;
}

/// <summary>
/// �ړ�����
/// </summary>
/// <param name="input"></param>
/// <param name="moveVec"></param>
VECTOR Player::Move(const VECTOR& cameraDirection, PlayerData& playerData)
{
    VECTOR returnPos = VGet(0.0f, 0.0f, 0.0f);
    playerData.isMove = false;
    VECTOR rightMove = VCross(cameraDirection, VGet(0.0f, 1.0f, 0.0f));

    //���K��
    rightMove = VNorm(rightMove);
    VECTOR upMove = VNorm(cameraDirection);

    upMove.y = 0.0f;
    rightMove.y = 0.0f;

    //�p�b�h or arrow�L�[�̓��͕����Ōv�Z
    returnPos = VAdd(VScale(rightMove, -PadInput::GetJoyPad_x_left()),
        VScale(upMove, -PadInput::GetJoyPad_y_left()));

    //0�łȂ���ΐ��K��
    if (VSize(returnPos) != 0)
    {
        returnPos = VNorm(returnPos);
        playerData.isMove = true;
    }

    //�K�����K�����ꂽ���̂�0��Ԃ�
    return returnPos;

}

/// <summary>
/// �W�����v
/// </summary>
void Player::JumpMove(PlayerData& playerData)
{
    if (PadInput::isJump())
    {
        //�W�����v
        if (!playerCalculation->GetIsJumpPower_add() && !isPush)
        {
            isPush = true;
            playerCalculation->ChangeIsJumpPower_add_ture();
            playerCalculation->SetJumpPower();
        }
    }
    else
    {
        isPush = false;
    }
}


void Player::Receive_CollisionResult()
{
    playerData.isGround = collision_result.isHitGround;
    position = collision_result.position_new;
    playerData.isHitWall = collision_result.isHitWall;
}

/// <summary>
/// ���X�g����폜
/// </summary>
/// <param name="observer"></param>
void Player::RemoveObserver(std::shared_ptr<ArrowObserver> observer)
{
    // �폜�Ώۂ����肷�郉���_�֐���p��
    auto shouldRemove = [&](const std::weak_ptr<ArrowObserver>& weakObs) {
        // weak_ptr����shared_ptr���擾
        std::shared_ptr<ArrowObserver> locked = weakObs.lock();

        // �����Ă��āA����observer�Ɠ����I�u�W�F�N�g�Ȃ�true
        if (locked) {
            return locked == observer;
        }

        // �����؂�܂��͈Ⴄ�I�u�W�F�N�g�Ȃ�false
        return false;
        };

    // remove_if���Ă�ō폜�Ώۂ̗v�f�𖖔��Ɉړ�������
    auto newEnd = std::remove_if(observers.begin(), observers.end(), shouldRemove);

    // ���ۂ�vector�̖����̍폜�Ώە������폜����
    observers.erase(newEnd, observers.end());
}

/// <summary>
/// �I�u�T�[�o�[�֒ʒm����
/// </summary>
void Player::NotifyRecoilArrow()
{
    //���X�g���̗v�f�������������
    for (auto it = observers.begin(); it != observers.end(); )
    {
        //lock��weak_ptr����shared_ptr�ֈꎞ�I�ɕϊ�
        if (auto obs = it->lock())
        {
            obs->OnRecoilArrow(true);
            ++it;
        }
        else
        {
            // ���łɔj������Ă��� observer ���폜
            it = observers.erase(it);
        }
    }
}

void Player::Update(){}