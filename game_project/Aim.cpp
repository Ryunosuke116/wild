#include "common.h"
#include <memory>
#include <vector>
#include "BaseObject.h"
#include "PlayerStateActionBase.h"
#include "AnimState.h"
#include "Aim.h"
#include "Player.h"
#include "PadInput.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="modelHandle"></param>
Aim::Aim(int& modelHandle,
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
    arrowStateNum_now = -1;
    animationNum_now = -1;
}

/// <summary>
/// デストラクタ
/// </summary>
Aim::~Aim()
{
    //  MV1DetachAnim(modelHandle, this->nowAnimState.AttachIndex);
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="modelHandle"></param>
void Aim::Initialize(int& modelHandle, PlayerData& playerData)
{
    // ３Ｄモデルの０番目のアニメーションをアタッチする
    if (!playerData.isMove)
    {
        this->nowAnimState.AttachIndex = MV1AttachAnim(modelHandle, animNum::standing_Draw_Arrow);
        animationNum_now = standing_Draw_Arrow;
    }
    if (playerData.isWalk_forward)
    {
        this->nowAnimState.AttachIndex = MV1AttachAnim(modelHandle, animNum::draw_Walk_Forward);
        animationNum_now = draw_Walk_Forward;
    }
    else if (playerData.isWalk_back)
    {
        this->nowAnimState.AttachIndex = MV1AttachAnim(modelHandle, animNum::draw_Walk_Back);
        animationNum_now = draw_Walk_Back;
    }
    else if (playerData.isWalk_left)
    {
        this->nowAnimState.AttachIndex = MV1AttachAnim(modelHandle, animNum::draw_Walk_Left);
        animationNum_now = draw_Walk_Left;
    }
    else if (playerData.isWalk_right)
    {
        this->nowAnimState.AttachIndex = MV1AttachAnim(modelHandle, animNum::draw_Walk_Right);
        animationNum_now = draw_Walk_Right;
    }

    arrowStateNum_now = ArrowState::draw;
    
    this->nowAnimState.PlayTime_anim = 0.0f;
    this->nowAnimState.PlayAnimSpeed = playAnimSpeed;

}

/// <summary>
/// 更新
/// </summary>
/// <param name="cameraDirection"></param>
/// <param name="fieldObjects"></param>
/// <param name="player"></param>
/// <returns></returns>
std::pair<VECTOR, PlayerData> Aim::Update(const VECTOR& cameraDirection,
    const std::vector<std::shared_ptr<BaseObject>>& fieldObjects, Player& player)
{
    VECTOR moveDirection = VGet(0.0f, 0.0f, 0.0f);

    PlayerData playerData = player.GetData();

    moveDirection = Command(cameraDirection, playerData, player);

    ChangeArrowState(playerData);

    if (!playerData.isAim)
    {
        if (VSize(moveDirection) != 0)
        {
            playerData.isRun = true;
        }
        else
        {
            playerData.isIdle = true;
        }
    }

    return std::make_pair(moveDirection, playerData);
}

/// <summary>
/// アニメーション更新
/// </summary>
/// <returns></returns>
bool Aim::MotionUpdate(PlayerData& playerData)
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

        //弓を弾いているとき
        //総再生時間を超えたらaimアニメーションに移行
        if (arrowStateNum_now == ArrowState::draw &&
            nowAnimState.PlayTime_anim >= totalTime_anim)
        {
            nowAnimState.PlayTime_anim = static_cast<float>(fmod(nowAnimState.PlayTime_anim, totalTime_anim));
            arrowStateNum_now = ArrowState::aim;
        }

        //エイム中
        // 再生時間が総時間を超えたら再生時間をループさせる
        if (arrowStateNum_now == ArrowState::aim &&
            nowAnimState.PlayTime_anim >= totalTime_anim)
        {
            nowAnimState.PlayTime_anim = static_cast<float>(fmod(nowAnimState.PlayTime_anim, totalTime_anim));
        }

        //矢を放つとき
         // 再生時間が総時間を超えたら通常時に戻す
        if (arrowStateNum_now == ArrowState::recoil && 
            nowAnimState.PlayTime_anim >= totalTime_anim)
        {
            nowAnimState.PlayTime_anim = totalTime_anim;
            playerData.isAim = false;
            playerData.isUse_bow = false;
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

        // 変更した再生時間をモデルに反映させる
        MV1SetAttachAnimTime(modelHandle, oldAnimState.AttachIndex, oldAnimState.PlayTime_anim);

        // アニメーション２のモデルに対する反映率をセット
        MV1SetAttachAnimBlendRate(modelHandle, oldAnimState.AttachIndex, 1.0f - animBlendRate);
    }

    return false;
}

void Aim::Enter(PlayerData& playerData)
{
    playerData.isAim = true;
}

void Aim::Exit(PlayerData& playerData)
{
    playerData.isAim = false;
}

/// <summary>
/// 入力
/// </summary>
/// <param name="cameraDirection"></param>
/// <param name="playerData"></param>
/// <param name="player"></param>
/// <returns></returns>
VECTOR Aim::Command(const VECTOR& cameraDirection, PlayerData& playerData, Player& player)
{
    VECTOR moveDirection = VGet(0.0f, 0.0f, 0.0f);

    //moveDirを取得する
    moveDirection = Move(cameraDirection, playerData);
    playerData.isWalk_forward = PadInput::isUp();
    playerData.isWalk_back = PadInput::isDown();
    playerData.isWalk_left = PadInput::isLeft();
    playerData.isWalk_right = PadInput::isRight();

    AimMove(playerData);

    return moveDirection;
}

