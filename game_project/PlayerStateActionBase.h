#pragma once
#include <memory>
#include <vector>
#include "BaseObject.h"
#include "PlayerData.h"

class Input;
class Player;

class PlayerStateActionBase
{
public:

	struct AnimState
	{
		int AttachIndex;			//�A�j���[�V�������
		float PlayTime_anim;		//�Đ�����
		float TotalPlayTime_anim;	//���Đ�����
		float PlayAnimSpeed;		//�A�j���[�V�����X�s�[�h
	};

	PlayerStateActionBase(int& modelHandle,int& bottomHandle,
		AnimState& oldAnimState, AnimState& nowAnimState);
	~PlayerStateActionBase() {};

	virtual void Initialize(int& modelHandle, int& bottomHandle, PlayerData& playerData)abstract;

	virtual void MotionUpdate(PlayerData& playerData);
	
virtual std::pair<VECTOR, PlayerData> Update(const VECTOR& cameraDirection,
		const std::vector<std::shared_ptr<BaseObject>>& fieldObjects, Player& player)abstract;
	virtual void Enter(PlayerData& playerData) abstract;		//��Ԃɓ������Ƃ�
	virtual void Exit(PlayerData& playerData) abstract;			//��Ԃ𔲂���Ƃ�
	virtual VECTOR Move(const VECTOR& cameraDirection, PlayerData& playerData);
	virtual void AimMove(PlayerData& playerData);
	virtual void SwitchingAnimation(const int& animNum,
		int& modelHandle,
		AnimState& oldAnimState, AnimState& nowAnimState);
	virtual void Transfer_swordState(PlayerData& playerData);

	void SetOldAnimState();
	void ResetOldAnimState();
	void ResetNowAnimState();
	bool Draw();
	void MotionUpdate(int& modelHandle, float& animBlendRate,
		AnimState& nowAnimState, AnimState& oldAnimState);


	virtual VECTOR Command(const VECTOR& cameraDirection, PlayerData& playerData, Player& player)abstract;

	//////////////////////////////////////////////
	// �Q�b�^�[
	//////////////////////////////////////////////
	//int GetPrevAttachIndex() { return oldAnimState.AttachIndex; }
	AnimState GetOldAnimState() const { return oldAnimState; }
	AnimState GetNowAnimState() const { return nowAnimState; }
	float GetAnimBlendRate()const { return animBlendRate; }
	bool GetIsChangeState()const { return isChangeState; }

	void SetPlayAnimSpeed_now(const float set) { nowAnimState.PlayTime_anim = set; }
	void SetAnimNumber_old(const int num) { animNumber_old = num; }

protected:
	static constexpr float	AnimBlendSpeed = 0.05f;		// �A�j���[�V�����̃u�����h���ω����x

	int modelHandle;			//���f���n���h��
	int bottomHandle;
	int animNumber_old;
	float animBlendRate;

	bool isChangeState;
	bool isPush;
	bool isMove_Bow;			//�|���\���Ă��邩

	AnimState oldAnimState;
	AnimState nowAnimState;
	AnimState oldAnimState_bottom;
	AnimState nowAnimState_bottom;

	//�ړ�
	//VECTOR moveVec;

	std::shared_ptr<Input> input = NULL;

	Player* player;

};

