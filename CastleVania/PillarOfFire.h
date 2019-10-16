#pragma once

#include "GameObject.h"

#define BRICK_BBOX_WIDTH  20
#define BRICK_BBOX_HEIGHT 40

class CPillarFire: public CGameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};