void Aim::AimMove(PlayerData& playerData)
{
  /*  if (PadInput::isAim())
    {
        playerData.isAim = true;
        playerData.isUse_bow = true;
    }
    else
    {
        arrowStateNum_now = ArrowState::recoil;
    }*/
}

void Aim::SwitchingAnimation(const int& animNum)
{
    //古い情報を削除
    if (this->oldAnimState.AttachIndex != -1)
    {
        MV1DetachAnim(this->modelHandle, this->oldAnimState.AttachIndex);
        this->oldAnimState.AttachIndex = -1;
    }

    ////いままで情報をprevに保存
   // this->oldAnimState.AttachIndex = nowAnimState.AttachIndex;
    this->oldAnimState.PlayTime_anim = nowAnimState.PlayTime_anim;
    animBlendRate = 0.0f;

    //情報を削除
    MV1DetachAnim(this->modelHandle, this->nowAnimState.AttachIndex);
    this->nowAnimState.AttachIndex = -1;

    //アニメーションをアタッチ
    this->nowAnimState.AttachIndex = MV1AttachAnim(modelHandle, animNum);

}

/// <summary>
/// 状態によってアニメーションを変更する
/// </summary>
void Aim::ChangeArrowState(PlayerData& playerData)
{
    //弓を弾いている場合
    Change_draw(playerData);
    
    //エイム中
    Change_aim(playerData);
 
    //放つ
    Change_recoil(playerData);
}

/// <summary>
///  弓を弾いている場合
/// </summary>
void Aim::Change_draw(PlayerData& playerData)
{
    //弓を弾いている場合
    if (arrowStateNum_now == ArrowState::draw)
    {
        if (playerData.isWalk_forward &&
            animationNum_now != animNum::draw_Walk_Forward)
        {
            SwitchingAnimation(animNum::draw_Walk_Forward);
            animationNum_now = animNum::draw_Walk_Forward;
        }
        else if (playerData.isWalk_back &&
            animationNum_now != animNum::draw_Walk_Back)
        {
            SwitchingAnimation(animNum::draw_Walk_Back);
            animationNum_now = animNum::draw_Walk_Back;
        }
        else if (playerData.isWalk_left &&
            animationNum_now != animNum::draw_Walk_Left)
        {
            SwitchingAnimation(animNum::draw_Walk_Left);
            animationNum_now = animNum::draw_Walk_Left;
        }
        else if (playerData.isWalk_right &&
            animationNum_now != animNum::draw_Walk_Right)
        {
            SwitchingAnimation(animNum::draw_Walk_Right);
            animationNum_now = animNum::draw_Walk_Right;
        }
        else if (!playerData.isMove &&
            animationNum_now != animNum::standing_Draw_Arrow)
        {
            SwitchingAnimation(animNum::standing_Draw_Arrow);
            animationNum_now = animNum::standing_Draw_Arrow;
        }
    }
}

/// <summary>
/// エイム中のアニメーション切り替え
/// </summary>
void Aim::Change_aim(PlayerData& playerData)
{
    if (arrowStateNum_now == ArrowState::aim)
    {
        if (playerData.isWalk_forward &&
            animationNum_now != animNum::standing_Aim_Walk_Forward)
        {
            SwitchingAnimation(animNum::standing_Aim_Walk_Forward);
            animationNum_now = animNum::standing_Aim_Walk_Forward;
        }
        else if (playerData.isWalk_back &&
            animationNum_now != animNum::standing_Aim_Walk_Back)
        {
            SwitchingAnimation(animNum::standing_Aim_Walk_Back);
            animationNum_now = animNum::standing_Aim_Walk_Back;
        }
        else if (playerData.isWalk_left &&
            animationNum_now != animNum::standing_Aim_Walk_Left)
        {
            SwitchingAnimation(animNum::standing_Aim_Walk_Left);
            animationNum_now = animNum::standing_Aim_Walk_Left;
        }
        else if (playerData.isWalk_right &&
            animationNum_now != animNum::standing_Aim_Walk_Right)
        {
            SwitchingAnimation(animNum::standing_Aim_Walk_Right);
            animationNum_now = animNum::standing_Aim_Walk_Right;
        }
        //動いてない場合
        else if (!playerData.isMove &&
            animationNum_now != animNum::standing_Aim_OverDraw)
        {
            SwitchingAnimation(animNum::standing_Aim_OverDraw);
            animationNum_now = animNum::standing_Aim_OverDraw;
        }
    }
}

/// <summary>
/// 矢を放つ間のアニメーション切り替え
/// </summary>
void Aim::Change_recoil(PlayerData& playerData)
{
    if (arrowStateNum_now == ArrowState::recoil)
    {
        if (playerData.isWalk_forward &&
            animationNum_now != animNum::recoil_Walk_Forward)
        {
            SwitchingAnimation(animNum::recoil_Walk_Forward);
        }
        else if (playerData.isWalk_back &&
            animationNum_now != recoil_Walk_Back)
        {
            SwitchingAnimation(animNum::recoil_Walk_Back);
        }
        else if (playerData.isWalk_left &&
            animationNum_now != animNum::recoil_Walk_Left)
        {
            SwitchingAnimation(animNum::recoil_Walk_Left);
        }
        else if (playerData.isWalk_right &&
            animationNum_now != animNum::recoil_Walk_Right)
        {
            SwitchingAnimation(animNum::recoil_Walk_Right);
        }
        //動いてない場合
        else if (!playerData.isMove &&
            animationNum_now != animNum::standing_Aim_Recoil)
        {
            SwitchingAnimation(animNum::standing_Aim_Recoil);
        }
    }
}

