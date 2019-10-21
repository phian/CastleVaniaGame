#pragma once
#include "GameObject.h"

#define KNIFE_MOVING_SPEED 0.2f;

#define KNIFE_BBOX_WIDTH 16
#define KNIFE_BBOX_HEIGHT 15
//#define GOOMBA_BBOX_HEIGHT_DIE 9

#define KNIFE_STATE_MOVING_RIGHT 100
#define KNIFE_STATE_MOVING_LEFT	 200

#define KNIFE_ANI_MOVING_RIGHT 0
#define KNIFE_ANI_MOVING_LEFT  1
//#define GOOMBA_ANI_DIE 1

class CKnife : public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
public:
	virtual void SetState(int state);
};