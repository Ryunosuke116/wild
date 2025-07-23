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

}

/// <summary>
/// �X�V
/// </summary>
void Player::Update(const VECTOR& cameraDirection,
	const std::vector<std::shared_ptr<BaseObject>>& fieldObjects)
{

	if (CheckHitKey(KEY_INPUT_3))
	{
		nowFrameNumber++;
	}
	else if (CheckHitKey(KEY_INPUT_2))
	{
		nowFrameNumber--;
	}

    moveVec = VGet(0.0f, 0.0f, 0.0f);

    //state�ɉ�������������
    auto [moveVec_new, data_new] = nowState->Update(cameraDirection, fieldObjects, *this);

    moveVec = moveVec_new;
    playerData = data_new;
    
    //move�v�Z
    moveVec = playerCalculation->Update(moveVec,
        nowState->GetNowAnimState().PlayTime_anim,
        animationChacger->GetAnimNumber_now(), playerData);

    position = VAdd(position, moveVec);

    // �v���C���[�̃��f���̍��W���X�V����
    MV1SetPosition(modelHandle, position);

    ChengeState();

    nowState->MotionUpdate(playerData);


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

	DrawSphere3D(nowFrame, 2.0f, 30, GetColor(0, 0, 0),
		GetColor(255, 0, 255), FALSE);

	printfDx("frame���ݐ�%d\n", nowFrameNumber);
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
            playerCalculation->SetJumpPower_now();
        }
    }
    else
    {
        isPush = false;
    }
}

void Player::Update(){}