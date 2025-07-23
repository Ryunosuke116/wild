#include "common.h"
#include "PlayerData.h"
#include "Player.h"
#include "playerAnim_all.h"
#include "PadInput.h"

/// <summary>
/// コンストラクタ
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
/// デストラクタ
/// </summary>
Player::~Player()
{

}

/// <summary>
/// 初期化
/// </summary>
void Player::Initialize()
{

}

/// <summary>
/// 更新
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

    //stateに応じた挙動処理
    auto [moveVec_new, data_new] = nowState->Update(cameraDirection, fieldObjects, *this);

    moveVec = moveVec_new;
    playerData = data_new;
    
    //move計算
    moveVec = playerCalculation->Update(moveVec,
        nowState->GetNowAnimState().PlayTime_anim,
        animationChacger->GetAnimNumber_now(), playerData);

    position = VAdd(position, moveVec);

    // プレイヤーのモデルの座標を更新する
    MV1SetPosition(modelHandle, position);

    ChengeState();

    nowState->MotionUpdate(playerData);


	//矢先 5
	//持ち手（?）2
}

/// <summary>
/// 描画
/// </summary>
bool Player::Draw()
{
	MV1DrawModel(modelHandle);

	VECTOR nowFrame = MV1GetFramePosition(modelHandle, nowFrameNumber);

	DrawSphere3D(nowFrame, 2.0f, 30, GetColor(0, 0, 0),
		GetColor(255, 0, 255), FALSE);

	printfDx("frame現在数%d\n", nowFrameNumber);
	return false;
}

void Player::ChengeState()
{
	//終了時に調整
	nowState->Exit(playerData);

	nowState = std::move(animationChacger->ChangeState(modelHandle, playerData, nowState));

	//初期化処理
	nowState->Enter(playerData);

}

VECTOR Player::Command(const VECTOR& cameraDirection, PlayerData& playerData)
{
    VECTOR returnPos = VGet(0.0f, 0.0f, 0.0f);

    //moveDirを取得する
    returnPos = Move(cameraDirection, playerData);
    JumpMove(playerData);

    return returnPos;
}

/// <summary>
/// 移動処理
/// </summary>
/// <param name="input"></param>
/// <param name="moveVec"></param>
VECTOR Player::Move(const VECTOR& cameraDirection, PlayerData& playerData)
{
    VECTOR returnPos = VGet(0.0f, 0.0f, 0.0f);
    playerData.isMove = false;
    VECTOR rightMove = VCross(cameraDirection, VGet(0.0f, 1.0f, 0.0f));

    //正規化
    rightMove = VNorm(rightMove);
    VECTOR upMove = VNorm(cameraDirection);

    upMove.y = 0.0f;
    rightMove.y = 0.0f;

    //パッド or arrowキーの入力方向で計算
    returnPos = VAdd(VScale(rightMove, -PadInput::GetJoyPad_x_left()),
        VScale(upMove, -PadInput::GetJoyPad_y_left()));

    //0でなければ正規化
    if (VSize(returnPos) != 0)
    {
        returnPos = VNorm(returnPos);
        playerData.isMove = true;
    }

    //必ず正規化されたものか0を返す
    return returnPos;

}

/// <summary>
/// ジャンプ
/// </summary>
void Player::JumpMove(PlayerData& playerData)
{
    if (PadInput::isJump())
    {
        //ジャンプ
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