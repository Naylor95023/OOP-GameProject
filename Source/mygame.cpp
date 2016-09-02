/*
 * mygame.cpp: ���ɮ��x�C��������class��implementation
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
 * History:
 *   2002-03-04 V3.1
 *          Add codes to demostrate the use of CMovingBitmap::ShowBitmap(CMovingBitmap &).
 *	 2004-03-02 V4.0
 *      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
 *         demonstrate the use of states.
 *      2. Demo the use of CInteger in CGameStateRun.
 *   2005-09-13
 *      Rewrite the codes for CBall and CEraser.
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *      1. Add codes to display IDC_GAMECURSOR in GameStateRun.
 *   2006-02-08 V4.2
 *      1. Revise sample screens to display in English only.
 *      2. Add code in CGameStateInit to demo the use of PostQuitMessage().
 *      3. Rename OnInitialUpdate() -> OnInit().
 *      4. Fix the bug that OnBeginState() of GameStateInit is not called.
 *      5. Replace AUDIO_CANYON as AUDIO_NTUT.
 *      6. Add help bitmap to CGameStateRun.
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *   2006-12-30
 *      1. Bug fix: fix a memory leak problem by replacing PostQuitMessage(0) as
 *         PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE,0,0).
 *   2008-02-15 V4.4
 *      1. Add namespace game_framework.
 *      2. Replace the demonstration of animation as a new bouncing ball.
 *      3. Use ShowInitProgress(percent) to display loading progress. 
 *   2010-03-23 V4.6
 *      1. Demo MP3 support: use lake.mp3 to replace lake.wav.
*/

