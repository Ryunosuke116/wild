#pragma once

enum animNum : int
{
	fast_run,						//速く走る
	idle,							//通常
	standing_Run,					//走る
	standing_Am_OverDraw,			//構える
	standing_Aim_Recoil,			//放つ
	standing_Aim_Walk_Back,			//構えながら歩く(後)
	standing_Aim_Walk_Forward,		//構えながら歩く(前)
	standing_Aim_Walk_Left,			//構えながら歩く(左)
	standing_Aim_Walk_Right,		//構えながら歩く(右)
	standing_Draw_Arrow				//弓を引く
};