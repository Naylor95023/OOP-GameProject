/*
 * mygame.h: ���ɮ��x�C��������class��interface
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

enum AUDIO_ID {				// �w�q�U�ح��Ī��s��
	AUDIO_Start,			// 0
	AUDIO_Over,				// 1
	AUDIO_Game,				// 2
	AUDIO_Ding,              //�Y��������
	AUDIO_Super,				//�Q�g����
	AUDIO_MenuMove,				//��� ���ʭ���
	AUDIO_MenuSelect,			//���T�{����
	AUDIO_Die                   //���`����
};

namespace game_framework {
/////////////////////////////////////////////////////////////////////////////
// �o��class���ѥi�H����L�ηƹ�������l
// �����N�i�H��g���ۤv���{���F
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
	int map[10000][600];   //�y��
	int X,Y;
	int speed;
	int old_speed;				//���ܳt�׫e���O���쥻�t��
	bool first;					//�O�_�Ĥ@���x�sspeed
	int count;
protected:
	CMovingBitmap back;   //�I���ʵe
	CMovingBitmap fire,gold,star;
	CMovingBitmap roadblock;  //����
	CMovingBitmap roadblock1;  //����1
	CMovingBitmap background;
};
class CMan               //�H��
{
public:
	CMan();
	int  GetX1();					// �H�����W�� x �y��
	int  GetY1();					// �H�����W�� y �y��
	int  GetX2();					// �H���k�U�� x �y��
	int  GetY2();					// �H���k�U�� y �y��
	void Initialize();				// �]�w�H������l��
	void LoadBitmap();				// ���J�ϧ�
	void OnMove(CGameMap*);					// ���ʤH��
	void OnShow(CGameMap*);					// �N�H���ϧζK��e��
	void SetMovingDown(bool flag);	// �]�w�O�_���b���U����
	void SetMovingLeft(bool flag);	// �]�w�O�_���b��������
	void SetMovingRight(bool flag); // �]�w�O�_���b���k����
	void SetMovingUp(bool flag);	// �]�w�O�_���b���W����
	void SetXY(int nx, int ny);		// �]�w�H�����W���y��
	void Invincible();				//�ϥιD��
	void Invincibled(CGameMap*);	//�D��ĪG����
	void LiveDie(CGameMap*);		//�P�_�O�_�I�쳴��
	void GetItem(CGameMap*);		//�O�_�Y�D�D��
	void ManJump(CGameMap*);		//����H�����D
	int velocity;
	int initial_velocity;
	int flag;
	bool rising;
	int height,width;
	int floor;
	bool die;                  //�O�_over
	bool Eat;
	int count;
	int speed;
	int x, y;					// �H�����W���y��
	int jumpSub;			
	int HaveStarCount;		//�L�ĹD��Ӽ�
	int EnableControlMan;   //�O�_�i�H����H��
	bool UseInvincible;    //�ϥεL��
	int CountInvincible;   //�L�Įɶ�
	int MoveUp,MoveDown;    //�L�ĮɤW�U����
	bool enableDown;		//����ɷǳƤU��
	int justInvincibledTime;   //�L�ĵ����g�L�ɶ�
	int flash;              //�L�İ{�{�ɶ�
	bool Ifslip;    //�O�_���b���S���A
	int CountSlip;   //���S�ɶ����u
	bool keyDown;    //�O�_���U�Ӫ�
protected:
	CAnimation animation;		//�H���]
	CAnimation animation2;      //�H�����_
	CAnimation animation3;		//�H������
	CAnimation animation4;		//�H���L��
	CAnimation animation5;		//�H��½�u
	CAnimation animation6;		//�H�����`
	bool isMovingDown;			// �O�_���b���U����
	bool isMovingLeft;			// �O�_���b��������
	bool isMovingRight;			// �O�_���b���k����
	bool isMovingUp;			// �O�_���b���W����
};

/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����C���}�Y�e������
// �C��Member function��Implementation���n����
/////////////////////////////////////////////////////////////////////////////

class CGameStateInit : public CGameState {
public:
	CGameStateInit(CGame *g);
	void OnInit();  								// �C������Ȥιϧγ]�w
	void OnBeginState();							// �]�w�C�������һݪ��ܼ�
	void OnKeyUp(UINT, UINT, UINT); 				// �B�z��LUp���ʧ@
	int isLoadStart;
	int stage;
protected:
	void OnShow();									// ��ܳo�Ӫ��A���C���e��
private:
	CMovingBitmap startView1;
	CMovingBitmap startView2;
	CMovingBitmap startView3;
	CMovingBitmap startView4;
	CMovingBitmap startView5;
};

/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����C�����檫��A�D�n���C���{�����b�o��
// �C��Member function��Implementation���n����
/////////////////////////////////////////////////////////////////////////////

class CGameStateRun : public CGameState {
public:
	CGameStateRun(CGame *g);
	~CGameStateRun();
	void OnBeginState();							// �]�w�C�������һݪ��ܼ�
	void OnInit();  								// �C������Ȥιϧγ]�w
	void OnInitOver();									//�C���C��Over��l��
	void OnKeyDown(UINT, UINT, UINT);
	void OnKeyUp(UINT, UINT, UINT);
	
	CMan			man;		// �H��
	CGameMap		gamemap;    // �a��

	int				moveSpeed;
	int				HighScore;
	int				isLoadGame;
protected:
	void OnMove();									// ���ʹC������
	void OnShow();									// ��ܳo�Ӫ��A���C���e��
private:
	CMovingBitmap	background;	// �I����
	CMovingBitmap	BestScore;	// �̰�����r��
	CMovingBitmap	YourScore;	// ��o���Ƥ�r��
	CMovingBitmap   YourStar;   //�Ѿl�D��ƶq
	CInteger		score;	// �ѤU��������
	int picX,picY;
};

/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����������A(Game Over)
// �C��Member function��Implementation���n����
/////////////////////////////////////////////////////////////////////////////

class CGameStateOver : public CGameState {
public:
	CGameStateOver(CGame *g);
	void OnBeginState();							// �]�w�C�������һݪ��ܼ�
	void OnInit();
	int isLoadOver;
	void OnKeyDown(UINT, UINT, UINT);
protected:
	void OnMove();									// ���ʹC������
	void OnShow();									// ��ܳo�Ӫ��A���C���e��
private:
	int counter;	// �˼Ƥ��p�ƾ�
	CAnimation		over;    //�����ʵe
};

}