#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "mygame.h"
#include <fstream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
namespace game_framework {

CMan::CMan()
{
	Initialize();

}

int CMan::GetX1()
{
	return x;
}

int CMan::GetY1()
{
	return y;
}

int CMan::GetX2()
{
	return x + animation.Width();
}

int CMan::GetY2()
{
	return y + animation.Height();
}

void CMan::Initialize()
{
	const int INITIAL_VELOCITY = 18;	// ��l�W�ɳt��
	const int FLOOR = 385;				// �a�O�y��
	jumpSub=0;
	floor = FLOOR;
	x = 0; y = FLOOR-1;				// y�y�Ф�a�O��1�I(���b�a�O�W)
	rising = true;
	initial_velocity = INITIAL_VELOCITY;
	velocity = initial_velocity;
	die=false;
	Eat=false;
	animation.SetDelayCount(3);
	animation2.SetDelayCount(3);
	animation4.SetDelayCount(3);
	animation5.SetDelayCount(2);
	animation6.SetDelayCount(2);
	count=0;
	speed=5;
	flag=0;
	isMovingLeft = isMovingRight= isMovingUp = isMovingDown = false;
	HaveStarCount=1;
	EnableControlMan=1;
	CountInvincible=0;
	UseInvincible=false;
	MoveUp=16;
	MoveDown=16;
	enableDown=false;
	justInvincibledTime=0;
	flash=0;
	Ifslip=false;
	CountSlip=10;
	keyDown=false;
}

void CMan::LoadBitmap()
{
	animation.AddBitmap(IDB_a1,RGB(255,255,255)); //�]
	animation.AddBitmap(IDB_a2,RGB(255,255,255));
	animation.AddBitmap(IDB_a3,RGB(255,255,255));
	animation.AddBitmap(IDB_a4,RGB(255,255,255));
	animation.AddBitmap(IDB_a5,RGB(255,255,255));
	animation.AddBitmap(IDB_a6,RGB(255,255,255));
	animation.AddBitmap(IDB_a7,RGB(255,255,255));
	animation.AddBitmap(IDB_a8,RGB(255,255,255));
	animation.AddBitmap(IDB_a9,RGB(255,255,255));
	animation.AddBitmap(IDB_a10,RGB(255,255,255));

	animation2.AddBitmap(IDB_UP,RGB(255,255,255));	//��
	animation3.AddBitmap(IDB_DOWN,RGB(255,255,255));//��

	animation4.AddBitmap(IDB_SUPER0,RGB(255,255,255));//�L��
	animation4.AddBitmap(IDB_SUPER1,RGB(255,255,255));//�L��
	animation4.AddBitmap(IDB_SUPER2,RGB(255,255,255));//�L��
	animation4.AddBitmap(IDB_SUPER3,RGB(255,255,255));//�L��

	animation5.AddBitmap(IDB_SLIP0,RGB(255,255,255));	//�u
	animation5.AddBitmap(IDB_SLIP1,RGB(255,255,255));
	animation5.AddBitmap(IDB_SLIP2,RGB(255,255,255));
	animation5.AddBitmap(IDB_SLIP3,RGB(255,255,255));
	animation5.AddBitmap(IDB_SLIP4,RGB(255,255,255));
	animation5.AddBitmap(IDB_SLIP5,RGB(255,255,255));
	animation5.AddBitmap(IDB_SLIP6,RGB(255,255,255));
	animation5.AddBitmap(IDB_SLIP7,RGB(255,255,255));

	animation6.AddBitmap(IDB_DIE0,RGB(255,255,255));	//Over
	animation6.AddBitmap(IDB_DIE1,RGB(255,255,255));
	animation6.AddBitmap(IDB_DIE2,RGB(255,255,255));
	animation6.AddBitmap(IDB_DIE3,RGB(255,255,255));

	height=animation.Height();
	width=animation.Width();
}


void CMan::Invincible(){

	if(UseInvincible==true){
		if(CountInvincible==0){
			CAudio::Instance()->Play(AUDIO_Super,true);
			HaveStarCount--;
			justInvincibledTime=0;
		}
				if (MoveUp > 0 && enableDown==false) {
					y -= MoveUp;	// ��t�� > 0�ɡAy�b�W��(����MoveUp���I�AMoveUp����쬰 �I/��)
					MoveUp--;		// �����O�v�T�A�U�����W�ɳt�׭��C
					MoveDown=30;
					if(y<=240)y=240;
				} 
				else {
					enableDown=true;
				}
		CountInvincible++;
		EnableControlMan=0;
	}


}

void CMan::Invincibled(CGameMap* map){
	int i=0,k=0;
	if(CountInvincible>=165){
		for(i=x;i<=x+600;i++){
			if(map->map[x][415]==2)k++;
		}
		if(k==0){
				if (y <= floor-1 && enableDown==true) {  // ��y�y���٨S�I��a�O
					y += MoveDown;	// y�b�U��(����velocity���I�Avelocity����쬰 �I/��)
					MoveDown++;		// �����O�v�T�A�U�����U���t�׼W�[
					MoveUp=16;
				} 
		
				else{
					y=floor;
					enableDown=false;
					UseInvincible=false;
					CountInvincible=0;
					EnableControlMan=1;
					CAudio::Instance()->Stop(AUDIO_Super);
		       }
		}
	}
}

void CMan:: LiveDie(CGameMap* map){
	int rx=0,ry=0;
	int lx=x+5,ly=y+height-6;
	if (isMovingUp){
		jumpSub=5;
	}
	rx=x+width-10-jumpSub;
	ry=y+height-6;
	
	if(justInvincibledTime>=100){
		if(map->map[lx][ly]==2||map->map[rx][ry]==2||map->map[rx][ry]==7||map->map[rx][ry]==8||map->map[rx][ry]==9||map->map[rx][ry]==10||map->map[lx][ly]==7||map->map[lx][ly]==8||map->map[lx][ly]==9||map->map[lx][ly]==10){
			CAudio::Instance()->Play(AUDIO_Die,false);
			die=true;
		justInvincibledTime=0;
		}
		if((map->map[x+10][y]==7||map->map[x+10][y]==8||map->map[x+width-10][y]==7||map->map[x+width-10][y]==8||map->map[x+10][y]==9||map->map[x+10][y]==10||map->map[x+width-10][y]==9||map->map[x+width-10][y]==10 )&& UseInvincible==false){
			CAudio::Instance()->Play(AUDIO_Die,false);
			die=true;
		justInvincibledTime=0;
		}
	}
	jumpSub=0;
}

void CMan::ManJump(CGameMap* map){
	const int floor = 385;
	int down=0;
	if(EnableControlMan==1){
	justInvincibledTime++;
	if (isMovingUp){
		flag=1;
	}
		if(isMovingDown){rising = false;down=30;}
		if(flag==1){
			if (rising) {			// �W�ɪ��A
				if (velocity > 0) {
					y -= velocity;	// ��t�� > 0�ɡAy�b�W��(����velocity���I�Avelocity����쬰 �I/��)
					velocity--;		// �����O�v�T�A�U�����W�ɳt�׭��C
				} else {
					rising = false; // ��t�� <= 0�A�W�ɲפ�A�U���אּ�U��
					velocity = 5;	// �U������t(velocity)��1
				}
			} else{				// �U�����A
				if (y < floor-1) {  // ��y�y���٨S�I��a�O
					y += (velocity+down);	// y�b�U��(����velocity���I�Avelocity����쬰 �I/��)
					velocity+=2;		// �����O�v�T�A�U�����U���T�׼W�[
					down=0;
				} else {
					y = floor -1 ;  // ��y�y�ЧC��a�O�A�󥿬��a�O�W
					if(CountSlip<=9999999999&&keyDown){
						y=floor+10;
						Ifslip=true;
						CountSlip++;
					}
					else{
						y = floor -1 ;
						Ifslip=false;
						CountSlip=0;
						rising = true;	// �����ϼu�A�U���אּ�W��
						velocity = initial_velocity; // ���]�W�ɪ�l�t��
						flag=0;
					}
				}
			}
		}

			else {		
				if(CountSlip<=9999999999&&keyDown){
						y=floor+15;
						Ifslip=true;
						CountSlip++;
				}
				else{
					y = floor -1 ;
					Ifslip=false;
					CountSlip=0;
					rising = true;	// �����ϼu�A�U���אּ�W��
					velocity = initial_velocity; // ���]�W�ɪ�l�t��
					flag=0;
				}
		}
}
}

void CMan:: GetItem(CGameMap* map){
	int i=0,j=0,l=0,m=0;
	int topRightX=x+width;
	int ry=y+height-6;
for(i=x;i<=topRightX;i++){
				for(j=y;j<=ry;j++){
					if(Eat==false){
						if(map->map[i][j]==6){
							for(l=i-40;l<=i+40;l++)
								for(m=j-40;m<=j+40;m++)
								if(map->map[l][m]==5){
									map->map[l][m]=0;
									if(HaveStarCount>=3)HaveStarCount=3;//�W���T��
									else HaveStarCount++;
									break;
								}
							Eat=true;
						}
						if(map->map[i][j]==4){
							for(l=i-50;l<=i+50;l++)
								for(m=j-50;m<=j+50;m++)
								if(map->map[l][m]==3){
									map->map[l][m]=0;
									break;
								}
							Eat=true;
						}
						if(Eat==true){
							CAudio::Instance()->Play(AUDIO_Ding, false);

						}
					}
				}
		}
}

void CMan::OnMove(CGameMap* map)
{
	const int floor = 385;
	jumpSub=0;
	die=false;

	Invincible();
	Invincibled(map);
	LiveDie(map);
	GetItem(map);
	ManJump(map);

	animation.OnMove();
	animation2.OnMove();
	animation3.OnMove();
	animation4.OnMove();
	animation5.OnMove();
}
void CMan::SetMovingDown(bool flag)
{
	isMovingDown = flag;
}

void CMan::SetMovingLeft(bool flag)
{
	isMovingLeft = flag;
}

void CMan::SetMovingRight(bool flag)
{
	isMovingRight = flag;
}

void CMan::SetMovingUp(bool flag)
{
	isMovingUp = flag;
}

void CMan::SetXY(int nx, int ny)
{
	x = nx; y = ny;
}

void CMan::OnShow(CGameMap* map)
{

	int	sx=map->screenx(x);
	int	sy=map->screeny(y);
	if(sx==15)x=15;

	animation.SetTopLeft(sx,sy);
	animation2.SetTopLeft(sx,sy);
	animation3.SetTopLeft(sx,sy);
	animation4.SetTopLeft(sx,sy);
	animation5.SetTopLeft(sx,sy);
	animation6.SetTopLeft(sx,sy);
	if(CountInvincible!=0)animation4.OnShow();
	else if(die)animation6.OnShow();
	else if(Ifslip){		
		if(justInvincibledTime<100){
			flash++;
			if(flash>=2){
				animation5.OnShow();
				flash=0;
			}
		}
		else animation5.OnShow();}

	else if(flag&&rising){
		if(justInvincibledTime<100){
			flash++;
			if(flash>=2){
				animation2.OnShow();
				flash=0;
			}
		}
		else animation2.OnShow();
	}
	else if(flag&&(!rising)){
		if(justInvincibledTime<100){
			flash++;
			if(flash>=2){
				animation3.OnShow();
				flash=0;
			}
		}
		else animation3.OnShow();
		
	}
	else {
		if(justInvincibledTime<100){
			flash++;
			if(flash>=2){
				animation.OnShow();
				flash=0;
			}
		}
		else animation.OnShow();
	}
}

/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����C���}�Y�e������
/////////////////////////////////////////////////////////////////////////////

