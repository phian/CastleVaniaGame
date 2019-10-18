#include "Scene.h"

void CScene::Render()
{
	animations[0]->Render(x, y);
	RenderBoundingBox();
}

void CScene::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + SCENE_BBOX_WIDTH;
	b = y + SCENE_BBOX_HEIGHT;
}