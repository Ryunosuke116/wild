#pragma once

enum animNum : int
{
	fast_run,						//��������
	idle,							//�ʏ�
	jump,							//�ʏ�W�����v
	standing_Jump_down,				//�\���Ȃ���W�����v
	runJump,						//�����W�����v
	standing_Jump_up,				//�\���Ȃ���W�����v
	standard_Run,					//����
	standing_Aim_OverDraw,			//�\����
	standing_Aim_Recoil,			//����
	standing_Aim_Walk_Back,			//�\���Ȃ������(��)
	standing_Aim_Walk_Forward,		//�\���Ȃ������(�O)
	standing_Aim_Walk_Left,			//�\���Ȃ������(��)
	standing_Aim_Walk_Right,		//�\���Ȃ������(�E)
	standing_Draw_Arrow				//�|������
};