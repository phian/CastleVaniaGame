#pragma once
#include "GameObject.h"

#define SIMON_WALKING_SPEED		0.1f 
//0.1f
#define SIMON_JUMP_SPEED_Y		0.5f
#define SIMON_JUMP_DEFLECT_SPEED 0.2f
#define SIMON_GRAVITY			0.002f
//#define MARIO_DIE_DEFLECT_SPEED	 0.5f

#define SIMON_STATE_IDLE			0
#define SIMON_STATE_WALKING_RIGHT	100
#define SIMON_STATE_WALKING_LEFT	200
#define SIMON_STATE_JUMP			300
#define SIMON_STATE_SITDOWN			400
#define SIMON_STATE_USE_WHIP_SIT	500
#define SIMON_STATE_USE_WHIP_STAND	600
//#define MARIO_STATE_DIE				400

#define SIMON_ANI_IDLE_RIGHT			0
#define SIMON_ANI_IDLE_LEFT				1
#define SIMON_ANI_SITDOWN_RIGHT			2
#define SIMON_ANI_SITDOWN_LEFT			3
#define SIMON_ANI_WALKING_RIGHT			4
#define SIMON_ANI_WALKING_LEFT			5
#define SIMON_ANI_USE_WHIP_SIT_RIGHT	6
#define SIMON_ANI_USE_WHIP_SIT_LEFT		7
#define SIMON_ANI_USE_WHIP_STAND_RIGHT	8
#define SIMON_ANI_USE_WHIP_STAND_LEFT	9
#define SIMON_ANI_JUMP_RIGHT			10
#define SIMON_ANI_JUMP_LEFT				11
//#define MARIO_ANI_DIE				8

//#define	MARIO_LEVEL_SMALL	1
//#define	MARIO_LEVEL_BIG		2

#define SIMON_BBOX_WIDTH  15
#define SIMOB_BBOX_HEIGHT 27

//#define MARIO_SMALL_BBOX_WIDTH  13
//#define MARIO_SMALL_BBOX_HEIGHT 15

#define MARIO_UNTOUCHABLE_TIME 5000


class CSimon : public CGameObject
{
	int level;
	int untouchable;
	DWORD untouchable_start;
public:
	CSimon() : CGameObject()
	{
		//level = MARIO_LEVEL_BIG;
		untouchable = 0;
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	//void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};