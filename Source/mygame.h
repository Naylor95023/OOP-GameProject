/*
 * mygame.h: 本檔案儲遊戲本身的class的interface
 * Copyright (C) 2002-2008 Woei-Kae Chen <wkc@csie.ntut.edu.tw>
 *
 * This file is part of game, a free game development framework for windows.
 *
 * game is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * game is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *	 2004-03-02 V4.0
 *      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
 *         demonstrate the use of states.
 *   2005-09-13
 *      Rewrite the codes for CBall and CEraser.
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *   2006-02-08 V4.2
 *      1. Rename OnInitialUpdate() -> OnInit().
 *      2. Replace AUDIO_CANYON as AUDIO_NTUT.
 *      3. Add help bitmap to CGameStateRun.
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *   2008-02-15 V4.4
 *      1. Add namespace game_framework.
 *      2. Replace the demonstration of animation as a new bouncing ball.
 *      3. Use ShowInitProgress(percent) to display loading progress. 
*/

/////////////////////////////////////////////////////////////////////////////
// Constants
/////////////////////////////////////////////////////////////////////////////

enum AUDIO_ID {				// 定義各種音效的編號
	AUDIO_Start,			// 0
	AUDIO_Over,				// 1
	AUDIO_Game,				// 2
	AUDIO_Ding,              //吃金幣音效
	AUDIO_Super,				//噴射音效
	AUDIO_MenuMove,				//選單 移動音效
	AUDIO_MenuSelect,			//選單確認音效
	AUDIO_Die                   //死亡音效
};

namespace game_framework {
/////////////////////////////////////////////////////////////////////////////
// 這個class提供可以用鍵盤或滑鼠控制的擦子
// 看懂就可以改寫成自己的程式了
/////////////////////////////////////////////////////////////////////////////

class CMan;
class CGameMap
{
public:
	//CGameMap();
	void LoadBitmap();
	void OnShow();
	void OnMove(CMan*);
	void OnKeyDown(UINT);
	void Initialize();
	void MapInitialize();
	~CGameMap();
	int screenx(int);
	int screeny(int);
	int map[10000][600];   //座標
	int X,Y;
	int speed;
	int old_speed;				//改變速度前先記錄原本速度
	bool first;					//是否第一次儲存speed
	int count;
protected:
	CMovingBitmap back;   //背景動畫
	CMovingBitmap fire,gold,star;
	CMovingBitmap roadblock;  //路障
	CMovingBitmap roadblock1;  //路障1
	CMovingBitmap background;
};
class CMan               //人物
{
public:
	CMan();
	int  GetX1();					// 人物左上角 x 座標
	int  GetY1();					// 人物左上角 y 座標
	int  GetX2();					// 人物右下角 x 座標
	int  GetY2();					// 人物右下角 y 座標
	void Initialize();				// 設定人物為初始值
	void LoadBitmap();				// 載入圖形
	void OnMove(CGameMap*);					// 移動人物
	void OnShow(CGameMap*);					// 將人物圖形貼到畫面
	void SetMovingDown(bool flag);	// 設定是否正在往下移動
	void SetMovingLeft(bool flag);	// 設定是否正在往左移動
	void SetMovingRight(bool flag); // 設定是否正在往右移動
	void SetMovingUp(bool flag);	// 設定是否正在往上移動
	void SetXY(int nx, int ny);		// 設定人物左上角座標
	void Invincible();				//使用道具
	void Invincibled(CGameMap*);	//道具效果結束
	void LiveDie(CGameMap*);		//判斷是否碰到陷阱
	void GetItem(CGameMap*);		//是否吃道道具
	void ManJump(CGameMap*);		//控制人物跳躍
	int velocity;
	int initial_velocity;
	int flag;
	bool rising;
	int height,width;
	int floor;
	bool die;                  //是否over
	bool Eat;
	int count;
	int speed;
	int x, y;					// 人物左上角座標
	int jumpSub;			
	int HaveStarCount;		//無敵道具個數
	int EnableControlMan;   //是否可以控制人物
	bool UseInvincible;    //使用無敵
	int CountInvincible;   //無敵時間
	int MoveUp,MoveDown;    //無敵時上下移動
	bool enableDown;		//飛行時準備下降
	int justInvincibledTime;   //無敵結束經過時間
	int flash;              //無敵閃爍時間
	bool Ifslip;    //是否正在滑壘狀態
	int CountSlip;   //滑壘時間長短
	bool keyDown;    //是否按下來的
protected:
	CAnimation animation;		//人物跑
	CAnimation animation2;      //人物跳起
	CAnimation animation3;		//人物降落
	CAnimation animation4;		//人物無敵
	CAnimation animation5;		//人物翻滾
	CAnimation animation6;		//人物死亡
	bool isMovingDown;			// 是否正在往下移動
	bool isMovingLeft;			// 是否正在往左移動
	bool isMovingRight;			// 是否正在往右移動
	bool isMovingUp;			// 是否正在往上移動
};

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲開頭畫面物件
// 每個Member function的Implementation都要弄懂
/////////////////////////////////////////////////////////////////////////////

class CGameStateInit : public CGameState {
public:
	CGameStateInit(CGame *g);
	void OnInit();  								// 遊戲的初值及圖形設定
	void OnBeginState();							// 設定每次重玩所需的變數
	void OnKeyUp(UINT, UINT, UINT); 				// 處理鍵盤Up的動作
	int isLoadStart;
	int stage;
protected:
	void OnShow();									// 顯示這個狀態的遊戲畫面
private:
	CMovingBitmap startView1;
	CMovingBitmap startView2;
	CMovingBitmap startView3;
	CMovingBitmap startView4;
	CMovingBitmap startView5;
};

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
// 每個Member function的Implementation都要弄懂
/////////////////////////////////////////////////////////////////////////////

class CGameStateRun : public CGameState {
public:
	CGameStateRun(CGame *g);
	~CGameStateRun();
	void OnBeginState();							// 設定每次重玩所需的變數
	void OnInit();  								// 遊戲的初值及圖形設定
	void OnInitOver();									//每次遊戲Over初始化
	void OnKeyDown(UINT, UINT, UINT);
	void OnKeyUp(UINT, UINT, UINT);
	
	CMan			man;		// 人物
	CGameMap		gamemap;    // 地圖

	int				moveSpeed;
	int				HighScore;
	int				isLoadGame;
protected:
	void OnMove();									// 移動遊戲元素
	void OnShow();									// 顯示這個狀態的遊戲畫面
private:
	CMovingBitmap	background;	// 背景圖
	CMovingBitmap	BestScore;	// 最高分文字圖
	CMovingBitmap	YourScore;	// 獲得分數文字圖
	CMovingBitmap   YourStar;   //剩餘道具數量
	CInteger		score;	// 剩下的撞擊數
	int picX,picY;
};

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的結束狀態(Game Over)
// 每個Member function的Implementation都要弄懂
/////////////////////////////////////////////////////////////////////////////

class CGameStateOver : public CGameState {
public:
	CGameStateOver(CGame *g);
	void OnBeginState();							// 設定每次重玩所需的變數
	void OnInit();
	int isLoadOver;
	void OnKeyDown(UINT, UINT, UINT);
protected:
	void OnMove();									// 移動遊戲元素
	void OnShow();									// 顯示這個狀態的遊戲畫面
private:
	int counter;	// 倒數之計數器
	CAnimation		over;    //結束動畫
};

}