CGameStateInit::CGameStateInit(CGame *g)
: CGameState(g)
{

}

void CGameStateInit::OnInit()
{
	ShowInitProgress(0);	// �@�}�l��loading�i�׬�0%
	//
	// �}�l���J���
	//
	startView1.LoadBitmap(IDB_STARTVIEW1);
	startView2.LoadBitmap(IDB_STARTVIEW2);
	startView3.LoadBitmap(IDB_STARTVIEW3);
	startView4.LoadBitmap(IDB_STARTVIEW4);
	startView5.LoadBitmap(IDB_STARTVIEW5);
		CAudio::Instance()->Load(AUDIO_MenuMove,  "sounds\\MOVE.mp3");
		CAudio::Instance()->Load(AUDIO_MenuSelect,  "sounds\\SELECT.mp3");

	stage=1;
	
}

void CGameStateInit::OnBeginState()
{
	if(isLoadStart!=99999999){
		CAudio::Instance()->Load(AUDIO_Start,  "sounds\\start.mp3");
		isLoadStart=99999999;
	}
	CAudio::Instance()->Play(AUDIO_Start, true);
}

void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char KEY_UP    = 0x26; // keyboard�W�b�Y
	const char KEY_DOWN  = 0x28; // keyboard�U�b�Y
	const char KEY_ESC = 27;
	const char KEY_SPACE = ' ';
	
	if (nChar == KEY_SPACE && stage==3){			//�}�l�C��
		CAudio::Instance()->Stop(AUDIO_Start);
		CAudio::Instance()->Play(AUDIO_MenuSelect, false);	
		GotoGameState(GAME_STATE_RUN);	// ������GAME_STATE_RUN
	}					
	else if(nChar == KEY_SPACE && stage==4){		//�����k�s
		CAudio::Instance()->Play(AUDIO_MenuSelect, false);
		int temp=0;
			char filename[]="Source\\score.txt";
			fstream fp;
			fp.open(filename, ios::out);
			fp<<temp<<endl;//�g�J�r��
			fp.close();//�����ɮ�
	}

	else if (nChar == KEY_SPACE && stage==5){
		CAudio::Instance()->Play(AUDIO_MenuSelect, false);
		PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE,0,0);	// �����C��
	
	}
	if(stage>=1&&stage<=2){
		if (nChar == KEY_SPACE){
			CAudio::Instance()->Play(AUDIO_MenuSelect, false);
			stage++;
		}
	}

	if (nChar == KEY_UP && stage>=3){
		if(stage>3){
			CAudio::Instance()->Play(AUDIO_MenuMove, false);
			stage--;
		}
		else {
			CAudio::Instance()->Play(AUDIO_MenuMove, false);
			stage=5;
		}
	}
	if (nChar == KEY_DOWN && stage>=3){
		if(stage<5){
			CAudio::Instance()->Play(AUDIO_MenuMove, false);
			stage++;
		}
		else {
			CAudio::Instance()->Play(AUDIO_MenuMove, false);
			stage=3;
		}
	}

}

