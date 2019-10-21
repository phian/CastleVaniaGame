#include "Knife.h"

void CKnife::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + KNIFE_BBOX_WIDTH;

	/*if (state == GOOMBA_STATE_DIE)
		bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
	else*/
	bottom = y + KNIFE_BBOX_HEIGHT;
}

void CKnife::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 

	x += dx;
	//y += dy;

	/*if (vx < 0 && x < 0) {
		x = 0; vx = -vx;
	}

	if (vx > 0 && x > 1465) {
		x = 290; vx = -vx;
	}*/
}

void CKnife::Render()
{
	int ani;
	/*if (state == GOOMBA_STATE_DIE) {
		ani = GOOMBA_ANI_DIE;
	}*/

	if (vx > 0) ani = KNIFE_ANI_MOVING_RIGHT;
	else ani = KNIFE_ANI_MOVING_LEFT;

	animations[ani]->Render(x, y);
	//RenderBoundingBox();
}

void CKnife::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	/*case GOOMBA_STATE_DIE:
		y += GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		break;*/
	case KNIFE_STATE_MOVING_RIGHT:
		vx = KNIFE_MOVING_SPEED;
		break;
	case KNIFE_STATE_MOVING_LEFT:
		vx = -KNIFE_MOVING_SPEED;
		break;
	}
}
