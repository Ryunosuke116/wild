#pragma once
#include "BaseChara.h"
#include "CollisionManager.h"
#include "Calculation.h"
#include  "nlohmann/json.hpp"

class Player : public BaseChara
{
private:

	static constexpr float modelScale = 0.1f;
	static constexpr float MaxMoveSpeed = 1.6f;	    // �ړ����x
	static constexpr float rollMoveSpeed_max = 2.5f;	//���[�����x
	static constexpr float angleSpeed = 0.3f;
	static constexpr float addJumpPower = 1.7f;		//�W�����v�p���[
	static constexpr float gravity = -0.06f;
	static constexpr float run_wall_rotate_x = 30.0f;
	static constexpr float entryDegree_wallRun = 30.0f;

	VECTOR linePos_end;
	VECTOR centerPosition;
	VECTOR topPosition;
	VECTOR bottomPosition;
	VECTOR moveVec;
	VECTOR moveVec_normal;
	VECTOR hangingPoint;
	VECTOR headPos;
	VECTOR handPos_right;
	VECTOR handPos_left;
	VECTOR handCenterPos;
	VECTOR moveDirection_now;		//���݌����Ă������
	VECTOR padInput_now;

	float radian_wall;
	float degree_pad_now;
	float degree_difference;
	float degree_pad_wall_difference;

	bool isPush;					//�{�^������������
	bool isChange_falling;				//�A�j���[�V������ύX���邩
	bool isCalc;
	bool isCalc_moveVec;

	//���N���X

public:
	Player();
	~Player();

	void Initialize();
	void Update()override;
	/*void Update(const VECTOR& cameraDirection,
		const std::vector<std::shared_ptr<BaseObject>>& fieldObjects);*/
	bool Draw();

	//////////////////////////////////
	//�@�Q�b�^�[
	///////////////////////////////////

	VECTOR GetCenterPos() { return centerPosition; }
	VECTOR GetTopPos() { return topPosition; }
	VECTOR GetBottomPos() { return bottomPosition; }
	VECTOR GetMoveVec() { return moveVec; }
	VECTOR GetlinePos_end() { return linePos_end; }
	VECTOR GetHangingPoint() { return hangingPoint; }
	VECTOR GetHeadPos() { return headPos; }
	VECTOR GetHandPos_right() { return handPos_right; }
	VECTOR GetHandPos_left() { return handPos_left; }

	//////////////////////////////////
	/// �Z�b�^�[
	///////////////////////////////// 
	void SetPos(VECTOR newPos) { position = newPos; }
};