void CGameStateInit::OnShow()
{

	switch(stage){
		case 1:
			startView1.SetTopLeft(0,0);    //�K�W�}�l�Ϥ�
			startView1.ShowBitmap();
			break;
		case 2:
			startView2.SetTopLeft(0,0);    //�K�W�}�l�Ϥ�
			startView2.ShowBitmap();
			break;
		case 3:
			startView3.SetTopLeft(0,0);    //�K�W�}�l�Ϥ�
			startView3.ShowBitmap();
			break;
		case 4:
			startView4.SetTopLeft(0,0);    //�K�W�}�l�Ϥ�
			startView4.ShowBitmap();
			break;
		case 5:
			startView5.SetTopLeft(0,0);    //�K�W�}�l�Ϥ�
			startView5.ShowBitmap();
			break;
		default: 
			break;

	}

}								

/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����������A(Game Over)
/////////////////////////////////////////////////////////////////////////////

CGameStateOver::CGameStateOver(CGame *g)
: CGameState(g)
{
}
void CGameStateOver::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char KEY_Enter =0x0D; 
	if(nChar==KEY_Enter) {
		CAudio::Instance()->Stop(1);
		GotoGameState(GAME_STATE_INIT);
	}

}
void CGameStateOver::OnMove()
{
	over.OnMove();
	counter--;
	if (counter < 0)
		GotoGameState(GAME_STATE_INIT);
}

