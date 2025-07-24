#pragma once
struct PlayerData
{
	bool isIdle;					//止まっているか
	bool isMove;					//動いているか
	bool isRun;						//走っているか
	bool isJump;					//ジャンプしたか
	bool isJump_second;				//二段ジャンプしたか
	bool isJump_PlayAnim;			//ジャンプのアニメを再生するか
	bool isJump_run_playAnim;		//ランジャンプのアニメを再生するか
	bool isGround;					//接地しているか
	bool isHitWall;					//壁に当たっているか
	bool isSprint;					//走り出しか
	bool isStopRun;					//走り終わったか
	bool isJumpAll;					//全てのジャンプが完了したか
	bool isFalling;					//落下中か
	bool isHanging;					//崖つかみ中か
	bool isHang_to_Crouch;			//上に上がる
	bool isSlip;					//急転回したか
	bool isTurn_right;				//右に回転しているか
	bool isLanding;					//着地したか
	bool isAim;						//弓を構えるか
};