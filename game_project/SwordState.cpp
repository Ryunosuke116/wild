#include "common.h"
#include <memory>
#include <vector>
#include "BaseObject.h"
#include "AnimState.h"
#include "SwordState.h"
#include "PadInput.h"
#include "Player.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="modelHandle"></param>
SwordState::SwordState(int& modelHandle, int& bottomHandle,
    AnimState& oldAnimState, AnimState& nowAnimState, PlayerData& playerData) :
    PlayerStateActionBase(modelHandle, bottomHandle, oldAnimState, nowAnimState)
{
    animationNum_now = -1;
}

/// <summary>
/// デストラクタ
/// </summary>
SwordState::~SwordState()
{
    //  MV1DetachAnim(modelHandle, this->nowAnimState.AttachIndex);
}

/// <summary>
/// 初期化
/// </summary>
void SwordState::Initialize(int& modelHandle, int& bottomHandle, PlayerData& playerData)
{
    // ３Ｄモデルの０番目のアニメーションをアタッチする
    this->nowAnimState.AttachIndex = MV1AttachAnim(modelHandle, animNum::sword_Idle);

    this->nowAnimState.PlayTime_anim = 0.0f;
    this->nowAnimState.PlayAnimSpeed = playAnimSpeed;
    animationNum_now = animNum::sword_Idle;
}

void SwordState::MotionUpdate(PlayerData& playerData)
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

    if (nowAnimState.AttachIndex != -1)
    {
        // アタッチしたアニメーションの総再生時間を取得する
        totalTime_anim = MV1GetAttachAnimTotalTime(modelHandle, nowAnimState.AttachIndex);

        //再生時間更新
        nowAnimState.PlayTime_anim += nowAnimState.PlayAnimSpeed;

        //総再生時間を超えたらリセット
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

        // 再生時間をセットする
        MV1SetAttachAnimTime(modelHandle, nowAnimState.AttachIndex, nowAnimState.PlayTime_anim);

        //アニメーションのモデルに対する反映率をセット
        MV1SetAttachAnimBlendRate(modelHandle, nowAnimState.AttachIndex, animBlendRate);
    }


    //再生しているアニメーション２の処理
    if (oldAnimState.AttachIndex != -1)
    {
        // アニメーションの総時間を取得
        totalTime_anim = MV1GetAttachAnimTotalTime(modelHandle, oldAnimState.AttachIndex);

        // 変更した再生時間をモデルに反映させる
        MV1SetAttachAnimTime(modelHandle, oldAnimState.AttachIndex, oldAnimState.PlayTime_anim);

        // アニメーション２のモデルに対する反映率をセット
        MV1SetAttachAnimBlendRate(modelHandle, oldAnimState.AttachIndex, 1.0f - animBlendRate);
    }
}

/// <summary>
/// 更新
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

    //moveDirを取得する
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