void CGameStateOver::OnBeginState()
{
	if(isLoadOver!=99999999){
		CAudio::Instance()->Load(AUDIO_Over,  "sounds\\over.mp3");	// ���J�s��1���n��lake.mp3
		isLoadOver=99999999;
	}
	CAudio::Instance()->Play(AUDIO_Over, false);			// ���� over.mp3
	counter = 30 * 5; // 5 seconds

}

void CGameStateOver::OnInit()
{
	ShowInitProgress(66);	// ���ӫe�@�Ӫ��A���i�סA���B�i�׵���66%
	over.AddBitmap(IDB_OVER0,RGB(255,255,255));
	over.AddBitmap(IDB_OVER1,RGB(255,255,255));
	over.AddBitmap(IDB_OVER2,RGB(255,255,255));
	over.AddBitmap(IDB_OVER3,RGB(255,255,255));
	over.SetDelayCount(10);
	ShowInitProgress(100);
}


void CGameStateOver::OnShow()
{
	over.SetTopLeft(0,0);
	over.OnShow();
}

/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����C�����檫��A�D�n���C���{�����b�o��
/////////////////////////////////////////////////////////////////////////////

CGameStateRun::CGameStateRun(CGame *g)
: CGameState(g)
{

}

CGameStateRun::~CGameStateRun()
{

}

void CGameStateRun::OnBeginState()
{
	const int BACKGROUND_X = 0;
	char str[100];
	int temp;
	char filename[]="Source\\score.txt";
		fstream fp;
		fp.open(filename, ios::in);
		fp.getline(str,sizeof(str));
		temp=atoi(str);
		score.SetInteger(temp);
		fp.close();//�����ɮ�
		HighScore=temp;
	if(isLoadGame!=99999999){
		CAudio::Instance()->Load(AUDIO_Game,  "sounds\\game.mp3");
		CAudio::Instance()->Load(AUDIO_Ding,  "sounds\\ding.mp3");
		CAudio::Instance()->Load(AUDIO_Super,  "sounds\\super.wav");
		CAudio::Instance()->Load(AUDIO_Die,  "sounds\\die.mp3");

		isLoadGame=99999999;
	}
	CAudio::Instance()->Play(AUDIO_Game, true);	

	gamemap.Initialize();
	moveSpeed=0;
	man.Initialize();
	background.SetTopLeft(BACKGROUND_X,0);				// �]�w�I�����_�l�y��
	score.SetInteger(0);					// ���w�ѤU��������
	score.SetTopLeft(480,0);		// ���Ʀ�m
	BestScore.SetTopLeft(0,-5);
	YourScore.SetTopLeft(70,180);

}

void CGameStateRun::OnMove()							// ���ʹC������
{
	int bonus=100;
	if(man.CountInvincible!=0)score.Add(5);
	if(man.die==false){
		score.Add(1);
		gamemap.OnMove(&man);
		man.OnMove(&gamemap);
		if(man.Eat==true){
			score.Add(bonus);
			man.Eat=false;
		}

	}
	// �p�G�Ʊ�ק�cursor���˦��A�h�N�U���{����commment�����Y�i
	//SetCursor(AfxGetApp()->LoadCursor(IDC_GAMECURSOR));
	if(man.die==true){

		int temp=score.GetInteger();
		if(temp>HighScore){
			char filename[]="Source\\score.txt";
			fstream fp;
			fp.open(filename, ios::out);
			fp<<temp<<endl;//�g�J�r��
			fp.close();//�����ɮ�
		}
		CAudio::Instance()->Stop(0);
		CAudio::Instance()->Stop(2);
		man.count++;	

		score.SetTopLeft(260,200);
		score.ShowBitmap();

		if(man.count>=100){
			OnInitOver();
			GotoGameState(GAME_STATE_OVER);
		}
	}

}
void CGameStateRun::OnInit()  								// �C������Ȥιϧγ]�w
{
	ShowInitProgress(33);	// ���ӫe�@�Ӫ��A���i�סA���B�i�׵���33%
	gamemap.LoadBitmap();
	man.LoadBitmap();
	background.LoadBitmap(IDB_BACKRUN);					// ���J�I�����ϧ�
	BestScore.LoadBitmap(IDB_BEST,RGB(255,255,255));
	YourScore.LoadBitmap(IDB_YOURSCORE,RGB(255,255,255));
	YourStar.LoadBitmap(IDB_STAR,RGB(255,255,255));
	ShowInitProgress(50);
	score.LoadBitmap();	
}

void CGameStateRun::OnInitOver(){

	const int BACKGROUND_X = 0;
	char str[100];
	int temp;
	char filename[]="Source\\score.txt";
		fstream fp;
		fp.open(filename, ios::in);
		fp.getline(str,sizeof(str));
		temp=atoi(str);
		score.SetInteger(temp);
		fp.close();//�����ɮ�
		HighScore=temp;

	gamemap.Initialize();
	moveSpeed=0;
	man.Initialize();
	background.SetTopLeft(BACKGROUND_X,0);				// �]�w�I�����_�l�y��
	score.SetInteger(0);					// ���w�ѤU��������
	score.SetTopLeft(480,0);		// ���w�ѤU�����ƪ��y��
	BestScore.SetTopLeft(0,-5);
	YourScore.SetTopLeft(70,180);

}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char KEY_UP    = 0x26; // keyboard�W�b�Y
	const char KEY_DOWN  = 0x28; // keyboard�U�b�Y
	const char KEY_Z  = 0x5A; // keyboard  Z
	const char KEY_X  = 0x58; // keyboard  X
	const char KEY_C  = 0x43; // keyboard  C

	if (nChar == KEY_UP)
		man.SetMovingUp(true);
	if (nChar == KEY_DOWN){
		man.keyDown=true;
		man.SetMovingDown(true);
	}
	if (nChar == KEY_DOWN){
		man.keyDown=true;
		man.SetMovingDown(true);
	}
	if(nChar==KEY_Z)         //�ϥεL�ĹD��
		if(man.HaveStarCount>0){
			man.UseInvincible=true;
	}
	if (nChar == KEY_X){		//�K�� �L�ĤT��
		man.justInvincibledTime=0;
	}
	if (nChar == KEY_C){		//�K�� �֦��D���ܦ�3��
		man.HaveStarCount=3;
	}

}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{	
	const char KEY_UP    = 0x26; // keyboard�W�b�Y
	const char KEY_DOWN  = 0x28; // keyboard�U�b�Y

	if (nChar == KEY_UP)
		man.SetMovingUp(false);
	if (nChar == KEY_DOWN){
		man.keyDown=false;
		man.SetMovingDown(false);
	}
}

