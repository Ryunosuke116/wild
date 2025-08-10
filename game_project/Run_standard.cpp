#include "common.h"
#include <memory>
#include <vector>
#include "BaseObject.h"
#include "PlayerStateActionBase.h"
#include "AnimState.h"
#include "Run_standard.h"
#include "Player.h"
#include "PadInput.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="modelHandle"></param>
Run_standard::Run_standard(int& modelHandle, int& bottomHandle,
    AnimState& oldAnimState, AnimState& nowAnimState, PlayerData& playerData) :
    PlayerStateActionBase(modelHandle, bottomHandle, oldAnimState, nowAnimState)
{

}

/// <summary>
/// デストラクタ
/// </summary>
Run_standard::~Run_standard()
{
    //  MV1DetachAnim(modelHandle, this->nowAnimState.AttachIndex);
}

/// <summary>
/// 初期化
/// </summary>
void Run_standard::Initialize(int& modelHandle, int& bottomHandle, PlayerData& playerData)
{

    // ３Ｄモデルの０番目のアニメーションをアタッチする
    this->nowAnimState.AttachIndex = MV1AttachAnim(modelHandle, animNum::standard_Run);

    this->nowAnimState.PlayTime_anim = 0.0f;
    this->nowAnimState.PlayAnimSpeed = playAnimSpeed;
}

std::pair<VECTOR, PlayerData> Run_standard::Update(const VECTOR& cameraDirection,
    const std::vector<std::shared_ptr<BaseObject>>& fieldObjects, Player& player)
{
    VECTOR moveVec = VGet(0.0f, 0.0f, 0.0f);

    PlayerData playerData = player.GetData();

    moveVec = Command(cameraDirection, playerData, player);

    return std::make_pair(moveVec, playerData);
}

void Run_standard::Enter(PlayerData& playerData)
{
    playerData.isRun = true;
}

void Run_standard::Exit(PlayerData& playerData)
{
    playerData.isRun = false;
}

VECTOR Run_standard::Command(const VECTOR& cameraDirection, PlayerData& playerData, Player& player)
{
    VECTOR returnPos = VGet(0.0f, 0.0f, 0.0f);

    //moveDirを取得する
    returnPos = Move(cameraDirection, playerData);
    JumpMove(playerData, player);
    
    if (VSize(returnPos) == 0.0f)
    {
        isChangeState = true;
        playerData.isIdle = true;
    }

    return returnPos;
}

/// <summary>
/// ジャンプ
/// </summary>
void Run_standard::JumpMove(PlayerData& playerData, Player& player)
{
    if (PadInput::isJump())
    {
        //ジャンプ
        if (!player.playerCalculation->GetIsJumpPower_add() && !isPush)
        {
            isChangeState = true;
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
