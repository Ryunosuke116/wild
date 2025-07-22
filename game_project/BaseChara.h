#pragma once
class BaseChara
{
protected:

	int modelHandle;
	int motionNum;
	int HP;
	int nowFrameNumber;
	int animNumber_Now;

	int prevAttachIndex;			//�O�̃A�j���[�V�������
	float prevPlayTime_anim;		//�O��̍Đ�����
	float prevPlayAnimSpeed;		//�O��̃A�j���[�V�����X�s�[�h

	int currentAttachIndex;			//���݂̃A�j���[�V�������
	float currentPlayTime_anim;		//���݂̍Đ�����
	float currentPlayAnimSpeed;		//�O��̃A�j���[�V�����X�s�[�h

	float animBlendRate;
	float keepPlayTime_anim;

	float totalTime_anim;
	float angle;
	float rotate_x;

	VECTOR targetMoveDirection;
	VECTOR framePosition;
	VECTOR position;

	static constexpr float	MoveSpeed = 0.4f;	    // �ړ����x
	static constexpr float angleSpeed = 0.2f;
	static constexpr float	AnimBlendSpeed = 0.1f;		// �A�j���[�V�����̃u�����h���ω����x
	static constexpr float PlayAnimSpeed = 0.5f;	//�A�j���Đ����x
public:

	BaseChara();
	~BaseChara();

	//�������z�֐�
	virtual void Initialize()abstract;
	virtual void Update()abstract;
	virtual bool Draw()abstract;

	virtual void ChangeMotion(const int& motionNum, const float playAnimSpeed);
	virtual void MotionUpdate();
	void UpdateAngle(const VECTOR& direction, bool& isTurn_right);

	/////////////////////////////////////////////
	// �Q�b�^�[
	/////////////////////////////////////////////
	const VECTOR GetPosition() { return position; }
	const VECTOR GetFramePosition()const { return framePosition; }
};

