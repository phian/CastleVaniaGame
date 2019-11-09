#include "Whip.h"

void CWhip::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + WHIP_BBOX_WIDTH;

	/*if (state == GOOMBA_STATE_DIE)
		bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
	else*/
	bottom = y + WHIP_BBOX_HEIGHT;
}

void CWhip::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//CGameObject::Update(dt, coObjects);

	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 

	//x += dx;
	//y += dy;

	/*if (vx < 0 && x < 0) {
		x = 0; vx = -vx;
	}

	if (vx > 0 && x > 1465) {
		x = 290; vx = -vx;
	}*/

	if (this->isAttacking == true)
	{
		if (GetTickCount() - this->beginAttackTime > 300)
		{
			this->beginAttackTime = 0;
			this->isAttacking = false;
			this->animations[0]->ResetAnimation();
		}
	}
}

void CWhip::Render()
{
	int ani;

	if (nx > 0) ani = WHIP_ANI_USE_RIGHT;
	else ani = WHIP_ANI_USE_LEFT;

	animations[ani]->Render(x, y);
	//RenderBoundingBox();
}

void CWhip::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case WHIP_STATE_USE_RIGHT:
		nx = 1;
		this->beginAttackTime = GetTickCount(); // Get time Simon start attack
		this->isAttacking = true;
		this->animations[WHIP_ANI_USE_RIGHT]->ResetAnimation();
		break;
	case WHIP_STATE_USE_LEFT:
		nx = -1;
		this->beginAttackTime = GetTickCount(); // Get time Simon start attack
		this->isAttacking = true;
		this->animations[WHIP_ANI_USE_LEFT]->ResetAnimation();
		break;
	}
}
