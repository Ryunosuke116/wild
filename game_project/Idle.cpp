#include "common.h"
#include <memory>
#include <vector>
#include "BaseObject.h"
#include "PlayerStateActionBase.h"
#include "AnimState.h"
#include "Idle.h"
#include "Player.h"
#include "PadInput.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="modelHandle"></param>
Idle::Idle(int& modelHandle,
    AnimState& oldAnimState, AnimState& nowAnimState, PlayerData& playerData) :
    PlayerStateActionBase(modelHandle, oldAnimState, nowAnimState)
{

}

/// <summary>
/// デストラクタ
/// </summary>
Idle::~Idle()
{
    //  MV1DetachAnim(modelHandle, this->nowAnimState.AttachIndex);
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="modelHandle"></param>
void Idle::Initialize(int& modelHandle)
{
    // ３Ｄモデルの０番目のアニメーションをアタッチする
    this->nowAnimState.AttachIndex = MV1AttachAnim(modelHandle, animNum::idle);
 
    this->nowAnimState.PlayTime_anim = 0.0f;
    this->nowAnimState.PlayAnimSpeed = playAnimSpeed;
    isRun = false;
    isJump = false;
    isMove = false;
}

/// <summary>
/// 更新
/// </summary>
/// <param name="cameraDirection"></param>
/// <param name="fieldObjects"></param>
/// <param name="player"></param>
/// <returns></returns>
std::pair<VECTOR,PlayerData> Idle::Update(const VECTOR& cameraDirection,
    const std::vector<std::shared_ptr<BaseObject>>& fieldObjects, Player& player)
{
    VECTOR moveDirection = VGet(0.0f, 0.0f, 0.0f);

    PlayerData playerData = player.GetData();

    moveDirection = Command(cameraDirection, playerData, player);

    return std::make_pair(moveDirection, playerData);
}

void Idle::Enter(PlayerData& playerData)
{
    playerData.isIdle = true;
}

void Idle::Exit(PlayerData& playerData)
{
    playerData.isIdle = false;
}

VECTOR Idle::Command(const VECTOR& cameraDirection, PlayerData& playerData, Player& player)
{
    VECTOR moveDirection = VGet(0.0f, 0.0f, 0.0f);

    //moveDirを取得する
    moveDirection = Move(cameraDirection, playerData);
    JumpMove(playerData,player);
    Aim(playerData);

    if (VSize(moveDirection) != 0.0f)
    {
        playerData.isRun = true;
    }

    return moveDirection;
}


/// <summary>
/// ジャンプ
/// </summary>
void Idle::JumpMove(PlayerData& playerData, Player& player)
{
    if (PadInput::isJump())
    {
        //ジャンプ
        if (!player.playerCalculation->GetIsJumpPower_add() && !isPush)
        {
            playerData.isJump = true;
            isPush = true;
            player.playerCalculation->ChangeIsJumpPower_add_ture();
            player.playerCalculation->SetJumpPower();
        }
    }
    else
    {
        isPush = false;
    }
}
