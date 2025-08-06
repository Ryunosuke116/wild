#include "common.h"
#include "PlayerData.h"
#include "Player.h"
#include "playerAnim_all.h"
#include "PadInput.h"
#include "DebugDrawer.h"
#include "ArrowManager.h"

/// <summary>
/// コンストラクタ
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
    // 再生時間をセットする
    MV1SetAttachAnimTime(bottomHandle, attachIndex, currentPlayTime_anim);

    //for (int i = 74; i <= 85; i++)
    //{
    //   // matrixs.push_back(MV1GetFrameLocalWorldMatrix(bottomHandle, i));
    //    MV1SetFrameUserLocalMatrix(modelHandle, i, MV1GetFrameLocalMatrix(bottomHandle, i));
    //}
}

/// <summary>
/// 更新
/// </summary>
void Player::Update(const float& deltaTime, const VECTOR& cameraDirection,
	const std::vector<std::shared_ptr<BaseObject>>& fieldObjects)
{
    positionData.position_bottom = position;
    positionData.position_top = MV1GetFramePosition(modelHandle, 6);

    //調整
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

    //stateに応じた挙動処理
    auto [moveDirection_new, data_new] = nowState->Update(cameraDirection, fieldObjects, *this);

    moveDirection = moveDirection_new;
    playerData = data_new;

    //弓を使っているときはカメラの方向だけ向く
    if (playerData.isUse_bow)
    {
        moveDirection_now = cameraDirection;
    }
    //0でなければ方向更新
    else if (VSize(moveDirection) != 0)
    {
        moveDirection_now = moveDirection;
    }
    
    //move計算
    moveVec = playerCalculation->Update(moveDirection,
        nowState->GetNowAnimState().PlayTime_anim,
        animationChacger->GetAnimNumber_now(), playerData,deltaTime);

    //state変更
    ChengeState();

    //アニメーション更新
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
    /// デバッグ用
    ///////////////////////////////////////

    DebugDrawer::Instance().InformationInput_string_bool("isGround %d\n", playerData.isGround);
    DebugDrawer::Instance().InformationInput_string_bool("isAim %d\n", playerData.isAim);
    DebugDrawer::Instance().InformationInput_string_bool("isUse_bow %d\n", playerData.isUse_bow);
    DebugDrawer::Instance().InformationInput_string_VECTOR("playerPosition.x %f\nplayerPosition.y %f\nplayerPosition.z %f\n", position);
    DebugDrawer::Instance().InformationInput_string_float("JoyPad_x_left %f\n", PadInput::GetJoyPad_x_left());
    DebugDrawer::Instance().InformationInput_string_float("JoyPad_y_left %f\n", PadInput::GetJoyPad_y_left());
    DebugDrawer::Instance().InformationInput_string_int("mesh数 %d", MV1GetMeshNum(modelHandle));
    DebugDrawer::Instance().InformationInput_string_float("angle_aim %f\n", angle_aim);
    //矢先 5
	//持ち手（?）2
}

/// <summary>
/// 描画
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
/// デバッグ描画
/// </summary>
void Player::Draw_log()
{
 /*   printfDx("playerPosition.x %f\nplayerPosition.y %f\nplayerPosition.z %f\n",
        position.x, position.y, position.z);
    printfDx("frame現在数%d\n", nowFrameNumber);
    printfDx("isGround %d\n", playerData.isGround);
    printfDx("AnimTime_now %f\n", nowState->GetNowAnimState().PlayTime_anim);
    printfDx("animBlendRate %f\n", nowState->GetAnimBlendRate());
    printfDx("JoyPad_x_left %f\n", PadInput::GetJoyPad_x_left());
    printfDx("JoyPad_y_left %f\n", PadInput::GetJoyPad_y_left());
    printfDx("GetJoypadDeadZone %f\n", GetJoypadDeadZone(DX_INPUT_KEY_PAD1));*/
}

/// <summary>
/// モーション更新
/// </summary>
void Player::MotionUpdate()
{
    float totalTime_anim;

    // ブレンド率が１以下の場合は１に近づける
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
        // アタッチしたアニメーションの総再生時間を取得する
        totalTime_anim = MV1GetAttachAnimTotalTime(bottomHandle, attachIndex);

        //再生時間更新
        currentPlayTime_anim += currentPlayAnimSpeed;
        keepPlayTime_anim = currentPlayTime_anim;

        //総再生時間を超えたらリセット
        if (currentPlayTime_anim >= totalTime_anim)
        {
            keepPlayTime_anim = currentPlayTime_anim;
            currentPlayTime_anim = static_cast<float>(fmod(currentPlayTime_anim, totalTime_anim));
        }

        // 再生時間をセットする
        MV1SetAttachAnimTime(bottomHandle, attachIndex, currentPlayTime_anim);

        //アニメーションのモデルに対する反映率をセット
        MV1SetAttachAnimBlendRate(bottomHandle, attachIndex, animBlendRate);
    }


    //再生しているアニメーション２の処理
    //if (prevAttachIndex != -1)
    //{
    //    // アニメーションの総時間を取得
    //    totalTime_anim = MV1GetAttachAnimTotalTime(modelHandle, prevAttachIndex);

    //    // 再生時間を進める
    //    prevPlayTime_anim += prevPlayAnimSpeed;

    //    // 再生時間が総時間に到達していたら再生時間をループさせる
    //    if (prevPlayTime_anim > totalTime_anim)
    //    {
    //        prevPlayTime_anim = static_cast<float>(fmod(prevPlayTime_anim, totalTime_anim));
    //    }

    //    // 変更した再生時間をモデルに反映させる
    //    MV1SetAttachAnimTime(modelHandle, prevAttachIndex, prevPlayTime_anim);

    //    // アニメーション２のモデルに対する反映率をセット
    //    MV1SetAttachAnimBlendRate(modelHandle, prevAttachIndex, 1.0f - animBlendRate);
    //}
}

void Player::ChengeState()
{
    if (nowState->GetIsChangeState())
    {
	    //終了時に調整
	    nowState->Exit(playerData);

	    nowState = std::move(animationChacger->ChangeState(modelHandle,bottomHandle, playerData, nowState));
    }

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
/// リストから削除
/// </summary>
/// <param name="observer"></param>
void Player::RemoveObserver(std::shared_ptr<ArrowObserver> observer)
{
    // 削除対象か判定するラムダ関数を用意
    auto shouldRemove = [&](const std::weak_ptr<ArrowObserver>& weakObs) {
        // weak_ptrからshared_ptrを取得
        std::shared_ptr<ArrowObserver> locked = weakObs.lock();

        // 生きていて、かつobserverと同じオブジェクトならtrue
        if (locked) {
            return locked == observer;
        }

        // 期限切れまたは違うオブジェクトならfalse
        return false;
        };

    // remove_ifを呼んで削除対象の要素を末尾に移動させる
    auto newEnd = std::remove_if(observers.begin(), observers.end(), shouldRemove);

    // 実際にvectorの末尾の削除対象部分を削除する
    observers.erase(newEnd, observers.end());
}

/// <summary>
/// オブサーバーへ通知する
/// </summary>
void Player::NotifyRecoilArrow()
{
    //リスト内の要素を一つずつ調査する
    for (auto it = observers.begin(); it != observers.end(); )
    {
        //lockでweak_ptrからshared_ptrへ一時的に変換
        if (auto obs = it->lock())
        {
            obs->OnRecoilArrow(true);
            ++it;
        }
        else
        {
            // すでに破棄されている observer を削除
            it = observers.erase(it);
        }
    }
}

void Player::Update(){}