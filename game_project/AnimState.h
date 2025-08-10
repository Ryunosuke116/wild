#pragma once

enum animNum : int
{
	draw_Sword,						//剣を抜く
	draw_Walk_Back,					//弓を弾きながら歩く(後)
	draw_Walk_Forward,				//弓を弾きながら歩く(前)
	draw_Walk_Left,					//弓を弾きながら歩く(左)
	draw_Walk_Right,				//弓を弾きながら歩く(右)
	fast_run,						//速く走る
	idle,							//通常
	jump,							//通常ジャンプ
	standing_Jump_down,				//構えながらジャンプ
	runJump,						//ランジャンプ
	standing_Jump_up,				//構えながらジャンプ
	recoil_Jump_down,				//矢を放ちながらジャンプ(降)
	recoil_Jump_Up,					//矢を放ちながらジャンプ(昇)
	recoil_Walk_Back,				//矢を放ちながら歩く(後)
	recoil_Walk_Forward,			//矢を放ちながら歩く(前)
	recoil_Walk_Left,				//矢を放ちながら歩く(左)
	recoil_Walk_Right,				//矢を放ちながら歩く(右)
	run_Sword,						//剣を持ったまま走る
	sheath_sword,					//剣をしまう
	slash,							//剣攻撃
	slash_combo,					//剣攻撃(コンボ)
	slash_high,						//剣攻撃(フィニッシュ)
	standard_Run,					//走る
	standing_Aim_OverDraw,			//構える
	standing_Aim_Recoil,			//放つ
	standing_Aim_Walk_Back,			//構えながら歩く(後)
	standing_Aim_Walk_Forward,		//構えながら歩く(前)
	standing_Aim_Walk_Left,			//構えながら歩く(左)
	standing_Aim_Walk_Right,		//構えながら歩く(右)
	standing_Draw_Arrow,			//弓を引く
	sword_Idle						//通常(剣)
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