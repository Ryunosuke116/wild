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
/// コンストラクタ
/// </summary>
/// <param name="modelHandle"></param>
Jump::Jump(int& modelHandle,
    AnimState& oldAnimState, AnimState& nowAnimState, PlayerData& playerData) :
    PlayerStateActionBase(modelHandle, oldAnimState, nowAnimState)
{

    //弓を弾いていたらtrue
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
/// デストラクタ
/// </summary>
Jump::~Jump()
{
    //  MV1DetachAnim(modelHandle, this->nowAnimState.AttachIndex);
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="modelHandle"></param>
void Jump::Initialize(int& modelHandle)
{
    // ３Ｄモデルの０番目のアニメーションをアタッチする
    //ランジャンプ
    if (animNumber_old == animNum::fast_run ||
        animNumber_old == animNum::standard_Run)
    {
        this->nowAnimState.AttachIndex = MV1AttachAnim(modelHandle, animNum::runJump);
        this->nowAnimState.PlayAnimSpeed = playAnimSpeed_runJump;
        isRun = true;
    }
    
    //通常ジャンプ
    if (animNumber_old == animNum::idle)
    {
        this->nowAnimState.AttachIndex = MV1AttachAnim(modelHandle, animNum::jump);
        this->nowAnimState.PlayAnimSpeed = playAnimSpeed_jump;
        isRun = false;
    }

    //弓を構えながらジャンプ
    if (isMove_Bow)
    {
        this->nowAnimState.AttachIndex = MV1AttachAnim(modelHandle, animNum::standing_Jump_up);
    }

    this->nowAnimState.PlayTime_anim = 0.0f;
}

/// <summary>
/// 更新
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

    //接地した場合、着地アニメーションに切り替え
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
/// アニメーション更新
/// </summary>
/// <returns></returns>
bool Jump::MotionUpdate(PlayerData& playerData)
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

    //現在のアニメーション処理
    if (nowAnimState.AttachIndex != -1)
    {
        // アタッチしたアニメーションの総再生時間を取得する
        totalTime_anim = MV1GetAttachAnimTotalTime(modelHandle, nowAnimState.AttachIndex);

        //再生時間更新
        nowAnimState.PlayTime_anim += nowAnimState.PlayAnimSpeed;

        //総再生時間を超えたらリセット
        if (nowAnimState.PlayTime_anim >= totalTime_anim)
        {
            nowAnimState.PlayTime_anim = totalTime_anim;
        }

        // 再生時間をセットする
        MV1SetAttachAnimTime(modelHandle, nowAnimState.AttachIndex, nowAnimState.PlayTime_anim);

        //アニメーションのモデルに対する反映率をセット
        MV1SetAttachAnimBlendRate(modelHandle, nowAnimState.AttachIndex, animBlendRate);
    }


    //前のアニメーション処理
    if (oldAnimState.AttachIndex != -1)
    {
        // アニメーションの総時間を取得
        totalTime_anim = MV1GetAttachAnimTotalTime(modelHandle, oldAnimState.AttachIndex);

        // 再生時間を進める
       // oldAnimState.PlayTime_anim += oldAnimState.PlayAnimSpeed;

        // 再生時間が総時間に到達していたら再生時間をループさせる
        if (oldAnimState.PlayTime_anim > totalTime_anim)
        {
            //oldAnimState.PlayTime_anim = static_cast<float>(fmod(oldAnimState.PlayTime_anim, totalTime_anim));
        }

        // 変更した再生時間をモデルに反映させる
        MV1SetAttachAnimTime(modelHandle, oldAnimState.AttachIndex, oldAnimState.PlayTime_anim);

        // アニメーション２のモデルに対する反映率をセット
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

    //moveDirを取得する
    moveDirection = Move(cameraDirection, playerData);

    return moveDirection;
}