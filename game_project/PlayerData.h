#pragma once
struct PlayerData
{
	bool isIdle;					//�~�܂��Ă��邩
	bool isMove;					//�����Ă��邩
	bool isRun;						//�����Ă��邩
	bool isJump;					//�W�����v������
	bool isJump_second;				//��i�W�����v������
	bool isJump_PlayAnim;			//�W�����v�̃A�j�����Đ����邩
	bool isJump_run_playAnim;		//�����W�����v�̃A�j�����Đ����邩
	bool isRoll;					//�]���邩
	bool isRoll_PlayAnim;			//���[���A�N�V�����̃A�j�����Đ����邩
	bool isRollFinished;			//���[���A�N�V�������I������
	bool isGround;					//�ڒn���Ă��邩
	bool isHitWall;					//�ǂɓ������Ă��邩
	bool isSprint;					//����o����
	bool isStopRun;					//����I�������
	bool isJumpAll;					//�S�ẴW�����v������������
	bool isFalling;					//��������
	bool isHanging;					//�R���ݒ���
	bool isHang_to_Crouch;			//��ɏオ��
	bool isSlip;
	bool isTurn_right;
	bool isRun_wall;
};