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
	bool isGround;					//�ڒn���Ă��邩
	bool isHitWall;					//�ǂɓ������Ă��邩
	bool isSprint;					//����o����
	bool isStopRun;					//����I�������
	bool isJumpAll;					//�S�ẴW�����v������������
	bool isFalling;					//��������
	bool isHanging;					//�R���ݒ���
	bool isHang_to_Crouch;			//��ɏオ��
	bool isSlip;					//�}�]�񂵂���
	bool isTurn_right;				//�E�ɉ�]���Ă��邩
	bool isLanding;					//���n������
	bool isAim;						//�|���\���邩
};