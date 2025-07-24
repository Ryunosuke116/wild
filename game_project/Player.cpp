#include "common.h"
#include "PlayerData.h"
#include "Player.h"
#include "playerAnim_all.h"
#include "PadInput.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Player::Player()
{
	modelHandle = MV1LoadModel("material/mv1/player/player.mv1");
	position = VGet(0.0f, 10.0f, 0.0f);
	MV1SetScale(modelHandle, VGet(modelScale, modelScale, modelScale));
	MV1SetPosition(modelHandle, position);
	nowFrameNumber = 0;
    nowState = std::make_shared<Idle>(modelHandle, oldAnimState, nowAnimState, playerData);
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
    nowState = std::move(animationChacger->ChangeState(modelHandle, playerData, nowState));
    nowState->Initialize(modelHandle);

    positionData.position_bottom = position;
    positionData.position_top = MV1GetFramePosition(modelHandle, 6);
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

    //0�łȂ���Ε����X�V
    if (VSize(moveDirection) != 0)
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

	//��� 5
	//������i?�j2
}

/// <summary>
/// �`��
/// </summary>
bool Player::Draw()
{
	MV1DrawModel(modelHandle);

	VECTOR nowFrame = MV1GetFramePosition(modelHandle, nowFrameNumber);

	DrawSphere3D(position, 2.0f, 30, GetColor(0, 0, 0),
		GetColor(255, 0, 0), FALSE);

    printfDx("playerPosition.x %f\nplayerPosition.y %f\nplayerPosition.z %f\n",
        position.x, position.y, position.z);
	printfDx("frame���ݐ�%d\n", nowFrameNumber);
    printfDx("isGround %d\n", playerData.isGround);
	return false;
}

void Player::ChengeState()
{
	//�I�����ɒ���
	nowState->Exit(playerData);

	nowState = std::move(animationChacger->ChangeState(modelHandle, playerData, nowState));

	//����������
	nowState->Enter(playerData);

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

void Player::Update(){}