#pragma once

enum animNum : int
{
	draw_Walk_Back,					//�|��e���Ȃ������(��)
	draw_Walk_Forward,				//�|��e���Ȃ������(�O)
	draw_Walk_Left,					//�|��e���Ȃ������(��)
	draw_Walk_Right,				//�|��e���Ȃ������(�E)
	fast_run,						//��������
	idle,							//�ʏ�
	jump,							//�ʏ�W�����v
	standing_Jump_down,				//�\���Ȃ���W�����v
	runJump,						//�����W�����v
	standing_Jump_up,				//�\���Ȃ���W�����v
	recoil_Jump_down,				//�������Ȃ���W�����v(�~)
	recoil_Jump_Up,					//�������Ȃ���W�����v(��)
	recoil_Walk_Back,				//�������Ȃ������(��)
	recoil_Walk_Forward,			//�������Ȃ������(�O)
	recoil_Walk_Left,				//�������Ȃ������(��)
	recoil_Walk_Right,				//�������Ȃ������(�E)
	standard_Run,					//����
	standing_Aim_OverDraw,			//�\����
	standing_Aim_Recoil,			//����
	standing_Aim_Walk_Back,			//�\���Ȃ������(��)
	standing_Aim_Walk_Forward,		//�\���Ȃ������(�O)
	standing_Aim_Walk_Left,			//�\���Ȃ������(��)
	standing_Aim_Walk_Right,		//�\���Ȃ������(�E)
	standing_Draw_Arrow				//�|������
};

enum animNum_bottom : int
{
	backward,
	backward_left,
	backward_right,
	forward,
	forward_left,
	forward_right
};