void CGameStateRun::OnShow()
{
	int temp=score.GetInteger();
	gamemap.OnShow();
	switch(man.HaveStarCount){
		case 3://�T��
			YourStar.SetTopLeft(100,50);//3
			YourStar.ShowBitmap();
			YourStar.SetTopLeft(50,50);//2
			YourStar.ShowBitmap();
			YourStar.SetTopLeft(0,50);//1
			YourStar.ShowBitmap();
			break;
		case 2://���
			YourStar.SetTopLeft(50,50);//2
			YourStar.ShowBitmap();
			YourStar.SetTopLeft(0,50);//1
			YourStar.ShowBitmap();
			break;
		case 1://�@��
			YourStar.SetTopLeft(0,50);//1
			YourStar.ShowBitmap();
			break;
		default: 
			break;
	}

	BestScore.ShowBitmap();
	score.ShowBitmap();
	score.SetTopLeft(70,0);
	score.SetInteger(HighScore);

	score.ShowBitmap();
	score.SetInteger(temp);
	score.SetTopLeft(480,0);
	if(man.die==true)	{	
		YourScore.ShowBitmap();
	}
	man.OnShow(&gamemap);
}

void CGameMap::Initialize()
{
	X=15;
	Y=0;
	speed=old_speed=8;
	count=0;
	first=false;
	int k=240;
	int temp=0;
	int r=0,gset=0;
	int r1=0;
	for(int i=0;i<415;i++)
		for(int j=0;j<8000;j++){
			map[j][i]=0;
		}
	for(int i=0;i<10000;i++)
		for(int j=415;j<600;j++){	
		map[i][j]=1;
		}

	while(1){							//�a�O����
		r1=((rand()%9100)+1);			//�������Ͷ}�l��m
		r=((rand()%60)+1);				//�����e��
		if(r1<800)continue;
		if(r1%100!=0)continue;
		if(r<20)continue;
			for(int i=r1;i<=r1+r;i++){
				for(int j=415;j<600;j++){
					map[i][j]=2;
				}
			}
			gset++;
			if(gset>=18)break;
	}
	gset=0;

			while(1){       //�����_�c
				r1=((rand()%9001)+1);//1~7000
					if(r1%100!=0)continue;
					if(r1<800)continue;
						map[r1][k]=3;
							for(int i=r1+1;i<=r1+50;i++)
								for(int j=k+1;j<=k+40;j++)
								map[i][j]=4;
				gset++;
				if(gset>10)break;//100��
			}
			gset=0;


		while(1){							//����
				r1=((rand()%9100)+1);			//�������Ͷ}�l��m
				r=rand()%2;                    // 0��1  ��س���
			if(gset>=8)break;
			if(r1<800)continue;
			if(r1%100!=0)continue;
				for(int j=-300;j<300;j++){
					for(int i=0;i<600;i++){
						if(map[r1+j][i]==2||map[r1+j][i]==9||map[r1+j][i]==10){
						temp++;
						}
					}
				}	
			if(temp!=0){
				temp=0;
				continue;
			}
			if(r==0){
				if((r1/100)%2==0)temp=50;
				else temp=0;
					for(int i=240+temp;i<=340;i+=50){
						map[r1][i]=7;
						for(int j=r1+1;j<=r1+49;j++){
							for(int l=i+1;l<i+49;l++){
							map[j][l]=8;
							}
						}	
					}		
			}
			else if(r==1){
					int i=340;
						map[r1][i]=9;
						for(int j=r1+1;j<=r1+49;j++){
							for(int l=i+1;l<i+49;l++){
							map[j][l]=10;
							}
						}	
			}
			gset++;
		}
}
void CGameMap::MapInitialize(){
	int k=240;
	int temp=0;
	int r=0,r1=0,gset=0;
 	int i,j;
	for(i=0;i<10000;i++){
		for(j=0;j<600;j++){
			if(map[i][j]==2||map[i][j]==3||map[i][j]==4||map[i][j]==5||map[i][j]==6||map[i][j]==7||map[i][j]==8||map[i][j]==9||map[i][j]==10){
			map[i][j]=0;
			}
		}
	}
	
	while(1){							//�a�O����
		r1=((rand()%9000)+1);			//�������Ͷ}�l��m
		r=((rand()%60)+1);				//�����e��
		if(r1<800)continue;
		if(r<20)continue;
			for(int i=r1;i<=r1+r;i++){
				for(int j=415;j<600;j++){
					map[i][j]=2;
				}
			}
			gset++;
			if(gset>=18)break;
	}
	gset=0;


	while(1){         //�X�{�_�c
		r=((rand()%9000)+1);//1~7000
			if(r%100!=0)continue;
			if(r<800)continue;
		map[r][k]=3;
		for(int i=r+1;i<=r+50;i++)
			for(int j=k+1;j<=k+40;j++)
				map[i][j]=4;
		gset++;
		if(gset>10)break;//10��
	}
	gset=0;
	while(1){	//����
				r1=((rand()%9000)+1);			//�������Ͷ}�l��m
				r=rand()%2;                    // 0��1  ��س���
			if(gset>=8)break;
			if(r1<800)continue;
			if(r1%100!=0)continue;
				for(int j=-300;j<300;j++){
					for(int i=0;i<600;i++){
						if(map[r1+j][i]==2||map[r1+j][i]==9||map[r1+j][i]==10){
						temp++;
						}
					}	
				}	
			if(temp!=0){
				temp=0;
				continue;
			}
			if(r==0){
				if((r1/100)%2==0)temp=50;
				else temp=0;
					for(int i=240+temp;i<=340;i+=50){
						map[r1][i]=7;
						for(int j=r1+1;j<=r1+49;j++){
							for(int l=i+1;l<i+49;l++){
							map[j][l]=8;
							}
						}	
					}		
			}
			else if(r==1){
					int i=340;
						map[r1][i]=9;
						for(int j=r1+1;j<=r1+49;j++){
							for(int l=i+1;l<i+49;l++){
							map[j][l]=10;
							}
						}	
			}
			gset++;
	}
	r=(rand()%2);
	r1=(rand()%2);
	if(r==r1)         //�X�{�L�ĹD��
	while(1){
			r=((rand()%9000)+1);//1~7000
			if(r%100!=0)continue;
			if(r<800)continue;
		map[r][k]=5;
		for(int i=r+1;i<=r+40;i++)
			for(int j=k+1;j<=k+40;j++)
				map[i][j]=6;
		break;
	}
}


