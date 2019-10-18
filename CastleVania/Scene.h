#pragma once

#include "GameObject.h"

#define SCENE_BBOX_WIDTH  600
#define SCENE_BBOX_HEIGHT 480

class CScene : public CGameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};