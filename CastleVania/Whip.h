#pragma once

#include "GameObject.h"

#define WHIP_BBOX_WIDTH  20
#define WHIP_BBOX_HEIGHT 40

#define WHIP_STATE_USE_RIGHT	100
#define WHIP_STATE_USE_LEFT		200

#define WHIP_ANI_USE_RIGHT	0
#define WHIP_ANI_USE_LEFT	1

class CWhip : public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
public:
	virtual void SetState(int state);
	virtual void Render();
};