void CGameMap::OnKeyDown(UINT nChar)
{
	const int KEY_SPACE= 0x20;

}

void CGameMap::OnMove(CMan* man)
{
	count++;
	if(man->CountInvincible!=0){
		if(first==false){
		old_speed=speed;
		speed=35;
		first=true;
		}
	}
	else {
	speed=old_speed;
	first=false;
	} 
	if(count>50 && man->CountInvincible==0){
		if(speed<18)speed++;
		old_speed=speed;
		count=0;
	}
	man->x=man->x+speed;
	X+=speed;

	background.SetTopLeft(-X,0);

	if(X>9800){X=15;man->x=15;MapInitialize();}


}

CGameMap::~CGameMap()
{
}

void CGameMap::LoadBitmap()
{
	fire.LoadBitmap(IDB_FIRE);
	background.LoadBitmap(IDB_BACKRUN);
	gold.LoadBitmap(IDB_GOLD,RGB(255,255,255));
	star.LoadBitmap(IDB_STAR,RGB(255,255,255));
	roadblock.LoadBitmap(IDB_ROADBLOCK,RGB(255,255,255));
	roadblock1.LoadBitmap(IDB_ROADBLOCK1,RGB(255,255,255));
}
int CGameMap::screenx(int x){
	if(X==15)return 15;
	else return x-X;
}
int CGameMap::screeny(int y){
	 return y-Y;
}

void CGameMap::OnShow()
{
	//background.ShowBitmap();
	for(int i=0;i<10000;i+=10)
		for(int j=0;j<600;j+=10)
		{
			switch(map[i][j]){
			case 2:     //2=�a�O����
				fire.SetTopLeft(i-X,j);
				fire.ShowBitmap();
				break;
			case 3:     //3.4=�_�c
				gold.SetTopLeft(i-X,j);
				gold.ShowBitmap();
				break;
			case 5:		//5.6�L�ĹD�� �P�P
				star.SetTopLeft(i-X,j);
				star.ShowBitmap();
				break;
			case 7:		//7.8����
				roadblock.SetTopLeft(i-X,j);
				roadblock.ShowBitmap();
				break;

			case 9:		//9.10����
				roadblock1.SetTopLeft(i-X,j);
				roadblock1.ShowBitmap();
				break;
		}
	}

}
}